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
};
#endif // FABLE_ENGINE_CNAVREGION_H
