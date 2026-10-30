// CTCSummonSpell::IsValidTeleportPos(const C3DVector& pos, CWorldMap* map, CThing* creature) -- a physical creature may teleport to pos when the map region there is valid and passable and no other physical thing in the surrounding square (twice the creature's radius, at least 1) would collide with it there
// Grade: DIFFER(486v508): one shared trailing return false (this now spilled, 497->505); the collision test's position is creature->GetPosition() (retail loads ecx from the creature arg slot [esp+68h]; the base called thing->GetPosition, a semantic error); the slot-90 4th argument points at a 0x1c-byte local whose +0x18 is set to 1.0f (unproven struct SCollideParams); residue: frame 0x4c v 0x48, ebx/ebp pushed at entry where retail pushes ebx/esi/edi and ebp after the passable test
// Retail: Fable.exe @ 0x0079A890, 512 bytes (`ret 0xc`).
// Callees: LowerBound 0x004365B0 (const), CWorldMap virtual slots 16 (+0x40) / 10 (+0x28), __ftol2 0x00BFEA70,
//   CWorldMap::IsPosPassable 0x004FCBD0, malloc 0x00BFEA0E (list header), CTCBase::GetThingSearchTools 0x00686D90,
//   CThingSearchTools::GetAllThingsInRoughArea<IsNot> 0x0079BF30, CThing::GetPosition 0x004C73D0, physics virtual slot 90
//   (+0x168, collision test), ~list<CThing*> 0x004AB490.
// Layout (local model): CThing TC bitset +0x20 (2 = +0x20 bit 2), interface map +0x44; the physics TC's radius +0x34.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);          // 0x00BFEA0E
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct C2DCoordI { long X, Y; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsRegionValid(long region);           // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetRegionAtPos(const C3DVector& pos); // slot 16 (+0x40)
    bool IsPosPassable(const C2DCoordI& pos);           // 0x004FCBD0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    const SEntry* LowerBound(const long& key) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long TCBits[9];                            // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct SCollideParams {
    unsigned char _pad00[0x18];
    float Scale;                                        // +0x18
    SCollideParams(float scale) : Scale(scale) {}
};
struct CTCPhysics {
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
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual bool WouldCollide(CTCPhysics* other, const C3DVector& otherPos, const C3DVector& pos, const SCollideParams& params);   // slot 90 (+0x168)
    unsigned char _pad04[0x30];
    float Radius;                                       // +0x34
};
struct CThingListNode { CThingListNode* _M_next; CThingListNode* _M_prev; CThing* _M_data; };
struct CThingList {
    CThingListNode* _M_node;
    CThingList() { _M_node = (CThingListNode*)malloc(sizeof(CThingListNode)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }
    ~CThingList();                                      // 0x004AB490
};
struct CThingSearchTools {
    bool GetAllThingsInRoughArea(const C2DBoxI& area, CThing* const& exclude, CThingList& out);   // 0x0079BF30
};
struct CTCSummonSpell {
    void* __vftable;
    CThingSearchTools* GetThingSearchTools() const;     // 0x00686D90
    bool IsValidTeleportPos(const C3DVector& pos, CWorldMap* map, CThing* creature);
};

bool CTCSummonSpell::IsValidTeleportPos(const C3DVector& pos, CWorldMap* map, CThing* creature)
{
    CTCPhysics* physics;
    if (creature->GetInterface(2, &physics) && map->IsRegionValid(map->GetRegionAtPos(pos))) {
        C2DCoordI coord;
        coord.X = (long)pos.X;
        coord.Y = (long)pos.Y;
        if (map->IsPosPassable(coord)) {
            bool blocked = false;
            float radius = physics->Radius + physics->Radius;
            if (radius < 1.0f)
                radius = 1.0f;
            long x = (long)pos.X;
            long y = (long)pos.Y;
            long r = (long)radius;
            C2DBoxI area(x - r, y - r, x + r, y + r);
            CThingList things;
            GetThingSearchTools()->GetAllThingsInRoughArea(area, creature, things);
            for (CThingListNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next)
            {
                CThing* thing = node->_M_data;
                CTCPhysics* other;
                if (thing != creature && thing->GetInterface(2, &other))
                {
                    C3DVector raised(pos.X, pos.Y, pos.Z + 0.1f);
                    if (physics->WouldCollide(other, creature->GetPosition(), raised, SCollideParams(1.0f)))
                    {
                        blocked = true;
                        break;
                    }
                }
            }
            return !blocked;
        }
    }
    return false;
}
