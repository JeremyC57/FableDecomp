// CVtable_012bedb8::RebuildGain(bool alternate) -- under the effect's lock record the selected gain record and rebuild it from the volume: the previous record (+0x34) when alternate is set, else the current one (+0x30), each through the class's ComputeGainRamp (gain = min(FtoL(volume * 32767), 0x7fff) >> 1, step = gain >> 7)
// Leaf: ?RebuildGain@CVtable_012bedb8@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBE0, 106 bytes (+ int3 padding), thiscall `ret 4`, one dword frame word.
// Identity (descriptive; libpk = the GameCODA software mixer, no PDB / RTTI / FableWin copy: the retail byte runs are absent from
//   FableWin.exe / ego_r.exe / Ego_d.exe): the owner-accepted vendor-naming-V2 spelling ghidra_out/labels_vendor_v2_2026-11-04.tsv
//   line 72, bound by rebuild/oracles/reloc_bindings.tsv (?RebuildGain@CVtable_012bedb8@@QAEX_N@Z, caller SetVolume 0x00C6EC50) and
//   the landed callers SetVolume 0x00C6EC50 / SetParameters 0x00C6EC80; the twin of the landed CVtable_012bed90::RebuildGain 0x00C6DEA0
//   and CVtable_012bed7c::RebuildGain 0x00C6CF40. Layout as the landed ComputeGainRamp 0x00C6EBA0 / ctor 0x00C6E760 (no shared header
//   for these libpk classes exists; the model below is the landed sibling's text).
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the lock +0x08; 32767.0f 0x0129C820.
// Retail-asm: 0x00C6EC0A / 0x00C6EC0E and 0x00C6EC17 / 0x00C6EC1B `fld dword ptr [esp+10h] ; fistp dword ptr [esp+8]` -- libpk's
//   inline-assembly float-to-long helper FtoL (shared libpk/FtoL.h: byte path + FABLE_PORTABLE twin, docs/pipeline/ASM_RULES.md); this
//   row has no asm of its own. VC7.1 `(long)x` calls __ftol2 and /QIfist emits `fistp qword`.
// What closed it: the same-TU ComputeGainRamp 0x00C6EBA0 (address-taken, emitted out of line) defined in this TU; VC7.1 expands it in
//   both arms of the record choice, hoisting the shared `m_volume * 32767` product into the dead `alternate` slot (0x00C6EBEF..0x00C6EC01)
//   and keeping the record pointer in ecx. The earlier drafts' hand-written helpers left the pointer in esi (DIFFER 106v106). One
//   ComputeGainRamp call on `alternate ? m_pPrevious : m_pCurrent` is DIFFER(98v106). This TU's ComputeGainRamp copy also grades
//   RELOCATION_MATCH at 0x00C6EBA0.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
#include "libpk/FtoL.h"
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 {                          // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
    void*            _vtable;                      // +0x00
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear
    bool             m_alternate;                  // +0x24
};
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
struct CVtable_012bedb8 : CVtable_012beda4 {       // vtable 0x012BEDB8 (ctor 0x00C6E760)
    SGainRamp  m_first;                            // +0x28
    SGainRamp  m_second;                           // +0x2c
    SGainRamp* m_pCurrent;                         // +0x30
    SGainRamp* m_pPrevious;                        // +0x34
    void ComputeGainRamp(SGainRamp* ramp);         // 0x00C6EBA0
    void RebuildGain(bool alternate);              // 0x00C6EBE0
};
void CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp)
{
    unsigned long level = FtoL(m_volume * 32767.0f);
    if (level > 0x7fff)
        level = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)level >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
}
void CVtable_012bedb8::RebuildGain(bool alternate)
{
    EnterCriticalSection(&m_lock);
    m_alternate = alternate;
    if (alternate)
        ComputeGainRamp(m_pPrevious);
    else
        ComputeGainRamp(m_pCurrent);
    LeaveCriticalSection(&m_lock);
}