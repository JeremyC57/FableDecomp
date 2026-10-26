// CCreatureAction_CastWillLightningSpell::Initialise() -- replace lightning component, configure AI cast and optional target, copy removed item.
// Retail 0x008B9C90, 316 bytes. Base 0x00857520, RemoveTC 0x004C9840, AddTC 0x004C9D60;
// actual aliased donor headers and native action view (donor container tail +0x18).
// Component PItemRemoved really is packed at +0x11, confirmed by both header and native.
#define CTCWillLightning CTCWillLightning_PDB
#include "engine/CTCWillLightning.h"
#undef CTCWillLightning
#define CCreatureAction_CastWillLightningSpell CCreatureAction_CastWillLightningSpell_PDB
#include "engine/CCreatureAction_CastWillLightningSpell.h"
#undef CCreatureAction_CastWillLightningSpell
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CClass.h"
#include "fable_string.h"

class CThing;
class CTCParamsBase;
class CTCWillLightning;
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
    void RemoveTC(long type); // 0x004C9840 marks matching component removed and queues removal
    CTCWillLightning* AddTC(const CCharString&, bool, CTCParamsBase*); // 0x004C9D60
};
FABLE_STATIC_ASSERT(offsetof(CThing, Type) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CThing, LifeFlags) == 0x91);

// Packed member offset is explicit in actual header and retail lea [component+0x11].
#pragma pack(push, 1)
class CTCWillLightning { public:
    unsigned char _nativeBase[0x10];
    bool LeftHandAnim;
    CIntelligentPointer<CThing> PItemRemoved; // native +0x11
    void SetAICast(long); // 0x0079F960 sets timer flag then jumps to SetLevel 0x0079F810
    void SetTimeToCastLightning(float); // 0x0079F5F0
    void SetTargetOverride(CThing*); // 0x0079F910 copies pointer to native +0x88
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(offsetof(CTCWillLightning, PItemRemoved)==0x11);

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

class CCreatureAction_CastWillLightningSpell : public CCombatActionBase { public:
    long Level; // native +0x114
    float Secs; // native +0x118
    void Initialise();
};

void CCreatureAction_CastWillLightningSpell::Initialise()
{
    CCombatActionBase::Initialise();

    PCreature.GetPItem()->RemoveTC(0x94);
    CTCWillLightning* force;
    {
        CCharString name("CTCWillLightning", -1);
        force = PCreature.GetPItem()->AddTC(name, true, 0);
    }

    CThing* creature = PCreature.GetPItem();
    if (creature->Type == 1) {
        force->SetAICast(Level);
        force->SetTimeToCastLightning(Secs);
        if (POptionalItem.GetPItem() && !(POptionalItem.GetPItem()->LifeFlags & 1))
            force->SetTargetOverride(POptionalItem.GetPItem());
    }

    CIntelligentPointer<CThing> source;
    source.SetPItem(PItemRemoved.GetPItem());
    if (source.GetPItem() && !(source.GetPItem()->LifeFlags & 1)) {
        CIntelligentPointer<CThing> target;
        target.SetPItem(source.GetPItem());
        force->PItemRemoved.SetPItem(target.GetPItem());
    }
}