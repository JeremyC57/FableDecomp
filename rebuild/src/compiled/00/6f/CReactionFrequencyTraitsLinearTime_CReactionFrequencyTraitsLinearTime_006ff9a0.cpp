// CReactionFrequencyTraits_LinearTime::CReactionFrequencyTraits_LinearTime(float minWait, float maxWait) -- vtable, MinWait = minWait, WaitRange = maxWait - minWait
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FF9A0, length 29 bytes.
// Leaf: ??0CReactionFrequencyTraits_LinearTimeRetail
// Retail bytes: fld [esp+8] ; mov eax,ecx ; fsub [esp+4] ; mov ecx,[esp+4] ; mov dword ptr [eax],offset vftable(0x01264B64) ; mov [eax+4],ecx ; fstp [eax+8] ; ret 8
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies; the vtable
// 0x01264B64 is CReactionFrequencyTraits_LinearTime (rtti-vtables.tsv: slot 0 0x006FF9C0, slot 1 LogReaction
// 0x006FECF0, slot 4 0x006FED00, GetType 0x00701A20 landed). The PDB layout (engine header) is
// {vptr, float MinWait +4, float WaitRange +8, unsigned long FrameHappened +0xc}; the header models the vptr as a
// plain `void*` member, so the constructor uses a polymorphic retail model with the same names and size. The
// float copy goes through ecx (a plain float assignment), the range is computed on the x87 and stored last.
#include "engine/CReactionFrequencyTraits_LinearTime.h"

struct CReactionFrequencyTraits_LinearTimeRetail {
    virtual void v00();                    // slot 0 = 0x006FF9C0
    float         MinWait;                 // +0x04
    float         WaitRange;               // +0x08
    unsigned long FrameHappened;           // +0x0c (not touched here)
    CReactionFrequencyTraits_LinearTimeRetail(float minWait, float maxWait);
};
FABLE_STATIC_ASSERT(sizeof(CReactionFrequencyTraits_LinearTimeRetail) == sizeof(CReactionFrequencyTraits_LinearTime));

CReactionFrequencyTraits_LinearTimeRetail::CReactionFrequencyTraits_LinearTimeRetail(float minWait, float maxWait)
    : MinWait(minWait), WaitRange(maxWait - minWait)
{
}