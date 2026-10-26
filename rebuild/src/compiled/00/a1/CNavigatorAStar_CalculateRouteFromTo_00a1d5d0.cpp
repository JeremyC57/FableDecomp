// CNavigatorAStar::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, vector<pair<C2DVector, unsigned char> >& route) -- interrupt the navigator and run the A* search until it has a complete culled route (or reports 1 = failure), then copy the route's way points (2D position and type) into the caller's vector
// Leaf: ?CalculateRouteFromTo@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1D5D0, 259 bytes (+ int3 padding), `ret 0Ch`.
// Callees: own virtuals slot 18 Interrupt (+0x48) 0x00A1A150 and slot 3 MoveToPos(const C3DVector&, const C3DVector&,
//   float, C3DVector&, bool x6, float) (+0x0c) 0x00A1FB00; the route vector's `__fastcall` _Move(first, last, dest, tag,
//   bool) 0x008001F0 (erase(begin(), end())), reserve(n) 0x00800CA0 and _M_insert_overflow(pos, x, tag, fill, atEnd)
//   0x00800DA0 (labels are propagated byte twins).
// Layout (retail, Ego_r.pdb less 8 bytes before / 16 bytes after the 4-byte STLport list): Route +0x8c (way point:
//   Position +8, Type +0x10 in the node), GotRoute +0xbe, Culled +0xc0, PartWayThroughAStar +0xc1. The route element is a
//   9-byte packed {C2DVector, unsigned char} pair.
// Shape: the search result position is a block-scoped local (the pair temporary reuses its word); the empty iterator
//   tags land in the dead `to` argument slot; STLport push_back inline with the out-of-line overflow path.
inline void* operator new(unsigned int, void* where) { return where; }
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class C3DVector { public: float X, Y, Z; };
#pragma pack(push, 1)
struct SRoutePoint {                               // std::pair<C2DVector, unsigned char> (9 bytes)
    C2DVector     first;
    unsigned char second;
    SRoutePoint(const C2DVector& a, const unsigned char& b) : first(a), second(b) { }
};
#pragma pack(pop)
struct __true_type { __true_type() { } };
struct _Nonscalar_ptr_iterator_tag { _Nonscalar_ptr_iterator_tag() { } };
SRoutePoint* __fastcall _Move(SRoutePoint* first, SRoutePoint* last, SRoutePoint* dest, const _Nonscalar_ptr_iterator_tag& tag, bool flag);   // 0x008001F0
void* FindRef_free__Move_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) SRoutePoint* __fastcall _Move(SRoutePoint* first, SRoutePoint* last, SRoutePoint* dest, const _Nonscalar_ptr_iterator_tag& tag, bool flag)
{
    void* p = FindRef_free__Move_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)dest = p;
    return 0;
}
#pragma auto_inline(on)
template <class T> struct _STLP_alloc_proxy {     // STLport: the end-of-storage pointer over the allocator
    T* _M_data;
};
struct CRoutePointVector {                         // STLport vector<SRoutePoint>
    SRoutePoint* _M_start;
    SRoutePoint* _M_finish;
    _STLP_alloc_proxy<SRoutePoint> _M_end_of_storage;
    void reserve(unsigned long n);                 // 0x00800CA0
    void _M_insert_overflow(SRoutePoint* position, const SRoutePoint& x, const __true_type&, unsigned long fillLen, bool atEnd);   // 0x00800DA0
    void erase(SRoutePoint* first, SRoutePoint* last)
    {
        SRoutePoint* kept = _Move(last, _M_finish, first, _Nonscalar_ptr_iterator_tag(), false);
        _M_finish = kept;
    }
    void push_back(const SRoutePoint& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            if (_M_finish)
                new (_M_finish) SRoutePoint(x);
            _M_finish = (SRoutePoint*)((unsigned char*)_M_finish + sizeof(SRoutePoint));
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* FindRef_CRoutePointVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CRoutePointVector::_M_insert_overflow(SRoutePoint* position, const SRoutePoint& x, const __true_type& p2, unsigned long fillLen, bool atEnd)
{
    void* p = FindRef_CRoutePointVector__M_insert_overflow_0();
    if (!p)
        return;
    *(void**)position = p;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)
struct SWayPointNode {                             // _List_node<CNavWayPoint>
    SWayPointNode* _M_next;
    SWayPointNode* _M_prev;
    C2DVector      Position;                       // +0x08
    unsigned char  Type;                           // +0x10
};
inline void __distance(SWayPointNode* first, SWayPointNode* last, unsigned long& n)
{
    while (first != last) {
        first = first->_M_next;
        ++n;
    }
}
struct CWayPointList {                             // STLport list<CNavWayPoint> (retail: the sentinel pointer)
    SWayPointNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        __distance(_M_node->_M_next, _M_node, n);
        return n;
    }
};
enum ENavigatorReturn { NAV_RETURN_0, NAV_RETURN_FAILED };

class CNavigatorAStar { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual ENavigatorReturn MoveToPos(const C3DVector& from, const C3DVector& to, float speed, C3DVector& position, bool a, bool b, bool c, bool d, bool e, bool f, float g);   // slot 3 (+0x0c)
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
    virtual void Interrupt();                      // slot 18 (+0x48)
    unsigned char _pad04[0x8c - 0x04];
    CWayPointList Route;                           // +0x8c
    unsigned char _pad90[0xbe - 0x90];
    bool          GotRoute;                        // +0xbe
    bool          SlowedDownForLastPoint;          // +0xbf
    bool          Culled;                          // +0xc0
    bool          PartWayThroughAStar;             // +0xc1
    virtual void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoutePointVector& route);
};

void CNavigatorAStar::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoutePointVector& route)
{
    Interrupt();
    {
        C3DVector position;
        do {
            if (MoveToPos(from, to, 1.0f, position, false, true, true, false, true, false, 0.0f) == NAV_RETURN_FAILED)
                return;
        } while (PartWayThroughAStar || !GotRoute || !Culled);
    }
    route.erase(route._M_start, route._M_finish);
    route.reserve(Route.size());
    for (SWayPointNode* node = Route._M_node->_M_next; node != Route._M_node; node = node->_M_next)
        route.push_back(SRoutePoint(node->Position, node->Type));
}