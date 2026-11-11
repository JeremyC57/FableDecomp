#ifndef FABLE_ENGINE_CNAVREGION_H
#define FABLE_ENGINE_CNAVREGION_H
// CNavRegion (new header, chunk 736): Ego_r.pdb RegionID +0, Connections +4 (STLport list<CCountedPointer<CNavRegionConnection> >,
// 4 bytes in retail), MostRecentSearchID +8; sizeof 0xC in retail (landed ctor 0x00A78680, SetAsConnectedToRegion 0x00A786C0).
// The copy constructor and destructor are implicit (out-of-line COMDATs: copy 0x00A83110; Connections.clear() 0x00A80A20).
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CNavRegionConnection;
class CNavRegion {
public:
    long RegionID;                                                       // +0x0
    std::list<CCountedPointer<CNavRegionConnection> > Connections;      // +0x4
    long MostRecentSearchID;                                             // +0x8
    CNavRegion(long mapIndex, long internalIndex);                       // 0x00A78680 ??0CNavRegion@@QAE@JJ@Z
#ifdef FABLE_RETAIL_CNAVREGION_ISCONNECTED
    // Retail opt-in (chunk 875, row 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity): Ego_r.pdb / FableWin.pdb
    // ?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z, out of line at 0x00A77780 (`push eax ; mov ecx,eax ; call 0x00A77780`,
    // 0x00A1F18C: the breadth-first search over Connections stamped with the search counter 0x013D2D28). Declaration only.
    bool IsConnectedToRegion(const CNavRegion& region) const;            // 0x00A77780
#endif
};
#endif // FABLE_ENGINE_CNAVREGION_H
