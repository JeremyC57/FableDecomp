// CVtable_012b8ab4::Process(const short* input, long* const* outputs) -- vtable slot 2 of libpk's MMX four-record gain mixer: under the effect lock either cross-fade (ramp each current gain record down on its output, swap the current / previous record arrays, ramp the new records up) or mix the input at each record's constant Q15 gain into outputs 0-1 (and 2-3 with four or more output channels), through the MMX kernels
// Leaf: ?Process@CVtable_012b8ab4@@UAEXPBFPBQAJ@Z
// Flags: /GS
// Retail-asm: 0x00C6F863 `pmulhw mm0,mm7` (0x00C6F852..0x00C6F8C2 `movq mm7,[gains] ; movq mm6,[increments] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; paddsw mm7,mm6 ; ... ; dec ecx ; jne ; emms`) -- the hand MMX ramp kernel CUnprovenMixKernel_00c6f800 (landed retail_asm) is a static of this retail TU: VC7.1 passes its `initial` argument in ecx only to a same-TU caller (`mov cx,[eax]` / `xor ecx,ecx` before each call here), so its landed body is repeated verbatim; Process itself is plain C++ (precedent: the landed MMX Process 0x00C6FE10 and SSE2 Process 0x00C6D630).
struct CRITICAL_SECTION { void* DebugInfo; long LockCount, RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CUnprovenLibraryObject_012B5868 { public: virtual ~CUnprovenLibraryObject_012B5868() {} };
class CUnprovenLock_012B8FC8 { public: virtual ~CUnprovenLock_012B8FC8() {} CRITICAL_SECTION Section; };
class CVtable_012beda4 : public CUnprovenLibraryObject_012B5868 {
public:
    virtual ~CVtable_012beda4() {}
    virtual void Clear() {}
    virtual void Process(const short*, long* const*) {}
    virtual void SetVolume(float, unsigned long, bool) = 0;
    virtual void SetParameters(long, long, long, float, long, bool, bool) {}
    CUnprovenLock_012B8FC8 Lock;
    float m_volume;
    bool  m_crossfade;
};
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C
struct SGainRamp {
    unsigned short m_gain;
    short          m_step;
};
class CVtable_012bed90 : public CVtable_012beda4 {
public:
    SGainRamp  m_first[6];                                                  // +0x28
    SGainRamp  m_second[6];                                                 // +0x40
    SGainRamp* m_pCurrent;                                                  // +0x58
    SGainRamp* m_pPrevious;                                                 // +0x5c
    float      m_scales[6];                                                 // +0x60
};
class CVtable_012b8ab4 : public CVtable_012bed90 {
public:
    virtual void Process(const short* input, long* const* outputs);
};
void __cdecl ScaleAccumulate16MMX_00c6f730(const short* src, unsigned long count, long* dst, short gain);   // 0x00C6F730
static __declspec(noinline) void __cdecl CUnprovenMixKernel_00c6f800(const short* src,unsigned long count,long* dst,short initial,short step){
__declspec(align(16)) short gains[4];__declspec(align(16)) short increments[4];
gains[0]=(short)(initial+0*step);
gains[1]=(short)(initial+1*step);
gains[2]=(short)(initial+2*step);
gains[3]=(short)(initial+3*step);
increments[0]=(short)(step*4);
increments[1]=(short)(step*4);
increments[2]=(short)(step*4);
increments[3]=(short)(step*4);
__asm {
 mov ecx,count
 mov eax,src
 mov edx,dst
 movq mm7,gains
 movq mm6,increments
 align 16
 next_block:
 movq mm0,[eax]
 pmulhw mm0,mm7
 paddsw mm7,mm6
 pxor mm1,mm1
 pxor mm2,mm2
 punpcklwd mm1,mm0
 punpckhwd mm2,mm0
 psrad mm1,15
 psrad mm2,15
 paddd mm1,[edx]
 paddd mm2,[edx+8]
 movq [edx],mm1
 movq [edx+8],mm2
 movq mm3,[eax+8]
 pmulhw mm3,mm7
 paddsw mm7,mm6
 pxor mm4,mm4
 pxor mm5,mm5
 punpcklwd mm4,mm3
 punpckhwd mm5,mm3
 psrad mm4,15
 psrad mm5,15
 paddd mm4,[edx+10h]
 paddd mm5,[edx+18h]
 movq [edx+10h],mm4
 add eax,10h
 movq [edx+18h],mm5
 add edx,20h
 dec ecx
 jne next_block
 emms
}
}
void MixGainRampUpMMX(const short* source, const SGainRamp* ramp, long* accumulator)          // 0x00C6F900 (no callers)
{
    if (ramp->m_step != 0)
        CUnprovenMixKernel_00c6f800(source, 16, accumulator, 0, ramp->m_step);
}
void MixGainRampDownMMX(const short* source, const SGainRamp* ramp, long* accumulator)        // 0x00C6F930 (no callers)
{
    if (ramp->m_gain != 0)
        CUnprovenMixKernel_00c6f800(source, 16, accumulator, ramp->m_gain, -ramp->m_step);
}
void MixGainMMX(const short* source, unsigned long count, const SGainRamp* ramp, long* accumulator)   // 0x00C6F960 (no callers)
{
    if (ramp->m_gain != 0)
        ScaleAccumulate16MMX_00c6f730(source, count, accumulator, ramp->m_gain);
}
void CVtable_012b8ab4::Process(const short* input, long* const* outputs)
{
    CRITICAL_SECTION* section = &Lock.Section;
    EnterCriticalSection(section);
    if (m_crossfade) {
        m_crossfade = false;
        MixGainRampDownMMX(input, &m_pCurrent[0], outputs[0]);
        MixGainRampDownMMX(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampDownMMX(input, &m_pCurrent[2], outputs[2]);
            MixGainRampDownMMX(input, &m_pCurrent[3], outputs[3]);
        }
        SGainRamp* current = m_pCurrent;
        m_pCurrent = m_pPrevious;
        m_pPrevious = current;
        MixGainRampUpMMX(input, &m_pCurrent[0], outputs[0]);
        MixGainRampUpMMX(input, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainRampUpMMX(input, &m_pCurrent[2], outputs[2]);
            MixGainRampUpMMX(input, &m_pCurrent[3], outputs[3]);
        }
    } else {
        MixGainMMX(input, 16, &m_pCurrent[0], outputs[0]);
        MixGainMMX(input, 16, &m_pCurrent[1], outputs[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            MixGainMMX(input, 16, &m_pCurrent[2], outputs[2]);
            MixGainMMX(input, 16, &m_pCurrent[3], outputs[3]);
        }
    }
    LeaveCriticalSection(section);
}