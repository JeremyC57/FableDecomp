// Behaviour test for 00a1d5d0: body copied from src/00a1d5d0.cpp (CNavigatorAStar::CalculateRouteFromTo), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
inline void* operator new(unsigned int, void* where) { return where; }
struct C2DVector { float X, Y; C2DVector() { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
struct C3DVector { float X, Y, Z; };
#pragma pack(push, 1)
struct SRoutePoint { C2DVector first; unsigned char second; SRoutePoint(const C2DVector& a, const unsigned char& b) : first(a), second(b) { } };
#pragma pack(pop)
struct __true_type { __true_type() { } };
struct _Nonscalar_ptr_iterator_tag { _Nonscalar_ptr_iterator_tag() { } };
static int g_move = 0;
static SRoutePoint* __fastcall _Move(SRoutePoint* first, SRoutePoint* last, SRoutePoint* dest, const _Nonscalar_ptr_iterator_tag&, bool flag)
{ ++g_move; CHECK(!flag); while (first != last) { memcpy(dest, first, sizeof(SRoutePoint)); ++first; ++dest; } return dest; }
template <class T> struct _STLP_alloc_proxy { T* _M_data; };
static int g_reserve = 0, g_overflow = 0; static unsigned long g_reserveN = 0;
struct CRoutePointVector {
    SRoutePoint* _M_start; SRoutePoint* _M_finish; _STLP_alloc_proxy<SRoutePoint> _M_end_of_storage;
    void reserve(unsigned long n) { ++g_reserve; g_reserveN = n; }
    void _M_insert_overflow(SRoutePoint* position, const SRoutePoint& x, const __true_type&, unsigned long fillLen, bool atEnd)
    { ++g_overflow; CHECK(position == _M_finish && fillLen == 1 && atEnd); new (_M_finish) SRoutePoint(x); _M_finish = (SRoutePoint*)((unsigned char*)_M_finish + sizeof(SRoutePoint)); _M_end_of_storage._M_data = _M_finish; }
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
struct SWayPointNode { SWayPointNode* _M_next; SWayPointNode* _M_prev; C2DVector Position; unsigned char Type; };
inline void __distance(SWayPointNode* first, SWayPointNode* last, unsigned long& n) { while (first != last) { first = first->_M_next; ++n; } }
struct CWayPointList { SWayPointNode* _M_node; unsigned long size() const { unsigned long n = 0; __distance(_M_node->_M_next, _M_node, n); return n; } };
enum ENavigatorReturn { NAV_RETURN_0, NAV_RETURN_FAILED };
static int g_interrupt = 0, g_move2pos = 0; static int g_failAt = -1; static int g_readyAt = 1;
struct CNavigatorAStar {
    virtual ENavigatorReturn MoveToPos(const C3DVector& from, const C3DVector& to, float speed, C3DVector& position, bool a, bool b, bool c, bool d, bool e, bool f, float g)
    {
        ++g_move2pos; CHECK(speed == 1.0f && !a && b && c && !d && e && !f && g == 0.0f); CHECK(from.X == 1.0f && to.X == 2.0f); position.X = 0;
        if (g_move2pos == g_failAt) return NAV_RETURN_FAILED;
        if (g_move2pos >= g_readyAt) { PartWayThroughAStar = false; GotRoute = true; Culled = true; }
        return NAV_RETURN_0;
    }
    virtual void Interrupt() { ++g_interrupt; PartWayThroughAStar = true; GotRoute = false; Culled = false; }
    unsigned char _pad04[0x8c - 0x04];
    CWayPointList Route; unsigned char _pad90[0xbe - 0x90];
    bool GotRoute; bool SlowedDownForLastPoint; bool Culled; bool PartWayThroughAStar;
    CNavigatorAStar() : GotRoute(false), SlowedDownForLastPoint(false), Culled(false), PartWayThroughAStar(false) { Route._M_node = 0; }
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
int main()
{
    SWayPointNode head, nodes[3];
    head._M_next = nodes; head._M_prev = nodes + 2;
    for (int i = 0; i < 3; ++i) { nodes[i]._M_next = i < 2 ? nodes + i + 1 : &head; nodes[i]._M_prev = i ? nodes + i - 1 : &head; nodes[i].Position.X = 10.0f * i; nodes[i].Position.Y = 1.0f + i; nodes[i].Type = (unsigned char)(i + 5); }
    CNavigatorAStar nav; nav.Route._M_node = &head;
    unsigned char storage[sizeof(SRoutePoint) * 6]; memset(storage, 0, sizeof storage);
    CRoutePointVector route; route._M_start = (SRoutePoint*)storage; route._M_finish = (SRoutePoint*)(storage + sizeof(SRoutePoint)); route._M_end_of_storage._M_data = (SRoutePoint*)(storage + 2 * sizeof(SRoutePoint));
    C3DVector from = { 1.0f, 0, 0 }, to = { 2.0f, 0, 0 };
    g_readyAt = 3;
    nav.CalculateRouteFromTo(from, to, route);              // three searches until the route is complete and culled
    CHECK(g_interrupt == 1 && g_move2pos == 3 && g_move == 1 && g_reserve == 1 && g_reserveN == 3 && g_overflow == 1);
    CHECK((unsigned char*)route._M_finish - storage == 3 * sizeof(SRoutePoint));
    CHECK(route._M_start[0].first.X == 0.0f && route._M_start[0].second == 5 && route._M_start[2].first.Y == 3.0f && route._M_start[2].second == 7);
    g_failAt = 5; g_readyAt = 99;
    nav.CalculateRouteFromTo(from, to, route);              // the search fails: nothing touched
    CHECK(g_interrupt == 2 && g_move2pos == 5 && g_move == 1 && g_reserve == 1);
    if (fails == 0) printf("00a1d5d0_TEST PASS\n"); else printf("00a1d5d0_TEST FAIL (%d)\n", fails);
    return fails;
}