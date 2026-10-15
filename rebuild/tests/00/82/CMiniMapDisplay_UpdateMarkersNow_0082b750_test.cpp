// Behaviour test for 0x0082B750 CMiniMapDisplay::UpdateMarkersNow: eleven steps in order when pending, nothing otherwise.
#include <cstdio>

static int g_log[16];
static int g_n = 0;

struct CIsMarkerBoundToThing { CIsMarkerBoundToThing() {} };
struct CMarkerList { void remove_if(CIsMarkerBoundToThing) { g_log[g_n++] = 0; } };

struct CMiniMapDisplay {
    CMarkerList Markers;
    bool UpdateRequired;
    void RemoveAllDeadMarkers()        { g_log[g_n++] = 1; }
    void UpdateMarkersInternal()       { g_log[g_n++] = 2; }
    void InitialiseInformationMarkers(){ g_log[g_n++] = 3; }
    void InitialiseTeleportMarkers()   { g_log[g_n++] = 4; }
    void InitialiseHouseMarkers()      { g_log[g_n++] = 5; }
    void InitialiseDemonDoorMarkers()  { g_log[g_n++] = 6; }
    void InitialiseTraderMarkers()     { g_log[g_n++] = 7; }
    void InitialiseRegionExitMarkers() { g_log[g_n++] = 8; }
    void InitialiseAttractionMarkers() { g_log[g_n++] = 9; }
    void InitialiseEnemyMarkers()      { g_log[g_n++] = 10; }
    void UpdateMarkersNow()
    {
        if (UpdateRequired) {
            Markers.remove_if(CIsMarkerBoundToThing());
            RemoveAllDeadMarkers();
            UpdateMarkersInternal();
            InitialiseInformationMarkers();
            InitialiseTeleportMarkers();
            InitialiseHouseMarkers();
            InitialiseDemonDoorMarkers();
            InitialiseTraderMarkers();
            InitialiseRegionExitMarkers();
            InitialiseAttractionMarkers();
            InitialiseEnemyMarkers();
            UpdateRequired = false;
        }
    }
};

int main()
{
    CMiniMapDisplay m;
    m.UpdateRequired = false;
    m.UpdateMarkersNow();
    if (g_n != 0) { std::printf("0082b750_TEST FAIL\n"); return 1; }
    m.UpdateRequired = true;
    m.UpdateMarkersNow();
    if (g_n != 11 || m.UpdateRequired) { std::printf("0082b750_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 11; ++i)
        if (g_log[i] != i) { std::printf("0082b750_TEST FAIL\n"); return 1; }
    std::printf("0082b750_TEST PASS\n");
    return 0;
}