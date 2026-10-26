// CTCWallMount::OnInitialActivate() -- re-links the mounted trophy from its saved UID; a live trophy gets its physics switched off, its interface-0x82 component emptied and removed, and its targetability cleared
// Leaf: ?OnInitialActivate@CTCWallMount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DE5D0, 311 bytes (plain `ret`).
// Callees: CTCBase::GetThingManager() 0x00686AE0; CThingManager::GetThingWithUID(__int64) 0x0051ED80 (label "CStopWatch::
//          GetTicks" propagated); CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer<CThing> vtable 0x01238C6C),
//          SetPItem 0x00A01B90, GetPItem 0x00A01B50 (x8), ~CBaseIntelligentPointer 0x00A01C10; the physics component's
//          SetPhysicsActive(bool) (slot 36, +0x90); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020
//          (interfaces 0x82 and 9); the interface-0x82 component's entry clear 0x007F8860 (label "Unwind@00fbdab0"
//          propagated; it erases its +0x10 vector); CThing::RemoveTC(long) 0x004C9840.
// Layout (Ego_r.pdb, offsets hold): CTCWallMount Trophy +0x10, TrophyID (unsigned __int64) +0x18; CThing PhysicsTC +0x60,
//          bitset +0x20 (0x82 = bit 2 of word +0x30, 9 = bit 9 of word +0x20), interface map +0x44, dead bit +0x91;
//          CTCTargeted::Targetable +0x10.
// Shape: `Trophy = CIntelligentPointer<CThing>(thing)` through the inline operator= with a converting temporary; the
//        alive test is IS_THING_ALIVE; the physics component is read through an inline getter (straight into ecx for the
//        virtual call); every later use re-fetches the item, the two presence tests use one fetch each
//        for both the bitset and the lookup (named locals).
class CThing;
class CTCBase;
class CBaseObject;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35();
    virtual void SetPhysicsActive(bool active); // slot 36 (+0x90)
};
class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];           // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad_0x4c[0x14];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad_0x64[0x2d];
    unsigned char   m_flags91;                  // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void RemoveTC(long interfaceType);          // 0x004C9840
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
class CThingManager { public:
    CThing* GetThingWithUID(__int64 uid);       // 0x0051ED80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCComponent82 {                         // the interface-0x82 component (class unidentified)
    void ClearEntries();                        // 0x007F8860
};
class CTCTargeted { public:
    unsigned char _pad_0x00[0x10];
    bool          Targetable;                   // +0x10
};

struct CTCBaseModel {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CThingManager* GetThingManager();           // 0x00686AE0
};
class CTCWallMount : public CTCBaseModel { public:
    long                        BoughtForAmount;   // +0x0c
    CIntelligentPointer<CThing> Trophy;            // +0x10
    unsigned __int64            TrophyID;          // +0x18
    virtual void OnInitialActivate();
};

void CTCWallMount::OnInitialActivate()
{
    if (TrophyID == 0)
        return;
    Trophy = CIntelligentPointer<CThing>(GetThingManager()->GetThingWithUID(TrophyID));
    if (IS_THING_ALIVE(Trophy)) {
        if (Trophy.GetPItem()->GetPhysicsTC())
            Trophy.GetPItem()->GetPhysicsTC()->SetPhysicsActive(false);
        CThing* trophy = Trophy.GetPItem();
        if (trophy->HasTC(0x82)) {
            ((CTCComponent82*)trophy->GetTC(0x82))->ClearEntries();
            Trophy.GetPItem()->RemoveTC(0x82);
        }
        CThing* target = Trophy.GetPItem();
        if (target->HasTC(9))
            ((CTCTargeted*)target->GetTC(9))->Targetable = false;
    }
}