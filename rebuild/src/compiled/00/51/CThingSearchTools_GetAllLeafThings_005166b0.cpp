// CThingSearchTools::GetAllLeafThings(const CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>& filter, std::list<const CThingPhysical*>& things) const -- (<CThingVillage>) every scan-visible CThingVillage in the active quest (or none) overlapping the area, appended to the list
// Leaf: ??$GetAllLeafThings@VCThingVillage@@V?$list@PBVCThingPhysical@@V?$allocator@PBVCThingPhysical@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@@CThingSearchTools@@QBEXABV?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@AAV?$list@PBVCThingPhysical@@V?$allocator@PBVCThingPhysical@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005166B0, 370 bytes, thiscall, `ret 8`, ebp frame.
// Identity (proven): PeekTypeList(4) is CThingVillage's type list (ThingListType 4, as in the landed CThingSearchTools rows);
//   Ego_r.pdb/FableWin.pdb ??$GetAllLeafThings@VCThingVillage@@V?$list@PBVCThingPhysical@@...@CThingSearchTools@@QBEX...@Z
//   (the three Bind2<IsInQuestOrNullQuest, OverlappingArea> list instantiations Building / Object / Village); called from
//   CWorldMap::GetMapNavigationAreaInit 0x0050A650 (call at 0x0050A90B). The manifest's "GetAllLeafThings<CThingAICreature,
//   ...>" is propagated.
// Callees: CThingSearchTools::PeekTypeList(long) 0x0049C780, CThingFilter_IsInQuestOrNullQuest::operator()(const CThing&) const
//   0x006A7C60, CTCPhysicsBase::GetBoundingInfo(C3DVector&, float&) const 0x006AF640, __ftol2 0x00BFEA70 (six times), malloc
//   0x00BFEA0E (the list node).
// Shape (family M111, representative 0x005163B0): the landed 0x006AE830 loop (end() re-read every pass, the inline
//   IsIncludedInThingScans test, push_back as insert(end(), x) with placement new's null test on the node's value) with the
//   M104 filter body inline (CThingFilter_OverlappingArea as in the landed 0x0050EF30).
// Header debt: engine/CThingSearchTools.h declares no member functions, so the class is modelled locally with its Ego_r.pdb
//   members (retail STLport 12-byte AreaMaps vector) to carry the member template's real name.
// Retail-asm: 0x00516748..0x00516762 `fld [ebp-18h] ; fadd [ebp-14h] ; fistp dword ptr [ebp-0Ch] ; fild [ebp-0Ch] ; fsub [ebp-10h] ; fstp [ebp-1Ch] ; mov eax,[ebp-18h] ; cmp eax,[ebp-1Ch] ; jne ; dec dword ptr [ebp-0Ch]` -- the engine's inline-assembly ceiling helper expanded in place (a bare dword fistp, the bit compare and the memory decrement in one 10-instruction block; VC7.1 emits no bare dword fistp from C++), as in the landed 0x0050EF30.
#pragma optimize("y", off)
#include <stdlib.h>
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class C3DVector { public: float X, Y, Z; };
class C2DBoxI {
public:
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) { }
    void Grow(float amount)
    {
        X1 = (long)(X1 + amount);
        X0 = (long)(X0 - amount);
        Y0 = (long)(Y0 - amount);
        Y1 = (long)(Y1 + amount);
    }
};
class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};
// ---- CThing (retail layout): PhysicsTC +0x60, the state bit byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08,
//      InLimbo 0x20 (the landed CThingSearchTools rows).
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                            // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char StateBits;                              // +0x91
    bool IsIncludedInThingScans() const                  // out of line at 0x0048BC40, inlined here
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingVillage : public CThingPhysical { public: enum { ThingListType = 4 }; };
__forceinline long CeilToLong(float value)
{
    long result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
class CCharString { public: void* PStringData; };
class CThingFilter_IsInQuestOrNullQuest {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7C60
    CCharString QuestSectionName;
};
class CThingFilter_OverlappingArea {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        if (thing.PhysicsTC) {
            float radius;
            C3DVector centre;
            thing.PhysicsTC->GetBoundingInfo(centre, radius);
            radius = (float)CeilToLong(radius);
            C2DBoxI box = *Area;
            box.Grow(radius);
            long x = (long)centre.X;
            long y = (long)centre.Y;
            return x >= box.X0 && x < box.X1 && y >= box.Y0 && y < box.Y1;
        }
        return false;
    }
    C2DBoxI* Area;                                        // +0x0
};
template <class TF1, class TF2> class CFilter_Bind2 {     // Ego_r.pdb: F1, F2 (pointers)
public:
    template <class T> __forceinline bool operator()(const T& thing) const { return (*F1)(thing) && (*F2)(thing); }
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
};
// The thing manager's per-type lists: STLport list<CThing*> (node = next, prev, value).
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    iterator end() { return iterator(_M_node); }
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    _List_node_base* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
class CWorldMap;
class CThingManager;
namespace NThingMap { class CThingMap; }
struct CThingMapManager;
class CThingSearchTools {                                 // Ego_r.pdb members (retail 0x18 bytes)
public:
    CThingList* PeekTypeList(long type) const;            // 0x0049C780
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& things) const;
    CWorldMap* WorldMap;                                  // +0x00
    CThingMapManager* PThingMapManager;                   // +0x04
    CThingManager* ThingManager;                          // +0x08
    std::vector<NThingMap::CThingMap*> AreaMaps;          // +0x0c
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) const
{
    CThingList* list = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = list->begin(); it != list->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            things.push_back((TThing*)thing);
    }
}
template void CThingSearchTools::GetAllLeafThings<CThingVillage, std::list<const CThingPhysical*>, CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea> >(
    const CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>&, std::list<const CThingPhysical*>&) const;