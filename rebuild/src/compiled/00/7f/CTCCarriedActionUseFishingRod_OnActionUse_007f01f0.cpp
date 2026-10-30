// CTCCarriedActionUseFishingRod::OnActionUse(CThing& user) -- base OnActionUse; a creature able to fish gets the "Expression_Fish" quest-activation action on the rod; then the rod's showable component is notified
// Grade: RELOCATION_MATCH -- the parent thing argument through the PDB inline accessor CTCBase::GetParentThing() (retail loads ParentThing into eax before the action temporary's other arguments); was register allocation only
// Leaf: ?OnActionUse@CTCCarriedActionUseFishingRod@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F01F0, length 189 bytes. Vtable slot 28 ("vfunc_28"), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), the static __fastcall
// CTCCarriedActionUseFishingRod::IsThingAbleToFish(const CThingCreatureBase&) @ 0x007EFF80, CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest @ 0x00843F50,
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC for the showable component, type 0x1A), the
// showable component's OnShowEnded @ 0x005D8E50 (landed name). Literal "Expression_Fish" 0x012718F8.
// Shape: the expression name is a named local in the fishing block (destroyed after the action temporary); the action is the
// CTCActionUseActivateQuest temporary (0x007EF600 model: novtable leaf, unaligned intelligent pointer at +0xAD); the closing
// lookup is HasTC + one-ternary GetTC (component in ecx).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class CThing {
public:
    unsigned char _pad00[0x70];
    void* PDef;                                     // +0x70 (CDefPointer<const CThingBaseDef>, unused here)
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                          // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                    // unaligned inside the action (+0xad)
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
#pragma pack(pop)
class CCreatureActionBase {                         // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                 // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_ActivateQuest : public CCreatureActionBase {
public:
    CCharString ScriptName;                         // +0xa8
    bool        LoadResources;                      // +0xac
    CBaseIntelligentPointer PObjectToUse;           // +0xad
    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,
                                  const CCharString& scriptName, bool loadResources);   // 0x00843F50
};

struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThingWithComponents {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CTCShowableItem {                         // interface type 0x1a
public:
    void OnShowEnded();                         // 0x005D8E50
};
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    virtual void OnActionUse(CThing& user);     // 0x007EAB90
    CThing& GetParentThing() { return *ParentThing; }   // CTCBase inline (Ego_r ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ)
    CThingWithComponents* GetParentComponents() const { return (CThingWithComponents*)ParentThing; }
};
class CTCCarriedActionUseFishingRod : public CTCActionUseBase {
public:
    static bool __fastcall IsThingAbleToFish(const CThingCreatureBase& creature);   // 0x007EFF80
    virtual void OnActionUse(CThing& user);
};

void CTCCarriedActionUseFishingRod::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase& creature = (CThingCreatureBase&)user;
    if (IsThingAbleToFish(creature)) {
        CCharString expression("Expression_Fish");
        creature.SetCurrentAction(CCreatureAction_ActivateQuest(creature, GetParentThing(), expression, false));
    }
    if (GetParentComponents()->HasTC(0x1a))
        GetParentComponents()->GetTC<CTCShowableItem>(0x1a)->OnShowEnded();
}