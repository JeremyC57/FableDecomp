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
class CGameScriptInterface { public:                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& position, const CCharString& boneName, float angle, bool alwaysUpdate, bool persistent) const;   // slot 100 (+0x190) CGameScriptInterface::CreateEffect @ 0x0089F9E0
};

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