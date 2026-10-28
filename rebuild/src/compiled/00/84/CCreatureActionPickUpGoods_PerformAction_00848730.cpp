// CCreatureAction_PickUpGoods::PerformAction() -- when the goods thing is alive: put it in the creature's carry slot named by its carryable def and push a new CModeCarryingGoodsStand(creature) mode
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00848730, 275 bytes (288 with padding), virtual PerformAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PGoods +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24),
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::PutThingInCarrySlot 0x006A1F80, operator new
//   0x00BFEA1A (0x74 bytes), NCreatureMode::CModeCarryingGoodsStand(CThing*) 0x0073FCE0 (landed), CCountedPointer<CModeBase>
//   (CModeBase*) 0x00718A00 (built in the argument area), CTCCreatureModeManager::AddMode 0x0073ACC0.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the goods pointer is this class's first member (+0xa8); CThing +0x70
//   the def, +0x91 dead bit; CCarryableDef +0x28 the carry slot.
// Shape: the GetPlateFromCupboard 0x00848500 model: both component lookups unchecked (the miss path reads the out
//   variable's uninitialised slot), the carryable def a CDefPointer local (DecRefToZero release, null test folded), the
//   new mode built through the owner pointer's inline `operator CThing*()`; the GetSubDef and LowerBound same-TU stand-ins
//   carry retail's LTCG knowledge (def held in ebx, key and def share one frame word).
class CThing;
class CThingObject;
class C3DVector { public: float x, y, z; };
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCarryableDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                   // +0x28
};
template<class T> class CDefPointer { public:
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
class CDefinitionManager { public:
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
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
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
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
class CThingObject : public CThing { public: };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
class CTCCarrying { public:
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                                  // +0
    void* m_pCount;                                   // +4
    CCountedPointer(T* object);                       // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeCarryingGoodsStand : CModeBase {
    unsigned char _pad[0x74];
    CModeCarryingGoodsStand(CThing* owner);           // 0x0073FCE0
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);    // 0x0073ACC0
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_PickUpGoods : public CCreatureActionBase { public:
    CBaseIntelligentPointer PGoods;                   // +0xa8
    void PerformAction();
};

void CCreatureAction_PickUpGoods::PerformAction()
{
    CThing* goods = PGoods.GetPItem();
    if (goods && !(goods->m_flags91 & 1)) {
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        CDefPointer<CCarryableDef> carryDef;
        goods->m_pDef->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot((CThingObject*)goods, carryDef->CarrySlot, true);
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->AddMode(CCountedPointer<CModeBase>(new CModeCarryingGoodsStand(PCreature)));
    }
}