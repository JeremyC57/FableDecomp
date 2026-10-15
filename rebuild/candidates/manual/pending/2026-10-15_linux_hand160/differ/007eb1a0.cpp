// CTCActionUsePickUpGenericBox::OnActionUse(CThing& user) -- base OnActionUse; if the user is a live creature, fetch the sub-def of the owner's base def and, when the user has the inventory component, set it the action of picking up the owner generic box
// Leaf: ?OnActionUse@CTCActionUsePickUpGenericBox@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB1A0, length 138 bytes (+6 int3 padding). Vtable slot 28 of
// CTCActionUsePickUpGenericBox (FableWin.pdb: ?OnActionUse@CTCActionUsePickUpGenericBox@@UAEXAAVCThing@@@Z). `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified), CDefClassBase::GetSubDef(CDefPointer*) @ 0x005CC10E
// (catalog: the <CAbilityDef> byte-twin), CCreatureAction_PickUpGenericBox::CCreatureAction_PickUpGenericBox
// (CThingCreatureBase&, CThing&) @ 0x00843700 (catalog label: the AddRealObjectToInventory ctor, propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0 (catalog
// "CPhysicsMeshInfo::~CPhysicsMeshInfo"), the def's virtual slot 1 at count zero.
// Shape: the user reference cast to a creature pointer is null-tested; the def pointer local (esp+8) is zeroed after the push;
// interface 70 (bit 6 of the bitset dword at +0x28) tests as `test byte ptr [esi+28h],40h`; the 0xB0-byte action temporary
// (CCreatureActionBase 0xA8 + PReceivedObject) is destroyed inline with no vtable store (a novtable leaf), the def released
// with no zero store (dead local).
// Grade: DIFFER(147v138) -- local def release: retail `mov ecx,[esp+8] ; test ecx,ecx ; je ; dec dword ptr [ecx+4] ; jne ; mov eax,[ecx]`, ours `mov eax,[esp+8] ; test eax,eax ; je ; dec dword ptr [eax+4] ; mov ecx,[esp+8] ; mov eax,[ecx+4] ; test ; jne` (reload); retail zeroes the def after pushing its address | tried: ReleaseTemporary helper (register-form dec), __forceinline dtor, const def + mutable count, direct `Data && --Data->RefCount == 0` (this one)
// Layout: retail CThing (+0x20 component bitset, +0x70 PBaseDef object word, +0x91 bit 0 Dead); CTCBase +4 ParentThing.
class CThing;
struct CDefClassBase;
struct CPickUpDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void ReleaseTemporary() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer()
    {
        if (Data && --Data->RefCount == 0)
            Data->Destroy();
    }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CPickUpDef>* out) const;     // 0x005CC10E
};
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

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
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
        CDefPointer<CPickUpDef> def;
        ParentThing->PBaseDef->GetSubDef(&def);
        if (creature->IsUsingComponent(70)) {
            creature->SetCurrentAction(CCreatureAction_PickUpGenericBox(creature, (CThing*)ParentThing));
        }
    }
}
