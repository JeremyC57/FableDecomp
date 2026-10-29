// CTCCarriedActionUseSpade::vfunc_28(CThingCreatureBase* user) -- a user able to dig starts the "Expression_Dig" activate-quest action; then the spade's interface-0x1a component is notified
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F03D0, 187 bytes (`ret 4`, 0xbc-byte frame). OnActionUse(CThing&) slot 28.
// Callees: the empty base hook @ 0x007EAB90 (another TU), IsThingAbleToDig @ 0x007F0300 (`__fastcall`, user in ecx),
// CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_ActivateQuest(CThingCreatureBase*, CThing*, const CCharString&,
// bool) @ 0x00843F50, CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CBaseIntelligentPointer @ 0x00A01C10, ~CCharString
// @ 0x0099EAE0, ~CCreatureActionBase @ 0x00693EF0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the
// interface-0x1a component's OnShowEnded @ 0x005D8E50 (descriptive, as CEventShownItem 0x008116D0). Literal "Expression_Dig"
// @ 0x01271908.
// Layout: the action as the landed ctor 0x00843F50 (retail CCreatureActionBase 0xa8; ScriptName +0xa8, LoadResources +0xac,
// PObjectToUse +0xad: the intelligent-pointer classes are `pack(1)`, the action keeps its 4-byte alignment), 0xb8 bytes of frame beside the 4-byte name string.
// Shape: the name is a named local in the dig block; the action is a temporary passed by address straight from the ctor's eax
// (GOTCHAS chunk 160), destroyed member by member with no vtable stores (the action class is `__declspec(novtable)`, the
// intelligent pointer has no user dtor); the 0x1a component is loaded straight into ecx (HasTC + one-ternary GetTC).
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#pragma pack(push, 1)                                   // the pointer classes are 1-aligned so the member lands at +0xad
class CBaseObject;
class CBaseObjectPointer;
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
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};   // CIntelligentPointer<CThing>
#pragma pack(pop)
class CCreatureActionBase { public:                            // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                             // 0x00693EF0
};
class CThingCreatureBase;
struct __declspec(novtable) CCreatureAction_ActivateQuest : CCreatureActionBase {
    CCharString         ScriptName;                     // +0xa8
    bool                LoadResources;                  // +0xac
    CIntelligentPointer<CThing> PObjectToUse;                   // +0xad
    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);   // 0x00843F50
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    bool SetCurrentAction(const CCreatureActionBase& action); // 0x006644F0
};
bool __fastcall IsThingAbleToDig(CThingCreatureBase* thing);   // 0x007F0300
struct CTCShowableItem {
    void OnShowEnded();                                 // 0x005D8E50
};
class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                    // +0x04 (CTCBase)
    void OnUseBase(CThingCreatureBase* user);           // 0x007EAB90 (empty base implementation)
};
struct CTCCarriedActionUseSpade : CTCCarriedActionUseBase {
    void vfunc_28(CThingCreatureBase* user);
};

void CTCCarriedActionUseSpade::vfunc_28(CThingCreatureBase* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (IsThingAbleToDig(user)) {
        CCharString name("Expression_Dig", -1);
        user->SetCurrentAction(CCreatureAction_ActivateQuest(user, 0, name, false));
    }
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}