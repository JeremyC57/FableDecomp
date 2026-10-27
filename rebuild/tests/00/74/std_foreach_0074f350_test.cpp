// Behaviour test for std::for_each<CThingObject* const*, CAddObjectNotSolidTo> (0x0074F350): adds each object as not-solid-to and returns the functor.
#include <cstdio>
class CThing { public: int Id; };
class CThingObject : public CThing { public: };
class CTCSolidToThing { public:
    const CThing* Added[8]; int Count;
    void AddThingNotSolidTo(const CThing* t) { Added[Count++] = t; }
};
class CAddObjectNotSolidTo { public:
    CTCSolidToThing* TCSolidToThing;
    void operator()(CThingObject* o) { TCSolidToThing->AddThingNotSolidTo(o); }
};
static CAddObjectNotSolidTo for_each_obj(CThingObject* const* first, CThingObject* const* last, CAddObjectNotSolidTo f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
int main()
{
    CThingObject o[3]; CThingObject* v[3] = { &o[0], &o[1], &o[2] };
    CTCSolidToThing tc; tc.Count = 0;
    CAddObjectNotSolidTo f; f.TCSolidToThing = &tc;
    CAddObjectNotSolidTo r = for_each_obj(v, v + 3, f);
    if (r.TCSolidToThing != &tc || tc.Count != 3 || tc.Added[0] != &o[0] || tc.Added[2] != &o[2]) { std::printf("0074f350_TEST FAIL\n"); return 1; }
    for_each_obj(v, v, f);
    if (tc.Count != 3) { std::printf("0074f350_TEST FAIL\n"); return 1; }
    std::printf("0074f350_TEST PASS\n");
    return 0;
}