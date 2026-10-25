// CTCAssassinRush::IsValidTeleportPos(const C3DVector& from, const C3DVector& to, const CWorldMap& map, const CTCPhysicsBase* physics, float radius) -- a teleport target is valid when its map is loaded, its cell is passable and no physical thing in the radius box around it would be hit moving from `from` to `to`
// Grade: DIFFER(350v346) -- register allocation only: retail keeps `to` in ebx for the whole body and shares ebp between `this` and the list iterator (so the map vptr is re-read after the first virtual call and `physics` comes from its arg slot in the loop); VC7.1 here caches the map vptr in ebx, spills `this`, moves `to` from esi to its slot and hoists `physics` into ebx
// Leaf: ?IsValidTeleportPos@CTCAssassinRush@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AA880, length 346 bytes (thiscall, `ret 14h`).
// Identity: Ego_r.pdb ?IsValidTeleportPos@CTCAssassinRush@@AAE_NABVC3DVector@@0ABVCWorldMap@@PBVCTCPhysicsBase@@M@Z
//   (callers 0x007AAAF5 / 0x007AABE9 in CTCAssassinRush::GetValidTeleportPos). The catalog label
//   "CTCVillage_FindValidTeleportPos" is descriptive.
// Callees: CWorldMap vtable slot 16 GetMapNumberAt(const C3DVector&) / slot 10 IsMapLoaded(long), CWorldMap::IsPosPassable(
//   const C2DCoordI&) const 0x004FCBD0, __ftol2 0x00BFEA70 (x, y, radius), malloc 0x00BFEA0E (the list header),
//   CTCBase::GetThingSearchTools 0x00686D90, CThingSearchTools::GetAllThingsInRoughArea<CThingFilter_All> 0x0079BF30 (into a
//   list<CThing*>), const LowerBound 0x004365B0 (interface 2, CTCPhysicsBase), CTCPhysicsBase vtable slot 90
//   HasCollidedWithThingFromTo(const CTCPhysicsBase&, const C3DVector&, const C3DVector&, CPhysicsCollideResponse&) const,
//   list<CThing*>::~list 0x004AB490 (out of line). Both list-taking callees carry same-TU noinline stand-in bodies so the
//   list local stays uncaptured (no dead zero store before the header malloc; the header cached in esi), as in retail.
// Layout: CThing TC bitset +0x20 (interface 2 = bit 2), interface map +0x44; CPhysicsCollideResponse (Ego_r.pdb, 28 bytes)
//   {CollidePos, CollideNormal, Friction +0x18 = 1.0f by its ctor}.
// Shape: `if (!map.IsMapLoaded(map.GetMapNumberAt(to)) || !map.IsPosPassable(C2DCoordI(to))) return false; hit = false;
//   x, y, r = (long)to.X, (long)to.Y, (long)radius; list<CThing*> things; GetThingSearchTools()->GetAllThingsInRoughArea(
//   C2DBoxI(x - r, y - r, x + r, y + r), CThingFilter_All(), things); for (it) { thing = *it; if (thing has TC 2) {
//   CPhysicsCollideResponse response; if (physics->HasCollidedWithThingFromTo(*GetTC(2), from, to, response)) { hit =
//   true; break; } } } return !hit;`
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
};

class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CPhysicsCollideResponse {                         // Ego_r.pdb, 28 bytes
public:
    C3DVector CollidePos;                               // +0x00
    C3DVector CollideNormal;                            // +0x0C
    float     Friction;                                 // +0x18
    CPhysicsCollideResponse() : Friction(1.0f) { }
};

class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline const T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual bool HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                            CPhysicsCollideResponse& response) const;                // slot 90
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                                   // slot 10
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;                    // slot 16
    bool IsPosPassable(const C2DCoordI& pos) const;                             // 0x004FCBD0
};

class CThingFilter_All {
public:
    CThingFilter_All() { }
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { _M_incr(); return *this; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _M_node._M_data = _M_node.allocate(1);
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    ~list();                                            // 0x004AB490 (same-TU stand-in below)
    iterator begin() { return iterator((_Node*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline) list<_Tp, _Alloc>::~list() { }   // 0x004AB490 (not graded)
#pragma auto_inline(on)

}

class CThingSearchTools {
public:
    template <class Filter>
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, std::list<CThing*>& things) const;   // 0x0079BF30
};

bool FindThingsInArea(const C2DBoxI& area);

// Same-TU stand-in (not graded): with the template body in the TU the list local stays uncaptured, as in retail.
#pragma auto_inline(off)
template <class Filter>
__declspec(noinline) bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter,
                                                                     std::list<CThing*>& things) const
{
    return FindThingsInArea(area);
}
#pragma auto_inline(on)

class CTCAssassinRush {
public:
    virtual void v00();
    CThing* PThing;                                     // +0x04 (CTCBase)
    const CThingSearchTools* GetThingSearchTools() const;                       // 0x00686D90 (CTCBase)
private:
    bool IsValidTeleportPos(const C3DVector& from, const C3DVector& to, const CWorldMap& map,
                            const CTCPhysicsBase* physics, float radius);
};

bool CTCAssassinRush::IsValidTeleportPos(const C3DVector& from, const C3DVector& to, const CWorldMap& map,
                                         const CTCPhysicsBase* physics, float radius)
{
    if (!map.IsMapLoaded(map.GetMapNumberAt(to)) || !map.IsPosPassable(C2DCoordI((long)to.X, (long)to.Y)))
        return false;
    bool hit = false;
    long x = (long)to.X;
    long y = (long)to.Y;
    long r = (long)radius;
    std::list<CThing*> things;
    GetThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - r, y - r, x + r, y + r), CThingFilter_All(), things);
    for (std::list<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        const CThing* thing = *it;
        if (thing->HasTC(2)) {
            CPhysicsCollideResponse response;
            if (physics->HasCollidedWithThingFromTo(*thing->GetTC<CTCPhysicsBase>(2), from, to, response)) {
                hit = true;
                break;
            }
        }
    }
    return !hit;
}
