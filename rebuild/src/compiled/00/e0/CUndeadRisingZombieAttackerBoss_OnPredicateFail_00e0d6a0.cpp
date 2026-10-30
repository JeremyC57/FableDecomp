// CUndeadRisingZombieAttackerBoss::OnPredicateFail() -- when the boss's predicate fails (its death), play the ZOMBIE_BOSS1_DEATH effect at its position
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0D6A0, length 179 bytes (192 with padding); slot 5 of the vtable at
// 0x012DFFDC (no RTTI locator: the entity-script module).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2), ~CCharString @ 0x0099EAE0 (x2), Me's virtual
// GetPos (slot 6, +0x18), the game interface's CreateEffect(const CCharString&, const C3DVector&, const CCharString&,
// float, bool, bool) (slot 100, +0x190, CScriptThing by hidden pointer), CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete
// @ 0x00BFE9BC. Literals: "ZOMBIE_BOSS1_DEATH" @ 0x012DFFF8, "" @ 0x0122D70E.
// Layout: the CEntityScriptBase family (Game +4, CScriptThing Me +8).
// Shape: the landed CPermanentBarrier::Main (0x00D2D830) CreateEffect call: both literals are implicit conversions built
// first (the empty bone name before the name), the zero pushes and the bone name's address precede Me.GetPos(); the
// effect handle is a named local destroyed (inline) after the two string temporaries.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SScriptThingInfo {                                   // the counted block behind CScriptThing::PImp_Info
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    void*             PImp_Data;                            // +0x04
    SScriptThingInfo* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;                // slot 6 (+0x18)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CUndeadRisingZombieAttackerBoss {
public:
    void*                 __vftable;                        // +0x00
    CGameScriptInterface* Game;                             // +0x04
    CScriptThing          Me;                               // +0x08
    void OnPredicateFail();
};

void CUndeadRisingZombieAttackerBoss::OnPredicateFail()
{
    CScriptThing effect = Game->CreateEffect("ZOMBIE_BOSS1_DEATH", Me.GetPos(), "", 0.0f, false, false);
}