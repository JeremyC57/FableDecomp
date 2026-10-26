// CCreatureAction_CastWillTurncoatSpell::PerformAction() -- create turncoat component, select target, configure AI level and removed-item reference.
// Retail 0x008B95A0, 249 bytes; AddTC 0x004C9D60; string ctor/dtor 0x0099EBF0/0x0099EAE0;
// SetPTarget 0x007C6940, SetAICast 0x007C66E0; intelligent pointer methods 0x00A01B10/B50/B90/C10.
// No exact action header exists. Native action tail +0x114 vs donor family +0x12c;
// actual aliased CTCTurncoatSpell header has +4 timer displacement, so native PItemRemoved is +0x54.
#define CTCTurncoatSpell CTCTurncoatSpell_PDB
#include "engine/CTCTurncoatSpell.h"
#undef CTCTurncoatSpell
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CClass.h"
#include "fable_string.h"

class CThing;
class CThingCreatureBase;
class CTCParamsBase;
class CTCTurncoatSpell;
struct CBaseObjectPointer { CThing* Object; unsigned long refs; };

class CBaseObject;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThing : public CClass { public:
    unsigned char _nativeTailBeforeLifeFlags[0x80];
    unsigned char LifeFlags;                          // +0x91 retail; named native view of CThing life state
    CTCTurncoatSpell* AddTC(const CCharString&, bool, CTCParamsBase*); // 0x004C9D60
};
FABLE_STATIC_ASSERT(offsetof(CThing, Type) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CThing, LifeFlags) == 0x91);

class CTCTurncoatSpell { public:
    unsigned char _nativeBase[0x54];
    CIntelligentPointer<CThing> PItemRemoved; // native +0x54, donor +0x58 (timer differs)
    void SetPTarget(CThingCreatureBase*); // 0x007C6940
    void SetAICast(long); // 0x007C66E0
};

class CCombatActionBase { public:
    void* __vftable;
    unsigned char _pad04[0x04];
    CIntelligentPointer<CThing> PCreature;              // +0x08
    unsigned char _pad10[0x1c];
    CIntelligentPointer<CThing> PItemRemoved;           // +0x2c
    unsigned char _pad34[0x74];
    CIntelligentPointer<CThing> POptionalItem;           // +0xa8
    unsigned char _padB0[0x64];
    void Initialise();                                 // 0x00857520
};

class CCreatureAction_CastWillTurncoatSpell : public CCombatActionBase { public:
    long Level; // native +0x114
    void PerformAction();
};

void CCreatureAction_CastWillTurncoatSpell::PerformAction()
{

    CTCTurncoatSpell* force;
    {
        CCharString name("CTCTurncoatSpell", -1);
        force = PCreature.GetPItem()->AddTC(name, true, 0);
    }

    force->SetPTarget(reinterpret_cast<CThingCreatureBase*>(POptionalItem.GetPItem()));
    CThing* creature = PCreature.GetPItem();
    if (creature->Type == 1)
        force->SetAICast(Level);


    CIntelligentPointer<CThing> source;
    source.SetPItem(PItemRemoved.GetPItem());
    if (source.GetPItem() && !(source.GetPItem()->LifeFlags & 1)) {
        CIntelligentPointer<CThing> target;
        target.SetPItem(source.GetPItem());
        force->PItemRemoved.SetPItem(target.GetPItem());
    }
}