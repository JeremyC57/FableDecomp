// CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable() -- seek a live enemy underground while it is beyond half the creature's surfacing range (XY distance); true when there is nothing to seek
// Retail: Fable.exe (TLC, VC7.1) @ 0x009207A0, 308 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_CreatureSeekEnemyUnderground@@MAE_NXZ.
// Callees: CVectorMap::LowerBound 0x004365B0 (const; inlined GetInterface types 8 and 0x34), the enemy source's virtual
// slot 29 (+0x74), CThing::GetPosition 0x004C73D0 (creature first), the 0x34 component's range getter 0x007448F0, the
// converting CIntelligentPointer temporary (vtable 0x01237F1C: ctor 0x00A01B10, SetPItem 0x00A01B90 x2, GetPItem 0x00A01B50,
// dtor 0x00A01C10); 0.5f pool 0x0122F59C.
// Shape: the landed AI state-group models (0x00937F70; one-ternary GetInterface select); `sqrt(dx*dx + dy*dy)` of
// enemy - creature through an XY helper written straight into the comparison with `range * 0.5f` (a named distance
// local lets VC sink the sqrt below the range call and spill dx/dy instead).
// Leaf: IsRunnable@CAIStateGroup_CreatureSeekEnemyUnderground
#include <math.h>
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

struct CThing {
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;   // one ternary: straight into ecx
            return true;
        }
        return false;
    }
    const struct C3DVector& GetPosition() const;   // 0x004C73D0
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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

struct C3DVector { float X, Y, Z; };


struct CTCUndergroundMover {                    // the interface-0x34 component
    float GetSurfaceRange();                    // 0x007448F0
};

static inline float XYDistance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}

struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14 (vtable 0x01237F1C)
    virtual bool IsRunnable();
};

bool CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable()
{
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        CThing* creature = PBrain->PCreature;
        CTCUndergroundMover* mover;
        if (creature->GetInterface(&mover, 0x34)) {
            if (XYDistance(enemy->GetPosition(), creature->GetPosition()) > mover->GetSurfaceRange() * 0.5f) {
                PEnemy = enemy;
                return true;
            }
            return false;
        }
    }
    return true;
}