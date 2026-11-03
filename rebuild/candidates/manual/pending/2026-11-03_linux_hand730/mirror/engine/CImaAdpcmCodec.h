#ifndef FABLE_RETAIL_CIMAADPCM_CODEC_H
#define FABLE_RETAIL_CIMAADPCM_CODEC_H
// Original Microsoft Xbox IMA ADPCM SDK interface, 2001.
// Copyright (C) 2001 Microsoft Corporation. All Rights Reserved.
// Layout: SDK imaadpcm.h and native Convert/Initialize: format+0, mode+20,
// left index+24, right index+28, converter+32, size36 in the native ABI.
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef int BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned __int64 ULONGLONG;
typedef BYTE* LPBYTE;
typedef INT* LPINT;
typedef const void* LPCVOID;
typedef void* LPVOID;
// Original mmreg.h type definitions and its standard SDK include boundary.
// Copyright (C) Microsoft Corporation, 1991 - 1999.
#ifdef _MSC_VER
#include <pshpack1.h>
#endif
typedef struct tWAVEFORMATEX {
 WORD wFormatTag,nChannels;
 DWORD nSamplesPerSec,nAvgBytesPerSec;
 WORD nBlockAlign,wBitsPerSample,cbSize;
} WAVEFORMATEX;
typedef WAVEFORMATEX* LPWAVEFORMATEX;
typedef struct ima_adpcmwaveformat_tag {
 WAVEFORMATEX wfx;WORD wSamplesPerBlock;
} IMAADPCMWAVEFORMAT;
typedef IMAADPCMWAVEFORMAT* LPIMAADPCMWAVEFORMAT;
typedef const WAVEFORMATEX* LPCWAVEFORMATEX;
typedef const IMAADPCMWAVEFORMAT* LPCIMAADPCMWAVEFORMAT;
#ifdef _MSC_VER
#include <poppack.h>
#endif
typedef BOOL (__fastcall* LPFNIMAADPCMCONVERT)(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
enum CODEC_MODE {
 CODEC_MODE_DECODE,CODEC_MODE_ENCODE_NORMAL,CODEC_MODE_ENCODE_OPTIMIZE_WHOLE_FILE,CODEC_MODE_ENCODE_OPTIMIZE_EACH_BLOCK
};
class CImaAdpcmCodec {
private:
 static const short m_asNextStep[16];
 static const short m_asStep[89];
 IMAADPCMWAVEFORMAT m_wfxEncode;
 CODEC_MODE m_cmCodecMode;
 int m_nStepIndexL,m_nStepIndexR;
 LPFNIMAADPCMCONVERT m_pfnConvert;
public:
 CImaAdpcmCodec();~CImaAdpcmCodec();
 BOOL Initialize(LPCIMAADPCMWAVEFORMAT,CODEC_MODE);
 WORD GetEncodeAlignment();WORD GetDecodeAlignment();WORD GetSourceAlignment();WORD GetDestinationAlignment();
 BOOL Convert(LPCVOID,LPVOID,UINT);void Reset();
 static void __fastcall CreatePcmFormat(WORD,DWORD,LPWAVEFORMATEX);
 static void __fastcall CreateImaAdpcmFormat(WORD,DWORD,WORD,LPIMAADPCMWAVEFORMAT);
 static BOOL __fastcall IsValidPcmFormat(LPCWAVEFORMATEX);
 static BOOL __fastcall IsValidImaAdpcmFormat(LPCIMAADPCMWAVEFORMAT);
private:
 static WORD __fastcall CalculateEncodeAlignment(WORD,WORD);
 static BOOL __fastcall EncodeM16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall EncodeS16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall DecodeM16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall DecodeS16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static int __fastcall EncodeSample(int,int*,int);
 static int __fastcall DecodeSample(int,int,int);
 static int __fastcall NextStepIndex(int,int);static BOOL __fastcall ValidStepIndex(int);
 ULONGLONG CalcDifference(LPBYTE,LPBYTE,UINT,UINT,DWORD);
};
#endif
