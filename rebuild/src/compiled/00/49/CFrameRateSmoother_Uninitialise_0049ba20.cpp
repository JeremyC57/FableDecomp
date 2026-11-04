// CFrameRateSmoother::Uninitialise() -- if the smoother is valid, empty the Times array (STLport clear = erase(begin, end)), then the base Uninitialise
// Leaf: ?Uninitialise@CFrameRateSmoother@@QAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BA20, 65 bytes (+ int3 padding to 0x0049BA70), thiscall; both exits tail-jump
//   CInitBaseClass::Uninitialise 0x0099A360.
// Identity (proven, two sources): the only caller 0x00417674 is CMainGameComponent::Shutdown (0x004175E5) with
//   `lea ecx,[esi+16178h]`, the Ego_r.pdb CMainGameComponent member FrameRateSmoother (CFrameRateSmoother, +90488 = +0x16178);
//   Ego_r.pdb and FableWin.pdb ?Uninitialise@CFrameRateSmoother@@QAEXXZ; the row sits in the CFrameRateSmoother block of retail
//   .text between the landed AddFrameTime 0x0049B9E0 and Init 0x0049BA70. The body is the CInitBaseClass pattern: if IsValid(), empty
//   the Times array, then the base Uninitialise. The catalog label "CThingCreatureBase::EditWorldUpdate" is propagated and the
//   landed caller's binding ?Shutdown@CEditWorld@@QAEXXZ is wrong (NAME_FIXES.tsv).
// Callees: ?IsValid@CInitBaseClass@@QBE_NXZ 0x0099A330 (`mov al,[ecx+4]`), _memmove 0x00BFEAE6 (STLport __copy_trivial),
//   ?Uninitialise@CInitBaseClass@@IAEXXZ 0x0099A360 (protected, both PDBs; tail).
// Shape: STLport 4.6 `clear()` = `erase(begin(), end())`, whose trivial-assignment path is `__copy_ptrs(last, finish, first,
//   __true_type)` -> `__copy_trivial`: `(last == first) ? result : memmove(result, first, n) + n` -- with last == finish the
//   compiler still emits retail's `cmp eax,eax ; jne` and the memmove arm; the trivial _Destroy is empty.
// Model: the landed sibling CFrameRateSmoother::Init 0x0049BA70 (CArray<T> : std::vector<T>, CInitBaseClass : CBaseClass with
//   Valid +0x04, Times +0x08 .. SmoothedTime +0x28), with the real STLport 4.6 erase / __copy_ptrs / __copy_trivial text.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

#include "engine/CArray.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInitBaseClass : public CBaseClass {
public:
    bool IsValid() const;                            // 0x0099A330 ?IsValid@CInitBaseClass@@QBE_NXZ
protected:
    bool Initialise();                               // 0x0099A350 ?Initialise@CInitBaseClass@@IAE_NXZ
    void Uninitialise();                             // 0x0099A360 ?Uninitialise@CInitBaseClass@@IAEXXZ
public:
    bool Valid;                                      // +0x04
};
class CFrameRateSmoother : public CInitBaseClass {
public:
    bool Init(long maxNoTimes, long noFramesToChangeFrameRateOver);
    void Uninitialise();
    CArray<float> Times;                             // +0x08
    long          MaxNoTimes;                        // +0x14
    long          NoTimes;                           // +0x18
    long          NoFramesToChangeFrameRateOver;     // +0x1C
    long          FirstTime;                         // +0x20
    long          LastTime;                          // +0x24
    double        SmoothedTime;                      // +0x28
};

void CFrameRateSmoother::Uninitialise()
{
    if (IsValid())
        Times.clear();
    CInitBaseClass::Uninitialise();
}