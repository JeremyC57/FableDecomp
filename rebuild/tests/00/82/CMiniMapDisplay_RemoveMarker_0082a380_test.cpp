// Behaviour test for CMiniMapDisplay::RemoveMarker(const CThing&) 0x0082A380: remove_if with CIsMarkerBoundToThing drops every
// marker whose thing is the argument and that is not an automatic marker (the predicate 0x0082C770 tests AutoMarker), keeps the rest.
#include <cstdio>
#include <list>
struct CThing { int id; };
struct CMarkerInfo { const CThing* PThing; long GraphicIndex; bool AutoMarker; };
struct CIsMarkerBoundToThing {
    const CThing* PThing;
    CIsMarkerBoundToThing(const CThing& thing) : PThing(&thing) { }
    bool operator()(const CMarkerInfo& m) const { return m.PThing == PThing && !m.AutoMarker; }
};
struct CMiniMapDisplay {
    std::list<CMarkerInfo> Markers;
    void RemoveMarker(const CThing& thing)                     // list::remove_if(pred): erase every element the predicate accepts
    {
        CIsMarkerBoundToThing pred(thing);
        for (std::list<CMarkerInfo>::iterator it = Markers.begin(); it != Markers.end(); )
            if (pred(*it)) it = Markers.erase(it); else ++it;
    }
};
int main()
{
    CThing a = { 1 }, b = { 2 };
    CMiniMapDisplay d;
    CMarkerInfo m[4] = { { &a, 1, false }, { &b, 2, false }, { &a, 3, true }, { &a, 4, false } };
    for (int i = 0; i < 4; ++i) d.Markers.push_back(m[i]);
    d.RemoveMarker(a);
    if (d.Markers.size() != 2) { std::printf("0082a380_TEST FAIL\n"); return 1; }
    for (std::list<CMarkerInfo>::iterator it = d.Markers.begin(); it != d.Markers.end(); ++it)
        if (it->PThing == &a && !it->AutoMarker) { std::printf("0082a380_TEST FAIL\n"); return 1; }
    std::printf("0082a380_TEST PASS\n");
    return 0;
}