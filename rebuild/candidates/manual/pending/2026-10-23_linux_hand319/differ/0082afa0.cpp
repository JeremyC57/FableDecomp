// Grade: DIFFER(16v1393) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CMiniMapDisplay::InitialiseRegionExitMarkers() -- rebuild the mini-map region-exit markers: for the environment's world
//   map, collect every region-exit thing (GetAllThings) in the current map region, look up the region each one connects
//   to, mark the exits on the active quest route (NoQuestRoute / IsRouteToAppearOnMinimap / GetNextRegionOnRouteTo) and
//   insert one marker per exit into the marker map, freeing the temporary vectors and trees
// Leaf: InitialiseRegionExitMarkers
// Stub: first-pass skeleton only; the route walk, the marker trees and the vectors are not reconstructed yet.
struct CWorld;
CWorld* DrawGetEnvironment();                           // 0x004FB150 (descriptive)
class CMiniMapDisplay {
public:
    unsigned char _pad00[0xb8];
    void*         MarkerMap;                            // +0xb8
    void InitialiseRegionExitMarkers();
};
void CMiniMapDisplay::InitialiseRegionExitMarkers()
{
    if (MarkerMap == 0)
        return;
    DrawGetEnvironment();
}
