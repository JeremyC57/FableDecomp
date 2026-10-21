// CAIStateGroup_IceTrollIcicleAssault::IsRunnable() -- runnable against the targeting AI's living target when the boss is ready for a phase change, or when the target is beyond the snow troll def's ice-trail distance limit in XY; caches the target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961FB0, length 451 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 8 = CTCTargetingAI, 0xAD = CTCBoss), CTCTargetingAI virtual
// slot 29 (+0x74, the current target), CTCBoss::IsReadyForPhaseChange @ 0x007AEE90, CIntelligentPointer<CThing>(CThing*) @
// 0x004A96F0 (twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CDefClassBase::GetSubDef<CSnowTrollDef> @ 0x008C7A70, the def class's and the def's virtual slot 1 (count release),
// CThing::GetPosition @ 0x004C73D0 (twice).
// Globals: 0.0f @ 0x0122DEDC.
// Shape: the targeting lookup is the zero-preloaded out pointer (the dead zero store stays at entry); the def class is a
// by-value counted temporary released after the `&&`; GetSubDef is a same-TU auto_inline(off) stand-in (accepted precedent
// 0x0092C070 / 0x007E4B60) so the def stays in edi; operator= is a __forceinline SetPItem(other.GetPItem()).
// Layout: Ego_r.pdb CAIStateGroup_IceTrollIcicleAssault (PTarget +0x14); CSnowTrollDef::IceTrailDistanceLimit +0x5c
// (Ego_r.pdb); the creature's counted def class at +0x70. The engine header flattens the base, so the model is local.
struct C3DVector { float X, Y, Z; };
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CSnowTrollDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x54];
    float IceTrailDistanceLimit;                // +0x5c
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerSnowTroll {                   // CDefPointer<const CSnowTrollDef>
    CSnowTrollDef* m_p;
    CDefPointerSnowTroll() : m_p(0) { }
    ~CDefPointerSnowTroll() { if (m_p) m_p->Release(); }
    CSnowTrollDef* operator->() const { return m_p; }
};
struct CDefClassBase {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
    bool GetSubDef(CDefPointerSnowTroll* out) const;   // 0x008C7A70 (GetSubDef<CSnowTrollDef>)
};
struct CDefClassPointer {                       // the counted def class pointer
    CDefClassBase* m_p;
    CDefClassPointer(const CDefClassPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefClassPointer() { if (m_p) m_p->Release(); }
    CDefClassBase* operator->() const { return m_p; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x24];
    CDefClassPointer PDefClass;                 // +0x70
    unsigned char _pad74[0x1d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CDefClassPointer GetPDefClass() const { return PDefClass; }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
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

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* thing);         // 0x004A96F0
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
// Same-TU stand-in (a template member compiled in the caller's TU): GetSubDef reads the old out pointer and writes the
// new one, like CDefPointer::operator=, so the def's address does not escape (as retail's LTCG saw it).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerSnowTroll* out) const
{
    CSnowTrollDef* old = out->m_p;
    out->m_p = (CSnowTrollDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)

struct CTCTargetingAI {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetPTarget();               // slot 29 (+0x74)
};
struct CTCBoss {                                // interface 0xAD
    bool IsReadyForPhaseChange() const;         // 0x007AEE90
};

struct CAIBrain {
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
};

struct CAIStateGroup_IceTrollIcicleAssault : CAIStateGroupBase {
    CIntelligentPointer PTarget;                // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_IceTrollIcicleAssault::IsRunnable()
{
    CTCTargetingAI* targeting = 0;
    if (PBrain->PCreature->GetInterface(&targeting, 8)) {
        CThing* target = targeting->GetPTarget();
        if (target != 0 && !(target->m_flags91 & 1)) {
            CThing* creature = PBrain->PCreature;
            if (creature->HasTC(0xad) && creature->GetTC<CTCBoss>(0xad)->IsReadyForPhaseChange()) {
                PTarget = target;
                return true;
            }
            CDefPointerSnowTroll def;
            bool hasLimit = PBrain->PCreature->GetPDefClass()->GetSubDef(&def) && def->IceTrailDistanceLimit > 0.0f;
            if (hasLimit) {
                const C3DVector& me = PBrain->PCreature->GetPosition();
                const C3DVector& him = target->GetPosition();
                float dx = him.X - me.X;
                float dy = him.Y - me.Y;
                float limit = def->IceTrailDistanceLimit;
                if (dx * dx + dy * dy > limit * limit) {
                    PTarget = target;
                    return true;
                }
            }
        }
    }
    return false;
}