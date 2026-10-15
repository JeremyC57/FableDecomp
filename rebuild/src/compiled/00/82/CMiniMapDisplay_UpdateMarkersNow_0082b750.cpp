// CMiniMapDisplay::UpdateMarkersNow() -- when a marker update is pending: drop the markers bound to things, remove the dead ones, refresh the rest and rebuild every automatic marker family, then clear the pending flag
// Leaf: UpdateMarkersNow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082B750, length 107 bytes (+ int3 padding). Catalogued as
// CEngineShadowRenderer::OnReleaseDefaultPoolResources (a propagated label): every callee is a CMiniMapDisplay member and
// its sole caller is CMiniMapDisplay::UpdateMarkers(bool) @ 0x0082B7C0 (`if (now) UpdateMarkersNow(); else UpdateRequired = true;`).
// Callees: list<CMarkerInfo>::remove_if<CIsMarkerBoundToThing> @ 0x0082C6E0 (on +0x20), RemoveAllDeadMarkers @ 0x0082A2A0,
// UpdateMarkersInternal @ 0x0082A960, InitialiseInformationMarkers @ 0x0082A9B0, InitialiseTeleportMarkers @ 0x0082ABD0,
// InitialiseHouseMarkers @ 0x0082ACD0, InitialiseDemonDoorMarkers @ 0x0082ADD0, InitialiseTraderMarkers @ 0x0082AE80,
// InitialiseRegionExitMarkers @ 0x0082AFA0, InitialiseAttractionMarkers @ 0x0082B520, InitialiseEnemyMarkers @ 0x0082B630.
// Layout: retail STLport containers are narrower than the PDB's (12-byte vectors, so Markers sits at +0x20 and
// UpdateRequired at +0xbc instead of engine/CMiniMapDisplay.h's +0x28 / +0xd8), so `this` is modelled locally.
// Shape: the empty predicate is passed BY VALUE: its 1-byte temporary lives in the `push ecx` frame word, which is
// pushed uninitialised (`mov eax,[esp+4] ; push eax`);
// the predicate needs a user (empty) constructor, else `T()` value-initialises it with a `mov byte ptr [esp+4],0`.
struct CIsMarkerBoundToThing {
    CIsMarkerBoundToThing() {}                     // user constructor: no value-initialising byte store
};

struct CMarkerList_0082c6e0 {
    void*         m_pHead;
    void remove_if(CIsMarkerBoundToThing pred);    // 0x0082C6E0
};

struct CMiniMapDisplay {
    unsigned char        _pad[0x20];
    CMarkerList_0082c6e0 Markers;                  // +0x20
    unsigned char        _pad24[0x98];
    bool                 UpdateRequired;           // +0xbc
    void RemoveAllDeadMarkers();                   // 0x0082A2A0
    void UpdateMarkersInternal();                  // 0x0082A960
    void InitialiseInformationMarkers();           // 0x0082A9B0
    void InitialiseTeleportMarkers();              // 0x0082ABD0
    void InitialiseHouseMarkers();                 // 0x0082ACD0
    void InitialiseDemonDoorMarkers();             // 0x0082ADD0
    void InitialiseTraderMarkers();                // 0x0082AE80
    void InitialiseRegionExitMarkers();            // 0x0082AFA0
    void InitialiseAttractionMarkers();            // 0x0082B520
    void InitialiseEnemyMarkers();                 // 0x0082B630
    void UpdateMarkersNow();
};

void CMiniMapDisplay::UpdateMarkersNow()
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