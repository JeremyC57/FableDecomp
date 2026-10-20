// CCreatureAction_PickUpThingAtTable::PerformAction() -- when the thing and the creature are alive: put the thing in the creature's carry slot named by its carryable def, then add the action's creature mode (if any)
// Leaf: ?PerformAction@CCreatureAction_PickUpThingAtTable
// Grade: DIFFER(318v302) -- VC7.1 CSEs the constant 1 of the two dead-bit tests and the `true` argument into ebx, so `this` is spilled to an 8-byte frame (retail: this in ebx, 4-byte frame, immediates); same recurring gap as 008559e0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00849F70, 302 bytes (304 with padding), virtual PerformAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThing +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24),
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::PutThingInCarrySlot 0x006A1F80,
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10.
// Layout: retail CCreatureActionBase is 0xa8 bytes: PThing +0xa8, the mode to add +0xb0; CThing +0x70 the def, +0x91 dead
//   bit; CCarryableDef +0x28 the carry slot.
// Shape: the PickUpGoods 0x00848730 model (unchecked component lookups, CDefPointer local, GetSubDef and LowerBound same-TU
//   stand-ins for retail's LTCG knowledge) behind two IS_THING_ALIVE tests; the thing is re-read through GetPItem for
//   each use; the sub-def fetch goes through an inline CThing helper so the thing's GetPItem runs before the out
//   argument is pushed.
struct CThing;
struct CThingObject;
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
    long CarrySlot;                                   // +0x28
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
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    void GetCarryableDef(CDefPointer<CCarryableDef>* out) { m_pDef->GetSubDef(out); }
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
};
struct CTCCarrying {
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCCreatureModeManager {
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureAction_PickUpThingAtTable : CCreatureActionBase {
    CBaseIntelligentPointer PThing;                   // +0xa8
    unsigned long m_mode;                             // +0xb0
    void PerformAction();
};

void CCreatureAction_PickUpThingAtTable::PerformAction()
{
    if (IS_THING_ALIVE(PThing) && IS_THING_ALIVE(PCreature)) {
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        CDefPointer<CCarryableDef> carryDef;
        PThing.GetPItem()->GetCarryableDef(&carryDef);
        carrying->PutThingInCarrySlot((CThingObject*)PThing.GetPItem(), carryDef->CarrySlot, true);
        if (m_mode) {
            CTCCreatureModeManager* modes;
            PCreature.GetPItem()->GetInterface(&modes, 0x31);
            modes->AddMode(m_mode);
        }
    }
}
