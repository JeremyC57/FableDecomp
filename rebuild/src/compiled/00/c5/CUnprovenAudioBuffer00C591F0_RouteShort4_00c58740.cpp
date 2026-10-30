// CUnprovenAudioBuffer_00C591F0::RouteShort4(const short* source, const unsigned short* gains) -- route four planar 16-bit source blocks with per-plane Q-gains to mono (quarter gain), stereo (half gain, alternating) or four output channels
// Leaf: ?RouteShort4@CUnprovenAudioBuffer_00C591F0@@QAEXPBFPBG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58740, 309 bytes, thiscall `ret 8`.
// Identity (descriptive; libpk has no PDB or RTTI): the 16-bit sibling of the landed RouteFloat4 0x00C58520 and RouteInt4
//   0x00C58630 on the same mixing-buffer class (vtable 0x012B8F38): it branches on ChannelCount (+0x20) == 1 / == 2 /
//   other, takes 128-sample planes (0x100 bytes of shorts) and calls virtual slot 3 MixShortGain(const short*, int*,
//   unsigned short) with Buffers[0..3] (+4..+0x10). Unlike its siblings it takes a per-plane gain array: mono uses
//   gains[i] >> 2, stereo gains[i] >> 1 (16-bit `shr cx,2` / `shr ax,1` after a zero-extending load), four channels the gain
//   as given. Its only caller is 0x00C5C6E9 (`push esi+22Ah ; push edi`: the voice's gain words and the decoded block).
//   The chunk label "AudioFormat_DecodeWithChannelShift" is a descriptive guess. Class model copied from 0x00C58630.
// Callees: none direct (slot-3 virtual calls, the vtable and Buffers re-read for each call).
#ifndef _MSC_VER
#define __cdecl
#endif
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CUnprovenAudioBuffer_00C591F0:public CUnprovenLibraryObject_012B5868{public:int* Buffers[6];unsigned int OutputShift,ChannelCount;unsigned char UnalignedStorage[6*512+16];virtual ~CUnprovenAudioBuffer_00C591F0();virtual void Reset();virtual void Clear();virtual void MixShortGain(const short*,int*,unsigned short);virtual void MixIntGain(const int*,int*,unsigned short);virtual void MixInt(const int*,int*);virtual void MixFloatGain(const float*,int*,float);virtual void MixFloatQ15Gain(const float*,int*,unsigned short);virtual void MixFloat(const float*,int*);virtual void Route(unsigned int,const void*);virtual void Output(short*,unsigned int);void RouteFloat4(const float*);void RouteInt4(const int*);void RouteShort4(const short*,const unsigned short*);};
#ifdef _MSC_VER
typedef char ExactBufferSize[sizeof(CUnprovenAudioBuffer_00C591F0)==0xC34?1:-1];
#endif
void CUnprovenAudioBuffer_00C591F0::RouteShort4(const short* source, const unsigned short* gains)
{
    if (ChannelCount == 1) {
        MixShortGain(source, Buffers[0], gains[0] >> 2);
        MixShortGain(source + 128, Buffers[0], gains[1] >> 2);
        MixShortGain(source + 256, Buffers[0], gains[2] >> 2);
        MixShortGain(source + 384, Buffers[0], gains[3] >> 2);
    }
    else if (ChannelCount == 2) {
        MixShortGain(source, Buffers[0], gains[0] >> 1);
        MixShortGain(source + 128, Buffers[1], gains[1] >> 1);
        MixShortGain(source + 256, Buffers[0], gains[2] >> 1);
        MixShortGain(source + 384, Buffers[1], gains[3] >> 1);
    }
    else {
        MixShortGain(source, Buffers[0], gains[0]);
        MixShortGain(source + 128, Buffers[1], gains[1]);
        MixShortGain(source + 256, Buffers[2], gains[2]);
        MixShortGain(source + 384, Buffers[3], gains[3]);
    }
}