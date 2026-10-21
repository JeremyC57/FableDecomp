// CCreatureAction_PutDownJugFromFilling::PerformAction() -- take the jug out of the creature's carry slot named by its carryable def (+0x30), reset the jug's physics (virtual slot 36 with 0) when it is alive, remove creature mode 0x2b and set the "filled" byte of the jug's interface-0xBB component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A4A0, length 325 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PJug +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020 (inlined
//   untested GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24; 0xBB the jug component,
//   bit 27 of +0x34), CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::RemoveThingInCarrySlot(long, bool) 0x006A16A0,
//   the jug physics component's virtual slot 36 (+0x90, one argument), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the jug pointer is this class's first member (+0xa8); CThing +0x70 the def,
//   +0x91 dead bit; CCarryableDef +0x30 the carry slot used here; the jug component's byte +0x10.
// Shape: the landed CCreatureAction_PickUpGoods 0x00848730 model: no outer alive test (only the physics reset is guarded), every lookup unchecked (the miss path
//   reads the out word), the carryable def a CDefPointer local whose release has its null test folded (the def was dereferenced),
//   the GetSubDef and LowerBound same-TU stand-ins carry retail's LTCG knowledge (def in ebx, key and def share one frame word).
struct CThing;
struct CThingObject;
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); virtual void v##n##4(); \
    virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); virtual void v##n##9();
struct CPhysicsComponent {
    V10(0) V10(1) V10(2)                              // slots 0..29
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetMovementType(long type);          // slot 36 (+0x90)
};
#undef V10
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    unsigned char _pad28[0x30 - 0x28];
    long FillCarrySlot;                               // +0x30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-ins (retail's LTCG knowledge): GetSubDef<CCarryableDef> 0x005CC10E (the landed GetSubDef body,
// 0x00485815's shape) and CVectorMap::LowerBound 0x0040F020 -- VC7.1 then knows neither keeps the address of the def
// pointer / lookup key, so the def stays in ebp and the frame words are shared like retail's.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with the AddTC name string and the by-value target copy.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsComponent* m_pPhysics;                    // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                            // +0x70
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
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
};
struct CThingObject : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CTCCarrying {
    void RemoveThingInCarrySlot(long slot, bool immediately);   // 0x006A16A0
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};
struct CTCJugComponent {                              // interface type 0xBB
    unsigned char _pad[0x10];
    unsigned char Filled;                             // +0x10
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_PutDownJugFromFilling : CCreatureActionBase {
    CBaseIntelligentPointer PJug;                     // +0xa8
    void PerformAction();
};

void CCreatureAction_PutDownJugFromFilling::PerformAction()
{
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    CDefPointer<CCarryableDef> carryDef;
    CThing* jug = PJug.GetPItem();                    // named: the GetPItem call runs before the out-pointer push
    jug->m_pDef->GetSubDef(&carryDef);
    carrying->RemoveThingInCarrySlot(carryDef->FillCarrySlot, true);
    if (PJug.GetPItem() && !(PJug.GetPItem()->m_flags91 & 1))
        PJug.GetPItem()->GetPhysics()->SetMovementType(0);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x2b);
    CTCJugComponent* jugComponent;
    PJug.GetPItem()->GetInterface(&jugComponent, 0xbb);
    jugComponent->Filled = 1;
}
