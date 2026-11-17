// CV_TempleOfLightScript::WatchForAcolyteExit() -- script thread: once the hero has attacked, collect the TOL_AcolyteGood things, wait until all are gone and set AllAcolytesEscaped
// Leaf: ?WatchForAcolyteExit@CV_TempleOfLightScript@NScript@@QAEXXZ
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE (header opt-in) + a scoped #pragma inline_depth(0) region at the merged end --
//   retail expands ~vector<CScriptThing> at the wait loop's terminating return and calls the COMDAT 0x008AC970 at the end; with the
//   header's plain inline ~vector VC7.1 calls the COMDAT at every exit (the twin 0x00EE6A40 graded DIFFER(246v441) that way).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDD060, length 270 bytes, plain `ret`.
// Label: catalog "CQ_BountyHuntScript::WatchForAllGruntsKilled" is propagated; the body is started as the thread
// "WatchForAcolyteExit" (0x012F3E28) by CV_TempleOfLightScript::Main (0x00EDB190, functor store at 0x00EDB3AD).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, AreAllThingsInVectorDead (__fastcall, 0x00CBED00), vector<CScriptThing>::~vector
// (out-of-line COMDAT copy 0x008AC970, propagated label "~vector<CConversation>"), CScriptThing's virtual destructor
// (slot 0) and free 0x00BFEA14 in the inlined copy; through the game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), GetAllThingsWithScriptName (slot 75, +0x12c). Literal "TOL_AcolyteGood" 0x012F3E54.
// Layout: Ego_r.pdb CV_TempleOfLightScript - 0x14: AttackedByHero +0x5b, AllAcolytesEscaped +0x5d.
// Shape: the inline-once destructor band: retail expands the local vector's destructor at the wait loop's terminating
// return and calls the out-of-line copy at the normal end (the two closing terminating tests jump to that call); the
// automatic destructor runs at both exits, the end one inside the inline_depth(0) region.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
class CV_TempleOfLightScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    long DonationTotal;                              // +0x48
    long DonationThisVisit;                          // +0x4c
    long DonationCounter;                            // +0x50
    bool DonationInvalid;                            // +0x54
    bool DonationActive;                             // +0x55
    bool DisplayQuestInfo;                           // +0x56
    bool WeaponWarningComplete;                      // +0x57
    bool WeaponUnsheathed;                           // +0x58
    bool TempleIntroComplete;                        // +0x59
    bool InsideTemple;                               // +0x5a
    bool AttackedByHero;                             // +0x5b
    bool AcolyteAttackMessageComplete;               // +0x5c
    bool AllAcolytesEscaped;                         // +0x5d
    void WatchForAcolyteExit();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForAcolyteExit()
{
    while (!AttackedByHero) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> acolytes;
    m_pInterface->GetAllThingsWithScriptName("TOL_AcolyteGood", acolytes);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (AreAllThingsInVectorDead(acolytes))
            break;
        m_pInterface->NewScriptFrame();
    }
#pragma inline_depth(0)
    if (!IsActiveThreadTerminating() && !IsActiveThreadTerminating())
        AllAcolytesEscaped = true;
}
#pragma inline_depth()