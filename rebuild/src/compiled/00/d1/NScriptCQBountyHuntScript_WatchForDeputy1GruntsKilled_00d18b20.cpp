// NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled() -- script thread: collect the BH_GangGruntSpawn things, wait until all are dead (stopping when the thread is terminated) and set AllDeputy1GruntsKilled
// Leaf: ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE (header opt-in) + a scoped #pragma inline_depth(0) region at the merged end --
//   retail expands ~vector<CScriptThing> at the wait loop's terminating return and calls the COMDAT 0x008AC970 at the end; with the
//   header's plain inline ~vector VC7.1 calls the COMDAT at every exit (the twin 0x00EE6A40 graded DIFFER(246v441) that way).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d18b20, 214 bytes, thiscall, plain `ret`.
// Identity (proven): its only reference is the functor store at 0x00D18906 (thread "WatchForDeputy1GruntsKilled"), the spawned-function registration of that thread name; Ego_r.pdb ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ; the flag it sets is Ego_r.pdb CQ_BountyHuntScript::AllDeputy1GruntsKilled (PDB +0x71, retail +0x5d with the script base 0x14 smaller).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, AreAllThingsInVectorDead (__fastcall) 0x00CBED00, vector<CScriptThing>::~vector 0x008AC970 (the out-of-line
//   copy at the normal end), CScriptThing's virtual destructor (slot 0) and free 0x00BFEA14 in the inlined copy; through the
//   game script interface (+0x40): GetAllThingsWithScriptName (slot 75, +0x12c), NewScriptFrame (slot 7, +0x1c).
//   Literal "BH_GangGruntSpawn" 0x012CA46C.
// Shape (family L140, as WatchForAcolyteExit 0x00EDD060): retail expands the local vector's destructor at the wait loop's
//   terminating return and calls the out-of-line copy at the normal end (both arms of the final terminating test, tail-merged,
//   its `lea ecx` hoisted above the branch); the automatic destructor runs at every exit, the end ones inside the inline_depth(0)
//   region.
#include "fable_compiler.h"
// NF48 (round 7): the local vector is STLport 4.6.2 std::vector<CScriptThing> from the shared stlport/fable_containers.h (a local
// namespace-std vector model with Destroy() / DestroyInline() before); its destructor is ~vector<CScriptThing> 0x008AC970, expanded at
// the terminating return (FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE, the header body) and called out of line at the merged end,
// where the inline_depth(0) region (the lever of the landed twin 0x00EE6A40) keeps the COMDAT call.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE
#include "stlport/fable_containers.h"
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCPPointerInfo;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

using std::vector;
bool __fastcall AreAllThingsInVectorDead(const vector<CScriptThing>& things);   // 0x00CBED00

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_BountyHuntScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    unsigned char         _pad44[0x5d - 0x44];
    bool                  AllDeputy1GruntsKilled;              // +0x5d
    void WatchForDeputy1GruntsKilled();
};
} // namespace NScript

void NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled()
{
    vector<CScriptThing> things;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", things);
    while (!AreAllThingsInVectorDead(things)) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    AllDeputy1GruntsKilled = true;
}
#pragma inline_depth()