// CAIStateGroup_RangedCombatUsePreferredFirePoint::IsRunnable() -- with a ranged weapon and a live enemy, take the nearest preferred fire point and reserve it for the creature.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D590, length 512 bytes (+ int3 padding). Vtable slot 11.
// Callees: CanUseRangedCombat @ 0x0095D310, CVectorMap::LowerBound const @ 0x004365B0 (enemy source, type 8 = bit 8 of
// +0x20) and non-const @ 0x0040F020 (fire point, type 0xac = bit 12 of +0x34), the enemy source's virtual GetEnemy
// (slot 29, +0x74), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
// (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8, CIntelligentPointer<CThing> vtable 0x01238C6C),
// GetFirePointList(vector<CThing*>&) @ 0x0095D4C0 (same TU), std::sort over the points with a by-position comparator
// @ 0x0095E010 (register-passed first/last; manifest "std_sort_Introsort"), CThing::GetPosition @ 0x004C73D0, the fire
// point's occupant setter @ 0x0087C390 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated),
// free @ 0x00BFEA14 (the vector's storage).
// Layout: engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h (PEnemy +0x14, PFirePoint +0x1c, TimeOutCounter +0x24)
// over the landed local CAIStateGroupBase model (the generated header flattens the vptr). Retail STLport vector = 12 bytes.
// Shape: GetFirePointList gets a same-TU noinline stand-in whose body does not let the vector's address escape: VC then
// keeps begin/end in ebp/esi across the sort, reuses the vector's slot for later temporaries (frame = 0xc) and drops the
// null test from the success path's free. Intelligent-pointer assignment names the item (`item = other.GetPItem()`);
// the fire point lookup is the one-ternary form (component straight into ecx); the comparator is a by-value temporary
// built in its argument slot through C3DVector's memberwise copy ctor.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class C3DVector { public: float x, y, z; C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { } };
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
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
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7D8)
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
extern "C" void __cdecl free(void*);           // 0x00BFEA14
struct CThingVector {                           // std::vector<CThing*> (retail STLport: 12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CCompareDistanceTo { C3DVector m_pos; CCompareDistanceTo(const C3DVector& pos) : m_pos(pos) { } };
void __fastcall SortFirePoints(CThing** first, CThing** last, CCompareDistanceTo compare);   // 0x0095E010 (std::sort)
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CTCFirePoint {                           // interface 0xac
    void SetOccupant(CThing* creature);         // 0x0087C390
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();
    virtual void OnReplace();
    virtual bool IsRunnable();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    CIntelligentPointerThing PFirePoint;        // +0x1c
    long TimeOutCounter;                        // +0x24
    bool CanUseRangedCombat();                  // 0x0095D310
    void GetFirePointList(CThingVector& points);   // 0x0095D4C0
    virtual bool IsRunnable();
};
CThing** FirePointSink(void* self, long n);
#pragma auto_inline(off)
// Same-TU stand-in for GetFirePointList @ 0x0095D4C0 (compiled in retail's TU).
__declspec(noinline) void CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(CThingVector& points)
{
    CThing** first = FirePointSink(this, 0);
    points._M_start = first;
    points._M_finish = FirePointSink(this, 1);
    points._M_end_of_storage = points._M_finish;
}
#pragma auto_inline(on)

bool CAIStateGroup_RangedCombatUsePreferredFirePoint::IsRunnable()
{
    if (!CanUseRangedCombat())
        return false;
    CTCEnemySource* source;
    PBrain->PCreature->GetInterfaceC(&source, 8);
    PEnemy = CIntelligentPointerCreature(source->GetEnemy());
    PFirePoint = CIntelligentPointerThing(0);
    TimeOutCounter = 0;
    if (IS_THING_ALIVE(PEnemy)) {
        CThingVector points;
        GetFirePointList(points);
        SortFirePoints(points._M_start, points._M_finish, CCompareDistanceTo(PBrain->PCreature->GetPosition()));
        if (points.size() > 0) {
            PFirePoint = CIntelligentPointerThing(*points._M_start);
            CTCFirePoint* firePoint;
            PFirePoint.GetPItem()->GetInterfaceX(&firePoint, 0xac);
            firePoint->SetOccupant(PBrain->PCreature);
            return true;
        }
    }
    return false;
}