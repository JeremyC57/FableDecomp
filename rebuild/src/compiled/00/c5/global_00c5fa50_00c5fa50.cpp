// CImaAdpcmCodec::Convert(LPCVOID, LPVOID, UINT) -- convert IMA ADPCM with the selected step-index search mode
// Leaf: ?Convert@CImaAdpcmCodec@@QAEHPBXPAXI@Z
// Flags: /Gr
#include "engine/CImaAdpcmCodec.h"
#include <string.h>
extern "C" __declspec(dllimport) int __cdecl printf(const char*,...);
void* __cdecl operator new[](unsigned int size);
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
   printf("Using normal encoding...\n");
   LPBYTE pvDecoded=new BYTE[cBlocks*dwSrcBlockSize];
   if(!pvDecoded)return 0;
   LPFNIMAADPCMCONVERT pfnOppConvert=apfnDecoders[m_wfxEncode.wfx.nChannels-1];
   if(!m_pfnConvert((LPBYTE)pvSrc,(LPBYTE)pvDst,cBlocks,m_wfxEncode.wfx.nBlockAlign,m_wfxEncode.wSamplesPerBlock,&m_nStepIndexL,&m_nStepIndexR)){delete[]pvDecoded;return 0;}
   if(!pfnOppConvert((LPBYTE)pvDst,pvDecoded,cBlocks,m_wfxEncode.wfx.nBlockAlign,64,&m_nStepIndexL,&m_nStepIndexR)){delete[]pvDecoded;return 0;}
   printf("Difference between original and decoded streams: 0x%I64x\n",CalcDifference((LPBYTE)pvSrc,pvDecoded,cBlocks,cBlocks,dwSrcBlockSize));
   delete[]pvDecoded;
  }
  break;
 case CODEC_MODE_ENCODE_OPTIMIZE_WHOLE_FILE:
  {
   printf("Using whole file encoding...\n");
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
   printf("Difference between original and decoded streams: 0x%I64x\n",ullLeastDiff);
   printf("Step index chosen: %d\n",uChosen);
   delete[]pvTempDst;delete[]pvDecoded;
  }
  break;
 case CODEC_MODE_ENCODE_OPTIMIZE_EACH_BLOCK:
  {
   printf("Using per-block encoding\n\n");
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
   printf("Difference between original and decoded streams: 0x%I64x\n",CalcDifference((LPBYTE)pvSrc,pvDecoded,cBlocks,cBlocks,dwSrcBlockSize));
   delete[]pvDecoded;
  }
  break;
 }
 return 1;
}