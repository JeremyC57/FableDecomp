// CTCActionUseScriptedHook::OnInitialActivate() -- base activation, make sure the thing is targetable (adding CTCTargeted when missing) and has entity events, resolve the connected entrance, and look up the camera track by UID
// Grade: DIFFER(290v290) -- only the two AddTC calls: retail loads the thing into ecx before the `push 0 ; push 0 ;
//   lea eax ; push eax` arguments, ours after them (the known "AddTC this-load before pushes" gap of the retry-10
//   Setup* family); the register of the name's `lea` (eax/edx v ecx/edx) follows
// Leaf: ?OnInitialActivate@CTCActionUseScriptedHook
// Retail: Fable.exe (TLC, VC7.1) @ 0x00729F60, 290 bytes (+14 int3 padding), `ret`; the TC OnInitialActivate slot
//   (catalog name as given).
// Callees: CTCBase::OnInitialActivate 0x00686870 (manifest "$E2" is propagated), CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCTargeted, type 9),
//   CTCTargeted::AddTargetingFlag(ETargetingType) 0x007EA240, the entrance resolver 0x00729BC0 (chunk 168 label
//   correction: a CTCActionUseScriptedHook member resolving EntranceConnectedToUID; descriptive name),
//   CTCBase::GetThingManager 0x00686AE0, CThingManager::GetThingWithUID(unsigned __int64) 0x0051ED80,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10.
//   Literals "CTCTargeted" 0x01239FC0, "CTCEntityEvents" 0x0123ADF8.
// Layout = Ego_r.pdb CTCActionUseScriptedHook: EntranceConnectedToUID +0x18, PEntranceConnectedTo +0x20, CameraTrackUID
//   +0x30, PCameraTrack +0x38; CTCTargeted::Targetable +0x10; interface 0x42 = CTCEntityEvents.
// Shape: each component name is a named CCharString in its own block; the freshly added CTCTargeted is read through
//   the unchecked accessor (an unset local when absent: it shares the name's dead frame word), its Targetable flag set
//   before AddTargetingFlag(1);  the presence mask 0x200 is kept in edi for both tests; the camera track is assigned
//   through the intelligent pointer's inline operator= from a CIntelligentPointer(thing) temporary.
// Tried: an inline GetParentThing() accessor, a named parent local read after the string, an inline forwarding
//   AddTC(name) overload: the thing load always follows the pushes.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CTCParamsBase;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
enum ETargetingType { TARGETING_TYPE_1 = 1 };
struct CTCTargeted {                            // interface 9
    unsigned char _pad00[0x10];
    bool Targetable;                            // +0x10
    void AddTargetingFlag(ETargetingType flag); // 0x007EA240
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CThingManager {
    CThing* GetThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};

class CTCActionUseScriptedHook {
public:
    void*            __vftable;                 // +0x00 (CTCBase)
    CThing*          ParentThing;               // +0x04
    bool             ToBeRemoved;               // +0x08
    bool             IsInGlobalUpdateTCs;       // +0x09
    unsigned char    _pad0a[0x18 - 0x0a];
    unsigned __int64 EntranceConnectedToUID;    // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x20
    bool             EntranceLocked;            // +0x28
    unsigned char    _pad29[0x30 - 0x29];
    unsigned __int64 CameraTrackUID;            // +0x30
    CIntelligentPointer<CThing> PCameraTrack;   // +0x38
    void CTCBase_OnInitialActivate();           // 0x00686870 (CTCBase::OnInitialActivate)
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void SetupEntranceConnectedTo();            // 0x00729BC0
    __forceinline CTCTargeted* GetTargeted() const
    {
        CTCTargeted* targeted;
        if (ParentThing->HasTC(9))
            targeted = ParentThing->GetTC<CTCTargeted>(9);
        return targeted;
    }
    void OnInitialActivate();
};

void CTCActionUseScriptedHook::OnInitialActivate()
{
    CTCBase_OnInitialActivate();
    if (!ParentThing->HasTC(9)) {
        {
            CCharString name("CTCTargeted", -1);
            ParentThing->AddTC(name, false, 0);
        }
        CTCTargeted* targeted = GetTargeted();
        targeted->Targetable = true;
        targeted->AddTargetingFlag(TARGETING_TYPE_1);
    }
    if (!ParentThing->HasTC(0x42)) {
        CCharString name("CTCEntityEvents", -1);
        ParentThing->AddTC(name, false, 0);
    }
    if (EntranceConnectedToUID != 0)
        SetupEntranceConnectedTo();
    if (CameraTrackUID != 0)
        PCameraTrack = CIntelligentPointer<CThing>(GetThingManager()->GetThingWithUID(CameraTrackUID));
}
