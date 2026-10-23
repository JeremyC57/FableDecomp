// CTCDegradableBase::OnInitialActivate() -- persist the parent when the def is degradable, re-evaluate a damaged parent's stage, and tell a switchable-navigation component whether the thing initially blocks navigation
// Leaf: OnInitialActivate
//
// Retail 0x008ECBE0, 151 bytes (plain `ret`; vtable 0x0126A6A4 slot 6, Ego_r.pdb
// `?OnInitialActivate@CTCDegradableBase@@MAEXXZ`; the catalog "CTCDegradableBarrier::vfunc_6" is the
// slot name). Callees: CThing::SetAsGameStatePersisted(bool) @ 0x004C9C10, CTCDegradableBase::
// OnParentHealthChanged(bool) @ 0x008ECAC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @
// 0x0040F020 (inlined GetInterface for interface 0x79, bit 25 of +0x2c), the switchable-navigation
// component's SetAsBlockingNavigation @ 0x00787580 (manifest "CTCSwitchableNavigation::InitialActivate"
// is a neighbour's name); the def's virtual slot 1 when the copied def pointer drops its last reference.
// Layout = Ego_r.pdb CTCDegradableBase (PDefinition +0x10, PParentThingPhysical +0x18); CDegradableDef
// Degradable +0x25, InitiallyBlocksNavigation +0x2c; retail CThingPhysical MaxHealth +0xb0 / Health
// +0xb4 (PDB +0xc8 / +0xcc: the containers before them are smaller in retail).
// Shape: the def comes from the inline by-value GetDefinition() (add-ref with a null test, release
// without one, Degradable parked in bl across the release); `Health < MaxHealth` gives `test ah,5 ; jp`;
// the navigation flag is read through the member pointer directly (no add-ref); a bool argument loaded from
// memory is pushed as `xor ecx,ecx ; mov cl` (a long parameter gives movzx); the component comes from the out-parameter GetInterface (`mov eax,[eax+4]` ... `mov ecx,eax`).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetAsGameStatePersisted_004c9c10(bool persisted);           // 0x004C9C10
};

struct CThingPhysical {
    unsigned char _pad0[0xb0];
    float MaxHealth;                                        // +0xb0
    float Health;                                           // +0xb4
};

struct CDegradableDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x25 - 0x08];
    bool          Degradable;                               // +0x25
    unsigned char _pad26[0x2c - 0x26];
    bool          InitiallyBlocksNavigation;                // +0x2c
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

struct CTCSwitchableNavigation {
    void SetAsBlockingNavigation(bool blocking);            // 0x00787580
};

struct CTCDegradableBase {
    virtual void v00();
    CThing*                     ParentThing;                // +0x04 (CTCBase)
    bool                        ToBeRemoved;                // +0x08
    bool                        IsInGlobalUpdateTCs;        // +0x09
    long                        LastWorldFrameHit;          // +0x0c
    CDefPointer<CDegradableDef> PDefinition;                // +0x10 CDefPointer<const CDegradableDef>
    void*                       PCurrentDegradableInfo;     // +0x14
    CThingPhysical*             PParentThingPhysical;       // +0x18
    CDefPointer<CDegradableDef> GetDefinition() const { return PDefinition; }
    void OnParentHealthChanged(bool fromHit);               // 0x008ECAC0
    void OnInitialActivate();
};

void CTCDegradableBase::OnInitialActivate()
{
    if (GetDefinition()->Degradable)
        ParentThing->SetAsGameStatePersisted_004c9c10(true);
    if (PParentThingPhysical->Health < PParentThingPhysical->MaxHealth)
        OnParentHealthChanged(true);
    CTCSwitchableNavigation* navigation;
    if (ParentThing->GetInterface(&navigation, 0x79))
        navigation->SetAsBlockingNavigation(PDefinition->InitiallyBlocksNavigation);
}