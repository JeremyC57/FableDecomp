// CTCAISummon::IsTimeToSummon() -- true when nothing was summoned yet, the def has no interval, or the interval has elapsed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FDE00, length 88 bytes (+ int3 padding).
// Callees: GetWorldFrame @ 0x0049D870 (returns the global frame counter @ 0x013B89BC; the manifest's
// "GFGetPreMainMemoryUsed" label is propagated), __ftol2 @ 0x00BFEA70. Globals: the frame rate (long @
// 0x01375550, as the landed CTCTurncoat::SetTurncoatTimeLast 0x007C7810), the 0.0f constant @ 0x0122DEDC.
//
// Layout = Ego_r.pdb CTCAISummon: CTCBase 0xc, CDefPointer<CSummonDef> PSummonDef +0x0c, the
// MySummonedCreatures list (PDB 12 bytes, retail one sentinel pointer at +0x10), LastSummonTick +0x14
// (PDB +0x1c). CSummonDef::TimeBetweenSummons is +0x40 (seconds).
// Shape: two early `return true` exits; the frame rate is read into a local BEFORE the frame-counter
// call (retail copies the global into the `push ecx` slot ahead of the call and `fild`s it after), the
// def pointer is loaded once (esi) for the zero test and the multiply, and the elapsed-frames compare
// is a signed `>` written `? true : false` (retail `setg al`; the bare comparison goes through
// `xor ecx,ecx ; setg cl ; mov al,cl`).
#include "engine/CSummonDef.h"                           // TimeBetweenSummons +0x40

struct CThing;

extern long g_FramesPerSecond;                              // 0x01375550 (.data), 15
long GetWorldFrame();                                       // 0x0049D870

template<class T>
struct CDefPointer {
    const T* m_pDef;
    const T* operator->() const { return m_pDef; }
};

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

struct CTCAISummon : CTCBase_Calls {
    CDefPointer<CSummonDef> PSummonDef;                     // +0x0c
    void*                   MySummonedCreatures;            // +0x10 retail list (sentinel pointer)
    long                    LastSummonTick;                 // +0x14
    bool IsTimeToSummon();
};

bool CTCAISummon::IsTimeToSummon()
{
    if (LastSummonTick == 0)
        return true;
    if (PSummonDef->TimeBetweenSummons == 0.0f)
        return true;
    long framesPerSecond = g_FramesPerSecond;
    return GetWorldFrame() - LastSummonTick > (long)(framesPerSecond * PSummonDef->TimeBetweenSummons) ? true : false;
}
