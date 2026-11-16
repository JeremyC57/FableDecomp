#ifndef FABLE_LIBPK_IAUDIOSTREAM_H
#define FABLE_LIBPK_IAUDIOSTREAM_H
#ifdef FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "engine/CString.h"
#include <stddef.h>
#include "libpk/OggVorbisNativeTypes.h"
// Libpk names descriptive: original PDBs/SDK do not cover this wrapper family.
// Native six-slot base012beffc: virtual destructor then five pure operations.
struct SAudioBuffer { short* m_data;unsigned long m_channels; };
class IAudioStream {
public:
 virtual ~IAudioStream(){}
 virtual long GetLength()=0;
 virtual long Seek(unsigned long position,bool exact)=0;
 virtual long Read(SAudioBuffer*,unsigned long frames)=0;
 virtual long GetRate()=0;
 virtual long GetState()=0;
};
class CNullAudioStream:public IAudioStream {
public:
 CNullAudioStream(void*,const PKCBufferedReaderParams&);
 virtual long GetLength();
 virtual long Seek(unsigned long,bool);
 virtual long Read(SAudioBuffer*,unsigned long);
 virtual long GetRate();
 virtual long GetState();
};
class COggVorbisStream:public IAudioStream {
public:
 COggVorbisStream(void*,const PKCBufferedReaderParams&);
 virtual ~COggVorbisStream();
 virtual long GetLength();
 virtual long Seek(unsigned long,bool);
 virtual long Read(SAudioBuffer*,unsigned long);
 virtual long GetRate();
 virtual long GetState();
 PKCBufferedReader m_reader;
 long m_valid,m_opened;
 OggVorbis_File m_vf;
};
class CTestToneAudioStream:public IAudioStream {
public:
 CTestToneAudioStream(float frequency);
 virtual long GetLength();
 virtual long Seek(unsigned long,bool);
 virtual long Read(SAudioBuffer*,unsigned long);
 virtual long GetRate();
 virtual long GetState();
 float m_scale;
 long m_position,m_sampleRate;
};
// Free fastcall classifier is the native const-CString& ABI. Its original
// source scope is unavailable; no fabricated CString subclass is required.
long __fastcall ClassifyAudioExtension(const CString& name);
void __fastcall WideToMultiByte(const unsigned short*,char*,int);
IAudioStream* __fastcall CreateDecoderStream(void*,const unsigned short*,const PKCBufferedReaderParams&);
FABLE_STATIC_ASSERT(sizeof(IAudioStream)==4);
FABLE_STATIC_ASSERT(sizeof(CNullAudioStream)==4);
FABLE_STATIC_ASSERT(sizeof(PKCBufferedReader)==0x70);
FABLE_STATIC_ASSERT(offsetof(COggVorbisStream,m_reader)==4);
FABLE_STATIC_ASSERT(offsetof(COggVorbisStream,m_valid)==0x74);
FABLE_STATIC_ASSERT(offsetof(COggVorbisStream,m_opened)==0x78);
FABLE_STATIC_ASSERT(offsetof(COggVorbisStream,m_vf)==0x80);
FABLE_STATIC_ASSERT(sizeof(COggVorbisStream)==0x340);
FABLE_STATIC_ASSERT(sizeof(CTestToneAudioStream)==0x10);
#endif
#endif
