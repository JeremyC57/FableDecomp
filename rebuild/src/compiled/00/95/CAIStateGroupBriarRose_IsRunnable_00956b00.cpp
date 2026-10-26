// CAIStateGroup_BriarRose::IsRunnable() -- Briar Rose attacks a live enemy when her boss phase is ready to change, otherwise only once NextRunFrame has passed; remember the enemy in PEnemy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956B00, 242 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_BriarRose@@MAE_NXZ.
// Callees: CVectorMap::LowerBound 0x004365B0 (const; inlined GetInterface types 8 and 0xad), CTCBoss::IsReadyForPhaseChange
// 0x007AEE90, the brain world accessor 0x008FCFC0, the static frame counter 0x0049D870, the enemy source's virtual slot 29
// (+0x74), the converting CIntelligentPointer<CThingCreatureBase> temporary (vtable 0x0123E7D8: ctor 0x00A01B10,
// SetPItem 0x00A01B90 twice, GetPItem 0x00A01B50, dtor 0x00A01C10).
// Shape: the landed AI state-group model of 0x00937F70; the interface-8 lookup is an out-parameter GetInterface whose
// result is ignored (a miss leaves the pointer unwritten: retail reloads it from the key's frame word), the boss test
// `!(GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())` guards the frame check (signed `jl`).
// Layout: engine/CAIStateGroup_BriarRose.h (PEnemy +0x14, NextRunFrame +0x1c).
// Leaf: IsRunnable@CAIStateGroup_BriarRose
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCEnemySource {                         // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

class CTCBoss { public:
    bool IsReadyForPhaseChange();               // 0x007AEE90
};

struct CWorldFrame {
    static long GetWorldFrame();                // 0x0049D870 (static: GetPWorld() is evaluated for nothing)
};

struct CAIStateGroup_BriarRose : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14 CIntelligentPointer<CThingCreatureBase>
    long NextRunFrame;                          // +0x1c
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual bool IsRunnable();
};

bool CAIStateGroup_BriarRose::IsRunnable()
{
    CThing* creature = PBrain->PCreature;
    CTCEnemySource* source;
    creature->GetInterface(&source, 8);
    CTCBoss* boss;
    if (!(creature->GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())) {
        if (((CWorldFrame*)GetPWorld())->GetWorldFrame() < NextRunFrame)
            return false;
    }
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        PEnemy = enemy;
        return true;
    }
    return false;
}