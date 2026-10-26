// CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector*) -- gather the valid positions within the battle def's rush-away distance (+2) of Jack, drop those closer than the rush-away distance to Jack and those within 21 of the boss phase start position, then try random candidates until one Jack can navigate to (copied to *out); returns whether one was found
// Leaf: ?GetValidRushPosition@CCombatAction_StrikeResponseJackOfBladesIntoRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C1600, 761 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), GetSubDef<CJackOfBladesBattleDef> 0x008C33E0 (same-TU
//   stand-in), CVectorMap::LowerBound 0x0040F020 (same-TU stand-in), CCreatureActionBase::GetPWorld 0x00692940, CWorld virtual
//   slot 12 (+0x30, world map), NCreatureHiding::GetValidPositions 0x0074A5D0 (__fastcall), CThing::GetPosition 0x004C73D0,
//   STLport remove_if<C3DVector*, pred> 0x008AC060 / 0x008C34A0 (__fastcall, 16-byte predicate by value), STLport __copy
//   <C3DVector> 0x0074CA20 (same-TU stand-in), CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890,
//   CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620, free 0x00BFEA14 (vector storage), def release via slot 1.
// Layout: CJackOfBladesBattleDef::RushAwayDistance retail +0x7c (PDB +0x80); CTCBoss (interface 0xAD) PhaseStartPos +0x22
//   (packed); CWorld thing-search tools +0x20; navigation is interface 7.
// Shape: the boss and navigation out pointers are never null-initialised (the absent path reads the stale argument slot, as
//   retail); same-TU LowerBound and __copy bodies put both lookup keys in one frame word and home the empty iterator-tag
//   temporaries in the dead argument slot; the near-Jack predicate is a NAMED local built from a named creature pointer and a
//   named distance (GetPItem, distance spill, GetPosition, X/Y on the x87 stack, Z through the frame), and the first erase
//   takes `positions.end()` captured right after the size test (retail's edi survives the calls); the second erase is the
//   plain expression form; `while (!found && size)` with if/else (no break) keeps retail's top-tested loop and the
//   `mov bl,1` / `mov bl,[found]` exits.
#include <stdlib.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CEmptyTag { CEmptyTag() { } };
C3DVector* __fastcall CopyPositions(const C3DVector* first, const C3DVector* last, C3DVector* dest, const CEmptyTag& tag, long* distance);   // 0x0074CA20
#pragma auto_inline(off)
__declspec(noinline) C3DVector* __fastcall CopyPositions(const C3DVector* first, const C3DVector* last, C3DVector* dest, const CEmptyTag&, long*)
{
    for (long n = last - first; n > 0; --n) {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}
#pragma auto_inline(on)
struct CPositionVector {                                // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    long size() const { return _M_finish - _M_start; }
    C3DVector& operator[](long i) { return *(begin() + i); }
    C3DVector* erase(C3DVector* first, C3DVector* last)
    {
        C3DVector* newFinish = CopyPositions(last, _M_finish, first, CEmptyTag(), 0);
        _M_finish = newFinish;
        return first;
    }
    C3DVector* erase(C3DVector* pos)
    {
        if (pos + 1 != _M_finish)
            CopyPositions(pos + 1, _M_finish, pos, CEmptyTag(), 0);
        --_M_finish;
        return pos;
    }
};
class CWorldMap;
class CThingSearchTools;
class CTCShapeManager;
void __fastcall GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                                  const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
#define VF(n) virtual void v##n();
class CWorld { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11)
    virtual const CWorldMap& GetWorldMap();             // slot 12 (+0x30)
    unsigned char      _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;               // +0x20
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long n = last - first;
    while (n > 0) {
        long half = n >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return lo;
}
#pragma auto_inline(on)
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
class CJackOfBladesBattleDef : public CDefBase { public:
    unsigned char _pad08[0x7c - 0x08];
    float         RushAwayDistance;                     // +0x7c (PDB +0x80)
};
template<class T> class CDefPointer { public:                  // released without a null test
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CJackOfBladesBattleDef>* out);   // 0x008C33E0
};
void* FindSubDef_CJackOfBladesBattleDef(CDefClassBase* defClass);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CJackOfBladesBattleDef>* out)
{
    void* def = FindSubDef_CJackOfBladesBattleDef(this);
    if (!def)
        return false;
    out->Object = (CJackOfBladesBattleDef*)def;
    return true;
}
#pragma auto_inline(on)

#pragma pack(push, 1)
class CTCBoss { public:                                        // interface 0xAD
    unsigned char _pad00[0x22];
    C3DVector     PhaseStartPos;                        // +0x22
};
#pragma pack(pop)
class CTCCreatureNavigation { public:                          // interface 7
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};
class CThing { public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x70 - 0x4c];
    CDefClassBase*  m_pDef;                             // +0x70
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
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
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};

struct CDistancePredicate {                             // remove_if predicate 0x008AC060 (16 bytes by value)
    float     Distance;
    C3DVector Pos;
    CDistancePredicate(float distance, const C3DVector& pos) : Distance(distance), Pos(pos) { }
};
struct CPhaseDistancePredicate {                        // remove_if predicate 0x008C34A0 (16 bytes by value)
    float     Distance;
    C3DVector Pos;
    CPhaseDistancePredicate(float distance, const C3DVector& pos) : Distance(distance), Pos(pos) { }
};
C3DVector* __fastcall RemoveIf(C3DVector* first, C3DVector* last, CDistancePredicate pred);        // 0x008AC060
C3DVector* __fastcall RemoveIf(C3DVector* first, C3DVector* last, CPhaseDistancePredicate pred);   // 0x008C34A0

class CCombatActionBase {
public:
    virtual void Initialise();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CWorld* GetPWorld() const;                          // 0x00692940
};
class CCombatAction_StrikeResponseJackOfBladesIntoRush : public CCombatActionBase {
public:
    bool GetValidRushPosition(C3DVector* out);
};

bool CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector* out)
{
    CDefPointer<CJackOfBladesBattleDef> def;
    PCreature.GetPItem()->GetDef()->GetSubDef(&def);
    CTCBoss* boss;
    PCreature.GetPItem()->GetInterface(&boss, 0xad);
    CPositionVector positions;
    CWorld* world = GetPWorld();
    CThing* creature = PCreature.GetPItem();
    const CThingSearchTools& search = *GetPWorld()->PThingSearchTools;
    GetValidPositions(positions, world->GetWorldMap(), search, creature->GetPosition(), def->RushAwayDistance + 2.0f, false, 0);
    if (positions.size() == 0)
        return false;
    C3DVector* last = positions.end();
    CThing* me = PCreature.GetPItem();
    float distance = def->RushAwayDistance;
    CDistancePredicate tooClose(distance, me->GetPosition());
    positions.erase(RemoveIf(positions.begin(), positions.end(), tooClose), last);
    positions.erase(RemoveIf(positions.begin(), positions.end(), CPhaseDistancePredicate(21.0f, boss->PhaseStartPos)),
                    positions.end());
    bool found = false;
    CTCCreatureNavigation* navigation;
    PCreature.GetPItem()->GetInterface(&navigation, 7);
    while (!found && positions.size() != 0) {
        unsigned long count = positions.size();
        unsigned long random = GFRandom(GetPWorld()->GetPRandomSeed());
        unsigned long index = count ? random % count : 0;
        CThing* self = PCreature.GetPItem();
        if (navigation->IsAbleToNavigateFromPosToPos(self->GetPosition(), positions[index], 0.0f)) {
            *out = positions[index];
            found = true;
        } else {
            positions.erase(positions.begin() + index);
        }
    }
    return found;
}