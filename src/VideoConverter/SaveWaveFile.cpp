#include "stdafx.h"
#include "XDebug.h"
#include "SaveWaveFile.h"

#ifdef _DEBUG

#define RIFF_TAG                    0x46464952
#define WAVE_TAG                    0x45564157
#define FMT__TAG                    0x20746D66
#define DATA_TAG                    0x61746164

#pragma pack(1)

typedef struct _WAVE_FILE_HEADER
{
	uint32_t dwRiff;			// FIFF 头文件标志
	uint32_t dwFileSize;		// 文件尺寸
	uint32_t dwWave;			// WAVE 文件标志
	uint32_t dwFormat;			// FMT 文件标志
	uint32_t dwFormatLength;	// FMT 的长度
	WAVEFORMATEX Format;		// 音频格式
	uint32_t dwData;			//
	uint32_t dwDataLength;		//

} WAVE_FILE_HEADER;

#pragma pack()

static WAVEFORMATEX wfx;
static HANDLE hFile;
static uint32_t datalen;

int32_t CreateWaveFile(const wchar_t *file, int32_t channels, int32_t samples_rat, int32_t bits_per_sample)
{
	DWORD fsize;

	DeleteFile(file);

	hFile = CreateFile(file, 
						GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		ErrorLog("Faild to create file:%s\n", file);
		hFile = 0;
		return -1;
	}
	
	fsize = SetFilePointer(hFile, sizeof(WAVE_FILE_HEADER), NULL, FILE_BEGIN);
	if (fsize == 0xFFFFFFFF)
	{
		ErrorLog("Faild to seek file\n", file);
		CloseHandle(hFile);
		hFile = 0;
		return -1;
	}
	wfx.wFormatTag = 1;
	wfx.nChannels = channels;
	wfx.nSamplesPerSec = samples_rat;
	wfx.nAvgBytesPerSec = samples_rat * channels * bits_per_sample / 8;
	wfx.nBlockAlign = channels * bits_per_sample / 8;
	wfx.wBitsPerSample = bits_per_sample;
	return 0;
}

void PutWaveFileData(void *data, int32_t size)
{
	DWORD nBytesWrite;
	if(WriteFile(hFile, data, size, &nBytesWrite, NULL) == 0)
	{
		ErrorLog("Faild to write\n");
	}
	datalen += size;
}

void CloseWaveFile()
{
	DWORD nBytesWrite;
	WAVE_FILE_HEADER Header;

	Header.dwRiff = RIFF_TAG;			// FIFF 头文件标志
	Header.dwFileSize = datalen + sizeof(Header);		// 文件尺寸
	Header.dwWave = WAVE_TAG;			// WAVE 文件标志
	Header.dwFormat = FMT__TAG;		// FMT 文件标志
	Header.dwFormatLength = sizeof(WAVEFORMATEX); // FMT 的长度
	Header.Format = wfx;			// 音频格式
	Header.dwData = DATA_TAG;			//
	Header.dwDataLength = datalen;	//

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	WriteFile(hFile, &Header, sizeof(WAVE_FILE_HEADER), &nBytesWrite,NULL);
	CloseHandle(hFile);
	hFile = 0;

}

#endif
