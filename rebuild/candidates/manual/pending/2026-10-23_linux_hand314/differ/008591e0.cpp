// CCombatActionBase::PerformPossibleDamageAction(float damage) -- collect every game object near the attacker that the strike could reach (all things on a small region map, else a rough area of 1.2 x (2 x (own radius + 1) + target radius)), then sweep the weapon trail's segments (extended into the creature's axis cylinder for melee weapons) against each one, dispatching creature / prone-thing / shot / object strike events
// Grade: DIFFER(1091v1146) -- 65 blocks: first pass; frame 0x64 v 0x6c (retail keeps a second copy of the list head and the region pointer in stack slots), the counted flag AddRef is a load/inc/store instead of inc [mem], and slot/register choices for the filter objects; flow, every call, the search choice, rough-area maths and the strike-event dispatch loop match
// Leaf: PerformPossibleDamageAction
// Retail 0x008591E0, 1144 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; malloc 0x00BFEA0E / free 0x00BFEA14; CCreatureActionBase::
//   GetWorldMap 0x00692C10; CWorldMap::GetCurrentRegionIndex 0x004FB150 / GetRegion 0x004FB490; the thing search tools
//   getter 0x00692990, GetAllThings 0x0085B5B0 / rough-area search 0x0085B2A0; CThingCreatureBase::GetPhysicsPosition
//   0x006A4D00; __ftol2 0x00BFEA70; CThing::GetPosition 0x004C73D0; CTCWeaponTrail::GetLastWeaponTrailPoints 0x0076E5F0;
//   CTCCarrying::GetThingInPrimarySlot 0x006A1440; CTCWeapon::IsMeleeWeapon 0x005DAA00;
//   ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis 0x00856310 (`__fastcall`); ProcessPossible{Creature,
//   ProneThings,Shot,Object}StrikeEvent 0x00858130 / 0x00858400 / 0x00858590 / 0x008587B0; thing-list dtor 0x006FE660.
// Layout: from this function's use -- CCombatActionBase {owner +0x08, target +0xa8, vslot 0x2b weapon-trail getter, vslot
//   0x4d IsProne}; CThing {interface bits +0x20, flags +0x2c / +0x6c, map +0x44, graphic +0x60 (radius +0x34), dead
//   +0x91}; the region's counted all-things flag object +0x24 (count +4, release slot 1, flag +0x91).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct CWeaponTrailPoint { C3DVector Start; C3DVector End; };   // pair<C3DVector, C3DVector>
template <class T> struct CArray {                     // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};
struct CThingGameObject;
struct SListNode {
    SListNode*        _M_next;
    SListNode*        _M_prev;
    CThingGameObject* _M_data;
};
struct CThingList {                                    // std::list<CThingGameObject*>
    SListNode* _M_node;
    CThingList()
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList();                                     // 0x006FE660
};
struct CThingGraphic {
    unsigned char _pad00[0x34];
    float         Radius;                              // +0x34
};
struct CTCBase {
    virtual void v00();
};
struct CThing;
struct CTCCarrying : CTCBase {
    CThing* GetThingInPrimarySlot();                   // 0x006A1440
};
struct CTCWeapon : CTCBase {
    bool IsMeleeWeapon();                              // 0x005DAA00
};
struct CTCWeaponTrail {
    bool GetLastWeaponTrailPoints(CArray<CWeaponTrailPoint>* points, long count);   // 0x0076E5F0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CThingGraphic* Graphic;                            // +0x60
    unsigned char  _pad64[0x6c - 0x64];
    unsigned long  Flags6c;                            // +0x6c
    unsigned char  _pad70[0x91 - 0x70];
    unsigned char  Flags91;                            // +0x91
    const C3DVector& GetPosition() const;              // 0x004C73D0
    const float* GetPhysicsPosition();                 // 0x006A4D00
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingGameObject : CThing { };
void __fastcall ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(CArray<CWeaponTrailPoint>* points,
                                                                             CThing* creature);   // 0x00856310
struct SCombatOverlappingSphere { C3DVector Centre; float Radius; };
struct SCombatCanCollide { CThingGraphic* Graphic; bool IgnoreFlag; bool NotProne; };
struct CStrikeFilter {                                 // CFilter_Bind4<IsNot, CombatIsStrikeToStrike, OverlappingSphere, CanCollide>
    CThing* const*            IsNot;
    CThing* const*            StrikeToStrike;
    SCombatOverlappingSphere* Sphere;
    SCombatCanCollide*        CanCollide;
};
struct SRoughArea { long MinX, MinY, MaxX, MaxY; };
struct CThingSearchTools {
    void GetAllThings(const CStrikeFilter& filter, CThingList* things);   // 0x0085B5B0
    void GetAllThingsInRoughArea(const SRoughArea& area, const CStrikeFilter& filter, CThingList* things);   // 0x0085B2A0
};
struct CRegionThingsFlag {
    virtual void v00();
    virtual void Release();                            // slot 1
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x91 - 0x08];
    bool          SearchAllThings;                     // +0x91
};
struct CRegion {
    unsigned char      _pad00[0x24];
    CRegionThingsFlag* ThingsFlag;                     // +0x24
};
struct CWorldMap {
    long GetCurrentRegionIndex();                      // 0x004FB150
    CRegion* GetRegion(long index);                    // 0x004FB490
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* Data;
    CThing* GetPItem() const;                          // 0x00A01B50
};
class CCombatActionBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0a();
    virtual void v0b();
    virtual void v0c();
    virtual void v0d();
    virtual void v0e();
    virtual void v0f();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1a();
    virtual void v1b();
    virtual void v1c();
    virtual void v1d();
    virtual void v1e();
    virtual void v1f();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2a();
    virtual CTCWeaponTrail* GetWeaponTrail(long* count);
    virtual void v2c();
    virtual void v2d();
    virtual void v2e();
    virtual void v2f();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v3a();
    virtual void v3b();
    virtual void v3c();
    virtual void v3d();
    virtual void v3e();
    virtual void v3f();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v4a();
    virtual void v4b();
    virtual void v4c();
    virtual bool IsProne();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer Target;                    // +0xa8
    CWorldMap* GetWorldMap() const;                    // 0x00692C10
    CThingSearchTools* GetSearchTools() const;         // 0x00692990
    void ProcessPossibleCreatureStrikeEvent(float damage, const CWeaponTrailPoint* from, const CWeaponTrailPoint* to,
                                            CThingGameObject* thing);   // 0x00858130
    void ProcessPossibleProneThingsStrikeEvent(float damage, const CWeaponTrailPoint* from, const CWeaponTrailPoint* to,
                                               CThingGameObject* thing);   // 0x00858400
    void ProcessPossibleShotStrikeEvent(float damage, const CWeaponTrailPoint* from, const CWeaponTrailPoint* to,
                                        CThingGameObject* thing);   // 0x00858590
    void ProcessPossibleObjectStrikeEvent(float damage, const CWeaponTrailPoint* from, const CWeaponTrailPoint* to,
                                          CThingGameObject* thing);   // 0x008587B0
    void PerformPossibleDamageAction(float damage);
};

void CCombatActionBase::PerformPossibleDamageAction(float damage)
{
    if (!Owner.GetPItem() || (Owner.GetPItem()->Flags91 & 1))
        return;
    CThing* owner = Owner.GetPItem();
    CArray<CWeaponTrailPoint> points;
    CThingList things;
    float targetRadius;
    if (Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1))
        targetRadius = Target.GetPItem()->Graphic->Radius;
    else
        targetRadius = 0.0f;
    float ownerRadius = Owner.GetPItem()->Graphic->Radius;
    float searchRadius = ((ownerRadius + 1.0f) + (ownerRadius + 1.0f) + targetRadius) * 1.2f;
    CRegion* region = GetWorldMap()->GetRegion(GetWorldMap()->GetCurrentRegionIndex());
    bool prone = false;
    if (IsProne())
        prone = true;
    CThing* self = Owner.GetPItem();
    CThing* striker = Owner.GetPItem();
    SCombatOverlappingSphere sphere;
    sphere.Centre = owner->GetPosition();
    sphere.Radius = (ownerRadius + targetRadius + 5.0f) * 1.25f;
    SCombatCanCollide canCollide;
    canCollide.Graphic = Owner.GetPItem()->Graphic;
    canCollide.NotProne = !prone;
    canCollide.IgnoreFlag = false;
    CStrikeFilter filter;
    filter.IsNot = &self;
    filter.StrikeToStrike = &striker;
    filter.Sphere = &sphere;
    filter.CanCollide = &canCollide;
    CRegionThingsFlag* flag = region->ThingsFlag;
    if (flag)
        ++flag->RefCount;
    if (flag && flag->SearchAllThings) {
        GetSearchTools()->GetAllThings(filter, &things);
    } else {
        const float* position = Owner.GetPItem()->GetPhysicsPosition();
        long x = (long)position[0];
        long y = (long)position[1];
        long r = (long)searchRadius;
        SRoughArea area;
        area.MinX = x - r;
        area.MinY = y - r;
        area.MaxX = x + r;
        area.MaxY = y + r;
        GetSearchTools()->GetAllThingsInRoughArea(area, filter, &things);
    }
    if (things._M_node->_M_next != things._M_node) {
        long count;
        CTCWeaponTrail* trail = GetWeaponTrail(&count);
        if (trail) {
            trail->GetLastWeaponTrailPoints(&points, count);
            CTCCarrying* carrying;
            if (Owner.GetPItem()->GetInterface(&carrying, 0x46)) {
                CThing* weaponThing = carrying->GetThingInPrimarySlot();
                CTCWeapon* weapon;
                if (weaponThing && !(weaponThing->Flags91 & 1) && weaponThing->GetInterface(&weapon, 0x48) &&
                    weapon->IsMeleeWeapon())
                    ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(&points, Owner.GetPItem());
            }
            for (SListNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
                CThingGameObject* thing = node->_M_data;
                for (unsigned long i = 1; i < points.size(); ++i) {
                    unsigned long flags = thing->Flags6c;
                    if (flags & 8)
                        ProcessPossibleCreatureStrikeEvent(damage, &points[i - 1], &points[i], thing);
                    else if (IsProne())
                        ProcessPossibleProneThingsStrikeEvent(damage, &points[i - 1], &points[i], thing);
                    else if (!(flags & 0x80)) {
                        if (thing->m_presentBits[3] & 0x800000)
                            ProcessPossibleShotStrikeEvent(damage, &points[i - 1], &points[i], thing);
                        else
                            ProcessPossibleObjectStrikeEvent(damage, &points[i - 1], &points[i], thing);
                    }
                }
            }
        }
    }
    if (flag && --flag->RefCount == 0)
        flag->Release();
}
