// Grade: DIFFER(ours v retail) -- 159v157: the component (phi of the hit `[eax+4]` and miss `[esp+8]` arms) is allocated to eax and copied (`mov ecx,eax`) before the virtual GetTarget call; retail loads both arms straight into ecx
// CAIStateGroup_MazeDefensiveMeleeCombat::IsRunnable() -- runnable when the creature's targeting component has a live target; adopt it as PTargetThing
// Leaf: ?IsRunnable@CAIStateGroup_MazeDefensiveMeleeCombat@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954980, length 157 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the const instantiation of the inlined interface lookup, type 8,
// tested as `mov ecx,[eax+20h] ; test ch,1`), the component's virtual slot 29 (+0x74, the current target),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer<CThing>.
//
// Shape: the const out-parameter GetInterface with its result ignored (the component lands straight in ecx for
// the virtual call; the miss arm reads the never-written local from the key slot), an inline `thing && !dead`
// test on the returned pointer, then `PTargetThing = CIntelligentPointer<CThing>(target)` through the stack
// temporary (whose 8-byte slot also homes the lookup key) and `return true`; the shared `xor al,al` tail last.
// The component class is not identified (interface type 8): descriptive names. The engine header
// CAIStateGroup_MazeDefensiveMeleeCombat.h is flattened data-only: local model (PDB: LastHealFrame +0x20).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];                 // +0x4c..+0x90
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CTCTargeting {                           // interface type 8 (descriptive)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    virtual bool IsRunnable();
};

struct CAIStateGroup_MazeDefensiveMeleeCombat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;   // +0x14
    long _base1c;                               // +0x1c
    long LastHealFrame;                         // +0x20
    virtual bool IsRunnable();
};

bool CAIStateGroup_MazeDefensiveMeleeCombat::IsRunnable()
{
    CTCTargeting* targeting;
    PBrain->PCreature->GetInterface(&targeting, 8);
    CThing* target = targeting->GetTarget();
    if (target && !(target->m_flags91 & 1)) {
        PTargetThing = CIntelligentPointer<CThing>(target);
        return true;
    }
    return false;
}
