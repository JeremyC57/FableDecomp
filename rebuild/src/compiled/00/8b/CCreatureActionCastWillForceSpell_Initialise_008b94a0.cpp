// CCreatureAction_CastWillForceSpell::Initialise() -- create and configure the force-push TC from this action's native owner, then copy its optional target.
// Retail 0x008B94A0, 254 bytes. Base initialiser 0x00857520; CCharString 0x0099EBF0/0x0099EAE0; CBaseIntelligentPointer 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10; CThing::AddTC 0x004C9D60.
// PDB header is included under an alias; the donor action tail starts 0x18 later than the retail native view because its STL containers are 16 rather than 12 bytes.
#define CCreatureAction_CastWillForceSpell CCreatureAction_CastWillForceSpell_PDB
#include "engine/CCreatureAction_CastWillForceSpell.h"
#undef CCreatureAction_CastWillForceSpell
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CClass.h"
#include "fable_string.h"

class CThing;
class CTCParamsBase;
class CTCForcePushPower;
struct CBaseObjectPointer { CThing* Object; unsigned long refs; };

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();               // deleting-dtor slot at 0x00A01C50; ordinary dtor 0x00A01C10
    CBaseObjectPointer* PData;                        // +0x04, CBaseObjectPointer (Object + refs)
    CBaseIntelligentPointer();                         // 0x00A01B10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing*);                            // 0x00A01B90
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing() : CBaseIntelligentPointer() { }
};

class CThing : public CClass { public:
    unsigned char _nativeTailBeforeLifeFlags[0x80];
    unsigned char LifeFlags;                          // +0x91 retail; named native view of CThing life state
    CTCForcePushPower* AddTC(const CCharString&, bool, CTCParamsBase*); // 0x004C9D60
};
FABLE_STATIC_ASSERT(offsetof(CThing, Type) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CThing, LifeFlags) == 0x91);

class CTCForcePushPower { public:
    unsigned char _pad00[0x10];
    long SpellLevel;                                   // native +0x10
    CIntelligentPointer_CThing PItemRemoved;            // native +0x14
};

class CCombatActionBase { public:
    void* __vftable;
    unsigned char _pad04[0x04];
    CIntelligentPointer_CThing PCreature;              // +0x08
    unsigned char _pad10[0x1c];
    CIntelligentPointer_CThing PItemRemoved;           // +0x2c
    unsigned char _pad34[0x74];
    CIntelligentPointer_CThing POptionalItem;           // +0xa8
    unsigned char _padB0[0x64];
    void Initialise();                                 // 0x00857520
};

class CCreatureAction_CastWillForceSpell : public CCombatActionBase { public:
    unsigned char _pad114[0x04];
    long LevelToCast;                                  // native +0x118
    void Initialise();
};

void CCreatureAction_CastWillForceSpell::Initialise()
{
    CCombatActionBase::Initialise();

    CTCForcePushPower* force;
    {
        CCharString name("CTCForcePushPower", -1);
        force = PCreature.GetPItem()->AddTC(name, true, 0);
    }

    CThing* creature = PCreature.GetPItem();
    if (creature->Type == 1)
        force->SpellLevel = LevelToCast;

    if (POptionalItem.GetPItem())
        POptionalItem.GetPItem();

    CIntelligentPointer_CThing source;
    source.SetPItem(PItemRemoved.GetPItem());
    if (source.GetPItem() && !(source.GetPItem()->LifeFlags & 1)) {
        CIntelligentPointer_CThing target;
        target.SetPItem(source.GetPItem());
        force->PItemRemoved.SetPItem(target.GetPItem());
    }
}