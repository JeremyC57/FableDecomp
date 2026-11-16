// 00c73af0 CreateDecoderStream
// Leaf: ?CreateDecoderStream@@YIPAVIAudioStream@@PAXPBGABUPKCBufferedReaderParams@@@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23
#include "libpk/IAudioStream.h"
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) int __cdecl rand(void);
#else
#include <stdlib.h>
#endif
extern bool g_ForceTestTone;
long __fastcall ClassifyAudioExtension(const CString& name)
{
 if(g_ForceTestTone)return 2;
 if(CString::SafeStrlen(name.m_pchData)<4)return 3;
 CString tail=name.Right(4);
 if(tail.CompareNoCase(".ogg")==0)return 1;
 if(tail.CompareNoCase(".wma")==0)return 0;
 return 3;
}
#define PK_REPORT_ERROR(line,text) { { CString s; s << ".\\CDecompressorInterface.cpp" << "(" << (long)(line) << ") : " << text << "\n"; if(g_pfnAssertHook)g_pfnAssertHook(s.m_pchData); } CDebugBreak(); }
IAudioStream* __fastcall CreateDecoderStream(void* source,const unsigned short* name,const PKCBufferedReaderParams& params)
{
 char path[0x100];
 WideToMultiByte(name,path,0x100);
 long type;
 {CString classified(path);type=ClassifyAudioExtension(classified);}
 IAudioStream* stream=0;
 if(type==0){
  stream=new CNullAudioStream(source,params);
  if(!stream)PK_REPORT_ERROR(40,"EMEM");
 }
 else if(type==1){
  stream=new COggVorbisStream(source,params);
  if(!stream)PK_REPORT_ERROR(45,"EMEM");
 }
 else if(type==2)stream=new CTestToneAudioStream(((float)rand()*(1.0f/32767.0f)+1.0f)*440.0f);
 return stream;
}