// CSP_NymphTrigger::Main() -- Secret Passage nymph trigger: wait (frame by frame) until the hero comes within 10 units of the trigger, then spawn the succubus nymph at the trigger with the hero-seeking script
// Leaf: Main
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
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    virtual ~CScriptThing();                               // 0x004AA840
    unsigned char _body[8];
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SecretPassageScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CSP_NymphTrigger : public CEntityScriptBase {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_SecretPassageScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CSP_NymphTrigger::Main()
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