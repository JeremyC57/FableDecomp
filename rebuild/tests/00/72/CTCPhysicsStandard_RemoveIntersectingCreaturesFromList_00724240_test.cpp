// Behaviour test for 0x00724240 CTCPhysicsStandard::RemoveIntersectingCreaturesFromList (find_if / remove_copy_if
// callees and the predicate modelled).
#include <cstdio>
#include <cstring>
class CThing { public: int zone; };
class CIsCreatureIsIntersecting { public:
    CThing* Owner;
    CIsCreatureIsIntersecting(CThing* owner) : Owner(owner) {}
    bool operator()(CThing* thing) const { return thing->zone == Owner->zone; }
};
static CThing** find_if_(CThing** first, CThing** last, CIsCreatureIsIntersecting pred)
{
    while (first != last && !pred(*first)) ++first;
    return first;
}
static CThing** remove_copy_if_(CThing** first, CThing** last, CThing** result, CIsCreatureIsIntersecting pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}
static CThing** remove_if_(CThing** first, CThing** last, CIsCreatureIsIntersecting pred)
{
    first = find_if_(first, last, pred);
    if (first == last) return first;
    CThing** next = first;
    return remove_copy_if_(++next, last, first, pred);
}
static void* copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)std::memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
struct CThingPtrVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThing** erase(CThing** first, CThing** last) { CThing** i = (CThing**)copy_trivial(last, _M_finish, first); _M_finish = i; return first; }
};
struct CTCPhysicsStandard {
    CThing* owner;
    void RemoveIntersectingCreaturesFromList(CThingPtrVector& things) const
    {
        things.erase(remove_if_(things._M_start, things._M_finish, CIsCreatureIsIntersecting(owner)), things._M_finish);
    }
};
int main()
{
    CThing me; me.zone = 7;
    CThing t[6];
    int zones[6] = { 1, 7, 2, 7, 7, 3 };
    CThing* buf[6];
    for (int i = 0; i < 6; ++i) { t[i].zone = zones[i]; buf[i] = &t[i]; }
    CThingPtrVector v; v._M_start = buf; v._M_finish = buf + 6; v._M_end_of_storage = buf + 6;
    CTCPhysicsStandard tc; tc.owner = &me;
    tc.RemoveIntersectingCreaturesFromList(v);
    if (v._M_finish - v._M_start != 3 || buf[0] != &t[0] || buf[1] != &t[2] || buf[2] != &t[5]) { std::printf("00724240_TEST FAIL\n"); return 1; }
    tc.RemoveIntersectingCreaturesFromList(v);
    if (v._M_finish - v._M_start != 3) { std::printf("00724240_TEST FAIL\n"); return 1; }
    me.zone = 1;
    tc.RemoveIntersectingCreaturesFromList(v);
    if (v._M_finish - v._M_start != 2 || buf[0] != &t[2]) { std::printf("00724240_TEST FAIL\n"); return 1; }
    std::printf("00724240_TEST PASS\n");
    return 0;
}