// NScript::CQ_SecretPassageScript::CSP_NymphTrigger::Main() -- Secret Passage nymph trigger: wait (frame by frame) until the hero comes within 10 units of the trigger, then spawn the succubus nymph at the trigger with the hero-seeking script
// Leaf: Main
// Identity (naming wave 11): ?Main@CSP_NymphTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DDD60 slot 1 is stored by the factory 0x00DED290 registered as "SP_NymphTrigger"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DED380, length 208 bytes (`ret`).
// Callees: IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// ~CCharString @ 0x0099EAE0, ~CScriptThing @ 0x004AA840; CScriptThing (vtable 0x01238C8C) slot 6 GetPos;
// CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature(const CCharString&, const C3DVector&,
// const CCharString&, bool) (CScriptThing by value).
// Layout = Ego_r.pdb CSP_NymphTrigger (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (CQ_SecretPassageScript*), PMasterData +0x18.
// Shape: the top-tested wait loop of the landed CHTG_AmbushPoint::Main 0x00D73610 (distance test repeated at the
// bottom, the 10.0f pushed before GetHero); the two literal names are implicit CCharString conversions built before
// every other argument (right to left), the created creature is a named local destroyed after them.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_SecretPassageScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CSP_NymphTrigger;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_SecretPassageScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SecretPassageScript::CSP_NymphTrigger : public CEntityScriptBase {
public:
        CQ_SecretPassageScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_SecretPassageScript::CSP_NymphTrigger::Main()
{
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing nymph = Game->CreateCreature("CREATURE_SUCCUBUS_NYMPH_01", Me.GetPos(), "EnemySeekingHero", true);
}