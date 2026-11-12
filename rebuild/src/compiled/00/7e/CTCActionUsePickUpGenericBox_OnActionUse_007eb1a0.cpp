// CTCActionUsePickUpGenericBox::OnActionUse(CThing& user) -- base OnActionUse; if the user is a live creature, fetch the sub-def of the owner's base def and, when the user has the inventory component, set it the action of picking up the owner generic box
// Leaf: ?OnActionUse@CTCActionUsePickUpGenericBox@@UAEXAAVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB1A0, length 138 bytes (+6 int3 padding). Vtable slot 28 of
// CTCActionUsePickUpGenericBox (FableWin.pdb: ?OnActionUse@CTCActionUsePickUpGenericBox@@UAEXAAVCThing@@@Z). `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified), CDefClassBase::GetSubDef<CCarryableDef> const @ 0x005CC10E
// (agentNF25b: the body pushes the 'CCarryableDef' literal, so T is CCarryableDef, not the earlier local CPickUpDef; now on
// engine/CDefClassBase_GetSubDef.h with the per-TU no-inline opt-in), CCreatureAction_PickUpGenericBox::CCreatureAction_PickUpGenericBox
// (CThingCreatureBase&, CThing&) @ 0x00843700 (catalog label: the AddRealObjectToInventory ctor, propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0 (catalog
// "CPhysicsMeshInfo::~CPhysicsMeshInfo"), the def's virtual slot 1 at count zero.
// Shape: the user reference cast to a creature pointer is null-tested; the def pointer local (esp+8) is zeroed after the push;
// interface 70 (bit 6 of the bitset dword at +0x28) tests as `test byte ptr [esi+28h],40h`; the 0xB0-byte action temporary
// (CCreatureActionBase 0xA8 + PReceivedObject) is destroyed inline with no vtable store (a novtable leaf), the def released
// with no zero store (dead local).
// Layout: retail CThing (+0x20 component bitset, +0x70 PBaseDef object word, +0x91 bit 0 Dead); CTCBase +4 ParentThing.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // the template body kept out of line (0x005CC10E), visible to the TU
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    unsigned char _pad44[0x70 - 0x44];
    CDefClassBase* PBaseDef;                       // +0x70 CDefPointer<CThingBaseDef const> object word
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                         // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long IsUsingComponent(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingObject : public CThing {};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCreatureActionBase {                        // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_PickUpGenericBox : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PThingToPickUp;        // +0xa8 CIntelligentPointer<CThing>
    CCreatureAction_PickUpGenericBox(CThingCreatureBase* creature, CThing* thing);   // 0x00843700
};

inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};
class CTCActionUseBase : public CTCBase {
public:
    virtual void OnActionUse(CThing& user);        // 0x007EAB90
};
class CTCActionUsePickUpGenericBox : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};

void CTCActionUsePickUpGenericBox::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase* creature = CastToCreature(&user);
    if (creature && !creature->IsDead()) {
        CDefPointer<const CCarryableDef> def;
        ParentThing->PBaseDef->GetSubDef(&def);
        if (creature->IsUsingComponent(70)) {
            creature->SetCurrentAction(CCreatureAction_PickUpGenericBox(creature, (CThing*)ParentThing));
        }
    }
}