#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __int64 long long
#endif
// 00c425e0 DecodeXboxAdpcm
// Flags: /EHsc
// Leaf: ?DecodeXboxAdpcm@@YIJPBXKKPAXK@Z
#include "engine/CImaAdpcmCodec.h"
// Naming: descriptive libpk wrapper; original codec types come from the SDK.
long __fastcall DecodeXboxAdpcm(const void* source, unsigned long sourceBytes,
    unsigned long channels, void* destination, unsigned long capacity)
{
    unsigned long blocks = sourceBytes / (channels * 36);
    unsigned long bytesPerBlock = channels << 7;
    unsigned long decodedBytes = bytesPerBlock * blocks;
    if (decodedBytes > capacity) return -1;
    CImaAdpcmCodec codec;
    IMAADPCMWAVEFORMAT format;
    CImaAdpcmCodec::CreateImaAdpcmFormat((WORD)channels, 44100, 64, &format);
    if (!codec.Initialize(&format, CODEC_MODE_DECODE)) return -1;
    if (!codec.Convert(source, destination, blocks)) return -1;
    return decodedBytes;
}

static int Trace[12], Used, Mode, ThrowAt, Destroyed;
static CImaAdpcmCodec* Instance;
static const void* Source;
static void* Destination;
static unsigned ExpectedChannels, ExpectedBlocks;
static void Step(int code) {
    Trace[Used++] = code;
#ifndef _MSC_VER
    if (code == ThrowAt) throw code;
#endif
}
CImaAdpcmCodec::CImaAdpcmCodec(){Instance=this;Step(1);}
CImaAdpcmCodec::~CImaAdpcmCodec(){assert(this==Instance);++Destroyed;Step(5);}
void __fastcall CImaAdpcmCodec::CreateImaAdpcmFormat(WORD channels,DWORD rate,WORD count,LPIMAADPCMWAVEFORMAT format){
    assert(channels==ExpectedChannels && rate==44100 && count==64);
    memset(format,0,sizeof(*format));
    format->wfx.wFormatTag=0x69;format->wfx.nChannels=channels;
    format->wfx.nSamplesPerSec=rate;format->wfx.nBlockAlign=36*channels;
    format->wfx.nAvgBytesPerSec=rate*format->wfx.nBlockAlign/count;
    format->wfx.wBitsPerSample=4;format->wfx.cbSize=2;format->wSamplesPerBlock=count;
    Step(2);
}
BOOL CImaAdpcmCodec::Initialize(LPCIMAADPCMWAVEFORMAT format,CODEC_MODE mode){
    assert(this==Instance && mode==CODEC_MODE_DECODE);
    assert(format->wfx.wFormatTag==0x69 && format->wfx.nChannels==ExpectedChannels);
    assert(format->wfx.nSamplesPerSec==44100 && format->wSamplesPerBlock==64);
    Step(3);return Mode!=1;
}
BOOL CImaAdpcmCodec::Convert(LPCVOID source,LPVOID destination,UINT blocks){
    assert(this==Instance && source==Source && destination==Destination && blocks==ExpectedBlocks);
    Step(4);return Mode!=2;
}
int main(){
    char source[2000],destination[4000];Source=source;Destination=destination;
    unsigned cases=0;
    for(unsigned channels=1;channels<=2;++channels)
    for(unsigned blocks=0;blocks<13;++blocks)
    for(unsigned tail=0;tail<3;++tail)
    for(unsigned capacityCase=0;capacityCase<3;++capacityCase)
    for(int mode=0;mode<3;++mode){
        ExpectedChannels=channels;ExpectedBlocks=blocks;Mode=mode;ThrowAt=0;Used=Destroyed=0;
        unsigned decoded=channels*128*blocks;
        unsigned capacity=capacityCase==0?(decoded?decoded-1:0):decoded+capacityCase-1;
        unsigned bytes=channels*36*blocks+(tail==2?channels*36-1:tail);
        long result=DecodeXboxAdpcm(Source,bytes,channels,Destination,capacity);
        bool called=decoded<=capacity;
        assert(result==((!called||mode)?-1:(long)decoded));
        assert(Destroyed==(called?1:0));
        if(!called)assert(Used==0);
        else {assert(Trace[0]==1&&Trace[1]==2&&Trace[2]==3&&Trace[Used-1]==5);
            assert(Used==(mode==1?4:5));if(mode!=1)assert(Trace[3]==4);}
        ++cases;
    }
    assert(cases==702);
#ifndef _MSC_VER
    ExpectedChannels=1;ExpectedBlocks=1;Mode=0;
    for(int failure=2;failure<=4;++failure){
        ThrowAt=failure;Used=Destroyed=0;bool caught=false;
        try{DecodeXboxAdpcm(Source,36,1,Destination,128);}catch(int code){assert(code==failure);caught=true;}
        assert(caught&&Destroyed==1&&Trace[Used-1]==5);
    }
#endif
    puts("012152d0_TEST PASS");return 0;
}
