// Self-contained behavior fixture; selected Convert body pasted below.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __int64 long
#endif
#ifndef FABLE_RETAIL_CIMAADPCM_CODEC_H
#define FABLE_RETAIL_CIMAADPCM_CODEC_H
// Original Microsoft Xbox IMA ADPCM SDK interface, 2001.
// Copyright (C) 2001 Microsoft Corporation. All Rights Reserved.
// Layout: SDK imaadpcm.h and native Convert/Initialize: format+0, mode+20,
// left index+24, right index+28, converter+32, size36 in the native ABI.
typedef unsigned char BYTE;
typedef unsigned short WORD;
#ifdef _MSC_VER
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif
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

// CImaAdpcmCodec::Convert(LPCVOID, LPVOID, UINT) -- convert IMA ADPCM with the selected step-index search mode
// Leaf: ?Convert@CImaAdpcmCodec@@QAEHPBXPAXI@Z
// Flags: /Gr

#include <string.h>
extern "C" int __cdecl CodecPrintf(const char*,...);
void* __cdecl operator new[](size_t size);
void __cdecl operator delete[](void* p);
BOOL CImaAdpcmCodec::Convert(LPCVOID pvSrc,LPVOID pvDst,UINT cBlocks)
{
 static const LPFNIMAADPCMCONVERT apfnDecoders[2]={DecodeM16,DecodeS16};
 DWORD dwSrcBlockSize=m_wfxEncode.wfx.nChannels*m_wfxEncode.wSamplesPerBlock*sizeof(short);
 DWORD dwDstBlockSize=m_wfxEncode.wfx.nBlockAlign;
 memset(pvDst,0,cBlocks*dwDstBlockSize);
 switch(m_cmCodecMode) {
 case CODEC_MODE_DECODE:
  return m_pfnConvert((LPBYTE)pvSrc,(LPBYTE)pvDst,cBlocks,m_wfxEncode.wfx.nBlockAlign,m_wfxEncode.wSamplesPerBlock,&m_nStepIndexL,&m_nStepIndexR);
 case CODEC_MODE_ENCODE_NORMAL:
  {
   CodecPrintf("Using normal encoding...\n");
   LPBYTE pvDecoded=new BYTE[cBlocks*dwSrcBlockSize];
   if(!pvDecoded)return 0;
   LPFNIMAADPCMCONVERT pfnOppConvert=apfnDecoders[m_wfxEncode.wfx.nChannels-1];
   if(!m_pfnConvert((LPBYTE)pvSrc,(LPBYTE)pvDst,cBlocks,m_wfxEncode.wfx.nBlockAlign,m_wfxEncode.wSamplesPerBlock,&m_nStepIndexL,&m_nStepIndexR)){delete[]pvDecoded;return 0;}
   if(!pfnOppConvert((LPBYTE)pvDst,pvDecoded,cBlocks,m_wfxEncode.wfx.nBlockAlign,64,&m_nStepIndexL,&m_nStepIndexR)){delete[]pvDecoded;return 0;}
   CodecPrintf("Difference between original and decoded streams: 0x%I64x\n",CalcDifference((LPBYTE)pvSrc,pvDecoded,cBlocks,cBlocks,dwSrcBlockSize));
   delete[]pvDecoded;
  }
  break;
 case CODEC_MODE_ENCODE_OPTIMIZE_WHOLE_FILE:
  {
   CodecPrintf("Using whole file encoding...\n");
   LPBYTE pvTempDst=new BYTE[cBlocks*dwDstBlockSize];
   if(!pvTempDst)return 0;
   LPBYTE pvDecoded=new BYTE[cBlocks*dwSrcBlockSize];
   if(!pvDecoded){delete[]pvTempDst;return 0;}
   LPFNIMAADPCMCONVERT pfnOppConvert=apfnDecoders[m_wfxEncode.wfx.nChannels-1];
   ULONGLONG ullLeastDiff=(ULONGLONG)-1;
   UINT uChosen=(UINT)-1;
   for(UINT i=0;i<sizeof(m_asStep)/sizeof(m_asStep[0]);++i){
    memset(pvTempDst,0,cBlocks*dwDstBlockSize);
    memset(pvDecoded,0,cBlocks*dwSrcBlockSize);
    m_nStepIndexL=m_nStepIndexR=i;
    if(!m_pfnConvert((LPBYTE)pvSrc,pvTempDst,cBlocks,m_wfxEncode.wfx.nBlockAlign,m_wfxEncode.wSamplesPerBlock,&m_nStepIndexL,&m_nStepIndexR))continue;
    if(!pfnOppConvert(pvTempDst,pvDecoded,cBlocks,m_wfxEncode.wfx.nBlockAlign,64,&m_nStepIndexL,&m_nStepIndexR))continue;
    ULONGLONG ullDiff=CalcDifference((LPBYTE)pvSrc,pvDecoded,cBlocks,cBlocks,dwSrcBlockSize);
    if(ullDiff<ullLeastDiff){ullLeastDiff=ullDiff;uChosen=i;memcpy(pvDst,pvTempDst,cBlocks*dwDstBlockSize);}
   }
   CodecPrintf("Difference between original and decoded streams: 0x%I64x\n",ullLeastDiff);
   CodecPrintf("Step index chosen: %d\n",uChosen);
   delete[]pvTempDst;delete[]pvDecoded;
  }
  break;
 case CODEC_MODE_ENCODE_OPTIMIZE_EACH_BLOCK:
  {
   CodecPrintf("Using per-block encoding\n\n");
   LPBYTE pvTempDst=new BYTE[dwDstBlockSize];if(!pvTempDst)return 0;
   LPBYTE pvDecoded=new BYTE[dwSrcBlockSize];if(!pvDecoded){delete[]pvTempDst;return 0;}
   LPFNIMAADPCMCONVERT pfnOppConvert=apfnDecoders[m_wfxEncode.wfx.nChannels-1];
   INT iPreviousBestStepIndex=-1;
   for(UINT c=0;c<cBlocks;++c){
    ULONGLONG ullLeastDiff=(ULONGLONG)-1;
    INT iThisBestStepIndex=-1,iStartIndex,iStopIndex;
    if(iPreviousBestStepIndex==-1){iStartIndex=0;iStopIndex=sizeof(m_asStep)/sizeof(m_asStep[0]);}
    else{iStartIndex=iPreviousBestStepIndex-24;iStopIndex=iPreviousBestStepIndex+25;}
    for(INT i=iStartIndex;i<iStopIndex;++i){
     if(i<0||i>=sizeof(m_asStep)/sizeof(m_asStep[0]))continue;
     memset(pvTempDst,0,dwDstBlockSize);memset(pvDecoded,0,dwSrcBlockSize);
     m_nStepIndexL=m_nStepIndexR=i;
     if(!m_pfnConvert((LPBYTE)pvSrc+c*dwSrcBlockSize,pvTempDst,1,m_wfxEncode.wfx.nBlockAlign,m_wfxEncode.wSamplesPerBlock,&m_nStepIndexL,&m_nStepIndexR))continue;
     if(!pfnOppConvert(pvTempDst,pvDecoded,1,m_wfxEncode.wfx.nBlockAlign,64,&m_nStepIndexL,&m_nStepIndexR))continue;
     ULONGLONG ullDiff=CalcDifference((LPBYTE)pvSrc+c*dwSrcBlockSize,pvDecoded,1,cBlocks,dwSrcBlockSize);
     if(ullDiff<ullLeastDiff){ullLeastDiff=ullDiff;iThisBestStepIndex=i;memcpy((LPBYTE)pvDst+c*dwDstBlockSize,pvTempDst,dwDstBlockSize);}
    }
    iPreviousBestStepIndex=iThisBestStepIndex;
   }
   delete[]pvTempDst;delete[]pvDecoded;
   pvDecoded=new BYTE[cBlocks*dwSrcBlockSize];
   pfnOppConvert((LPBYTE)pvDst,pvDecoded,cBlocks,m_wfxEncode.wfx.nBlockAlign,64,&m_nStepIndexL,&m_nStepIndexR);
   CodecPrintf("Difference between original and decoded streams: 0x%I64x\n",CalcDifference((LPBYTE)pvSrc,pvDecoded,cBlocks,cBlocks,dwSrcBlockSize));
   delete[]pvDecoded;
  }
  break;
 }
 return 1;
}


static int CallsEncode,CallsDecode,CallsDiff,CallsPrint,FailEncode,FailDecode,SkipStep;
static int LastSamples,LastAlign,LastCount,LastStereo,LastTotal,Steps[200];
static int StepCount;
static BOOL TestEncode(LPBYTE src,LPBYTE dst,UINT blocks,UINT align,UINT samples,LPINT left,LPINT right,int stereo)
{
 ++CallsEncode;LastSamples=samples;LastAlign=align;LastCount=blocks;LastStereo=stereo;
 assert(*left==*right);assert(*left>=0&&*left<89);assert(samples==64);
 if(StepCount<200)Steps[StepCount++]=*left;
 if(FailEncode || *left==SkipStep)return 0;
 for(UINT b=0;b<blocks;++b){for(UINT k=0;k<align;++k)assert(dst[b*align+k]==0);memset(dst+b*align,*left,align);}
 (void)src;return 1;
}
static BOOL TestDecode(LPBYTE src,LPBYTE dst,UINT blocks,UINT align,UINT samples,LPINT left,LPINT right,int stereo)
{
 ++CallsDecode;LastSamples=samples;LastAlign=align;LastCount=blocks;LastStereo=stereo;
 assert(samples==64);assert(left&&right);
 if(FailDecode)return 0;
 for(UINT b=0;b<blocks;++b)for(UINT j=0;j<samples*(stereo+1);++j)((short*)dst)[b*samples*(stereo+1)+j]=src[b*align];
 return 1;
}
BOOL __fastcall CImaAdpcmCodec::EncodeM16(LPBYTE a,LPBYTE b,UINT c,UINT d,UINT e,LPINT f,LPINT g){return TestEncode(a,b,c,d,e,f,g,0);}
BOOL __fastcall CImaAdpcmCodec::EncodeS16(LPBYTE a,LPBYTE b,UINT c,UINT d,UINT e,LPINT f,LPINT g){return TestEncode(a,b,c,d,e,f,g,1);}
BOOL __fastcall CImaAdpcmCodec::DecodeM16(LPBYTE a,LPBYTE b,UINT c,UINT d,UINT e,LPINT f,LPINT g){return TestDecode(a,b,c,d,e,f,g,0);}
BOOL __fastcall CImaAdpcmCodec::DecodeS16(LPBYTE a,LPBYTE b,UINT c,UINT d,UINT e,LPINT f,LPINT g){return TestDecode(a,b,c,d,e,f,g,1);}
CImaAdpcmCodec::CImaAdpcmCodec(){}
CImaAdpcmCodec::~CImaAdpcmCodec(){}
BOOL __fastcall CImaAdpcmCodec::IsValidImaAdpcmFormat(LPCIMAADPCMWAVEFORMAT f){return f&&f->wfx.nChannels>=1&&f->wfx.nChannels<=2&&f->wSamplesPerBlock==64;}
BOOL CImaAdpcmCodec::Initialize(LPCIMAADPCMWAVEFORMAT f,CODEC_MODE mode)
{
 static const LPFNIMAADPCMCONVERT funcs[2][2]={{DecodeM16,DecodeS16},{EncodeM16,EncodeS16}};
 if(!IsValidImaAdpcmFormat(f))return 0;
 m_wfxEncode=*f;m_cmCodecMode=mode;m_pfnConvert=funcs[!(mode==CODEC_MODE_DECODE)][f->wfx.nChannels-1];m_nStepIndexL=m_nStepIndexR=0;return 1;
}
extern "C" int __cdecl CodecPrintf(const char*,...){++CallsPrint;return 0;}
ULONGLONG CImaAdpcmCodec::CalcDifference(LPBYTE a,LPBYTE b,UINT count,UINT total,DWORD blockSize)
{
 ++CallsDiff;LastTotal=total;
 ULONGLONG result=0;
 const ULONGLONG maximumContribution=(ULONGLONG)-1/total;
 const ULONGLONG maximumDifference=((ULONGLONG)1<<32)*m_wfxEncode.wSamplesPerBlock;
 for(UINT i=0;i<count;++i){
  short* first=(short*)(a+i*blockSize);short* second=(short*)(b+i*blockSize);ULONGLONG difference=0;
  for(UINT j=0;j<m_wfxEncode.wSamplesPerBlock;++j){ULONGLONG sample=(ULONGLONG)second[j]-(ULONGLONG)first[j];difference+=sample*sample;}
  assert(difference<=maximumDifference);
  result+=(ULONGLONG)(((double)difference/(double)maximumDifference)*maximumContribution);
 }
 return result;
}
static void ResetTest()
{
 CallsEncode=CallsDecode=CallsDiff=CallsPrint=0;FailEncode=FailDecode=0;SkipStep=-1;StepCount=0;LastSamples=LastAlign=LastCount=LastStereo=LastTotal=-1;
}
static IMAADPCMWAVEFORMAT Format(int stereo)
{
 IMAADPCMWAVEFORMAT f;memset(&f,0,sizeof(f));f.wfx.wFormatTag=0x69;f.wfx.nChannels=(WORD)(stereo+1);f.wfx.nBlockAlign=(WORD)(36*(stereo+1));f.wSamplesPerBlock=64;return f;
}
static void Fill(short* source,int stereo,int blocks,int first,int second)
{
 for(int b=0;b<blocks;++b)for(int j=0;j<64*(stereo+1);++j)source[b*64*(stereo+1)+j]=(short)(b?second:first);
}
int main()
{
#ifdef _MSC_VER
 assert(sizeof(WAVEFORMATEX)==18&&sizeof(IMAADPCMWAVEFORMAT)==20&&sizeof(CImaAdpcmCodec)==36);
#endif
 short source[384];unsigned char output[768];CImaAdpcmCodec codec;
 for(int stereo=0;stereo<2;++stereo){
  IMAADPCMWAVEFORMAT f=Format(stereo);const int stride=f.wfx.nBlockAlign;
  ResetTest();assert(codec.Initialize(&f,CODEC_MODE_DECODE));memset(source,7,sizeof(source));memset(output,0x55,sizeof(output));
  assert(codec.Convert(source,output,2));assert(CallsDecode==1&&CallsEncode==0&&CallsPrint==0);assert(LastCount==2&&LastSamples==64&&LastAlign==stride&&LastStereo==stereo);assert(((short*)output)[0]==7&&((short*)output)[64*(stereo+1)]==7);
  ResetTest();FailDecode=1;assert(codec.Convert(source,output,1)==0);assert(CallsDecode==1);for(int q=0;q<stride;++q)assert(output[q]==0);
  ResetTest();Fill(source,stereo,2,25,25);assert(codec.Initialize(&f,CODEC_MODE_ENCODE_NORMAL));assert(codec.Convert(source,output,2));assert(CallsEncode==1&&CallsDecode==1&&CallsDiff==1&&CallsPrint==2);assert(LastTotal==2&&LastStereo==stereo);for(int q=0;q<2*stride;++q)assert(output[q]==0);
  ResetTest();FailEncode=1;assert(!codec.Convert(source,output,2));assert(CallsEncode==1&&CallsDecode==0&&CallsDiff==0);
  ResetTest();FailDecode=1;assert(!codec.Convert(source,output,2));assert(CallsEncode==1&&CallsDecode==1&&CallsDiff==0);
  ResetTest();assert(codec.Initialize(&f,CODEC_MODE_ENCODE_OPTIMIZE_WHOLE_FILE));assert(codec.Convert(source,output,2));assert(CallsEncode==89&&CallsDecode==89&&CallsDiff==89&&CallsPrint==3);for(int q=0;q<2*stride;++q)assert(output[q]==25);
  ResetTest();SkipStep=25;assert(codec.Convert(source,output,2));assert(CallsEncode==89&&CallsDecode==88&&CallsDiff==88);for(int q=0;q<2*stride;++q)assert(output[q]==24);
  ResetTest();FailDecode=1;assert(codec.Convert(source,output,2));assert(CallsEncode==89&&CallsDecode==89&&CallsDiff==0);for(int q=0;q<2*stride;++q)assert(output[q]==0);
  ResetTest();Fill(source,stereo,2,25,80);assert(codec.Initialize(&f,CODEC_MODE_ENCODE_OPTIMIZE_EACH_BLOCK));assert(codec.Convert(source,output,2));assert(CallsEncode==138&&CallsDecode==139&&CallsDiff==139&&CallsPrint==2);for(int q=0;q<stride;++q){assert(output[q]==25);assert(output[q+stride]==49);}assert(Steps[89]==1&&Steps[137]==49&&LastTotal==2);
  ResetTest();Fill(source,stereo,2,0,88);assert(codec.Convert(source,output,2));assert(CallsEncode==114&&CallsDecode==115);for(int q=0;q<stride;++q){assert(output[q]==0);assert(output[q+stride]==24);}assert(Steps[89]==0&&Steps[113]==24);
  ResetTest();Fill(source,stereo,2,88,0);assert(codec.Convert(source,output,2));assert(CallsEncode==114&&CallsDecode==115);for(int q=0;q<stride;++q){assert(output[q]==88);assert(output[q+stride]==64);}assert(Steps[89]==64&&Steps[113]==88);
  ResetTest();assert(codec.Initialize(&f,(CODEC_MODE)99));assert(codec.Convert(source,output,2));assert(!CallsEncode&&!CallsDecode&&!CallsPrint);for(int q=0;q<stride*2;++q)assert(output[q]==0);
 }
 puts("00c5fa50_TEST PASS");return 0;
}
