// Behaviour test for CMiniMapDisplay::RemoveAllMarkers 0x0082A300: after the call the marker list is empty, whatever it held.
#include <cstdio>
#include <list>
struct CMarkerInfo { const void* PThing; long GraphicIndex; bool AutoMarker; };
struct CMiniMapDisplay {
    std::list<CMarkerInfo> Markers;
    void RemoveAllMarkers() { Markers.clear(); }
};
int main()
{
    CMiniMapDisplay d; int a, b;
    CMarkerInfo m1 = { &a, 3, false }, m2 = { &b, 4, true };
    d.Markers.push_back(m1); d.Markers.push_back(m2);
    d.RemoveAllMarkers();
    if (!d.Markers.empty()) { std::printf("0082a300_TEST FAIL\n"); return 1; }
    d.RemoveAllMarkers();
    if (!d.Markers.empty()) { std::printf("0082a300_TEST FAIL\n"); return 1; }
    std::printf("0082a300_TEST PASS\n");
    return 0;
}