// CAIStateGroup_IceTrollIcicleAttack::IsRunnable() -- runnable against the creature's live enemy when the creature def has no positive will-power-indicator range, or the enemy is within that range on the XY plane; latch the enemy as the target
// Grade: DIFFER(ours v retail) -- this in edi with a frame spill vs retail ebx; retail keeps 0 in edi (entry zero store into the shared key/holder slot) and the range bool in a frame byte
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961CA0, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 8, bit 8 of +0x20 masked, one-ternary pick into ecx, component preset to null),
//          the enemy tracker's virtual slot 29 (+0x74) GetEnemy, CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
//          @ 0x008C7A70 on the creature's def (+0x70, add-ref'd copy), CThing::GetPosition @ 0x004C73D0,
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10
//          (temporary vtable 0x01238C6C), def virtual slot 1 (destroy on last release).
// Globals: 0.0f @ 0x0122DEDC.
// Shape: the range test `found && def->Range > 0` is a frame byte kept across the creature-def release; the sub-def is
//          released by hand (no null test on the out-of-range path, a null test on the success path).
// Layout: the generated engine/CAIStateGroup_IceTrollIcicleAttack.h has no member for the target at +0x14 (an
//          intelligent pointer, set through SetPItem) so the class is modelled locally on the PDB CAIStateGroupBase
//          layout (PBrain +0x04).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct C3DVector {
    float X, Y, Z;
};

struct CDefPointee {
    virtual void v00();
    virtual void Delete() const;                // slot 1 (+4)
    mutable long m_refCount;                    // +0x04
    void AddRef() const { ++m_refCount; }
    void Release() const { if (--m_refCount == 0) Delete(); }
};

struct CAICreatureWillPowerIndicatorDef : CDefPointee {
    unsigned char _pad08[0x5c - 0x08];
    float Range;                                // +0x5c (descriptive)
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CDefClassBase : CDefPointee {
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out);   // 0x008C7A70
};


void* FindDef_GetSubDef();                      // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail at 0x008C7A70): it writes the out slot only, so VC7.1 knows
// the holder's address does not escape the call (the landed 0x00962C10 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out)
{
    void* def = FindDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

// the thing's add-ref'd def copy (CDefPointer<const CDefClassBase> copy ctor / dtor)
struct CDefClassPointer {
    CDefClassBase* Object;
    CDefClassPointer(CDefClassBase* def) : Object(def) { if (Object) Object->AddRef(); }
    ~CDefClassPointer() { if (Object) Object->Release(); }
    CDefClassBase* operator->() const { return Object; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad2[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
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
};

#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CTCEnemyTracker {                        // interface 8
    V16(a) V4(b) V4(c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};

struct CAIStateGroup_IceTrollIcicleAttack : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_IceTrollIcicleAttack::IsRunnable()
{
    CTCEnemyTracker* tracker = 0;
    if (GetCreature()->GetInterfaceX(&tracker, 8)) {
        CThing* enemy = tracker->GetEnemy();
        if (enemy && !(enemy->m_flags91 & 1)) {
            CDefPointer<CAICreatureWillPowerIndicatorDef> holder;
            bool limited;
            CAICreatureWillPowerIndicatorDef* indicator;
            {
                CDefClassPointer def(GetCreature()->PDef);
                bool found = def->GetSubDef(&holder);
                indicator = holder.Object;
                limited = found && indicator->Range > 0.0f;
            }
            if (limited) {
                const C3DVector& from = GetCreature()->GetPosition();
                const C3DVector& to = enemy->GetPosition();
                float dx = to.X - from.X;
                float dy = to.Y - from.Y;
                float range = indicator->Range;
                if (dx * dx + dy * dy > range * range) {
                    indicator->Release();
                    return false;
                }
            }
            PTarget = CIntelligentPointerThing(enemy);
            if (indicator)
                indicator->Release();
            return true;
        }
    }
    return false;
}
