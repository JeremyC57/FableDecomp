// CNavRegion::RemoveConnectionToRegion(CNavRegion& region) -- walk this region's connection list: the first connection whose other end is `region` loses one count, and when the count reaches zero the connection is removed from both regions' lists
// Leaf: ?RemoveConnectionToRegion@CNavRegion@@QAEXAAV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78060, 80 bytes, `ret 4`.
// Identity: Ego_r.pdb ?RemoveConnectionToRegion@CNavRegion@@QAEXAAV1@@Z (0x00BBE6B0): the other region by REFERENCE (the earlier text spelled a
//   pointer). Parameter and locals: Ego_d.pdb (debug length 822): this, region; { it { pconnection } }, pconnection a
//   `CCountedPointer<CNavRegionConnection>&` (type 0x354C6) bound to *it at the top of the loop body (Ego_d line 111, 11 bytes); every
//   `pconnection->` goes through the reference, which is why retail reloads the record at the hit (`mov esi,[eax+8]` 0x00A78090).
// Shape: Ego_d C13 line table (108..121): 109 the for, 111 pconnection, 112 the test, 116 RemoveConnection (15 bytes), 117 / 118 return.
//   Lines 115 (294 bytes) and 120 (270 bytes, after the loop) have no retail counterpart (debug-only checks) and are not written.
// Callees: CNavRegion::RemoveConnectionFromConnectionList 0x00A77710 (twice, through the inline CNavRegionConnection::RemoveConnection).
// Re-port (2026-11-19): the local CNavRegion and the address-suffixed list / connection stand-ins are replaced by the shared engine/CNavRegion.h
//   (Connections is the STLport list of counted connection pointers; opt-in FABLE_RETAIL_CNAVREGION_CONNECTIONS) with Ego_d's names.
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CNavRegion.h"
// CNavRegionConnection (Ego_r.pdb, 0xC bytes: FirstRegion +0, SecondRegion +4, NumberOfConnections +8; no shared header yet): the record two
// connected regions share. Members: Ego_r.pdb ?GetRegionConnectedTo@CNavRegionConnection@@QBEAAVCNavRegion@@ABV2@@Z and
// ?RemoveConnection@CNavRegionConnection@@QAEXXZ (inline here; retail's out-of-line copy of RemoveConnection is 0x00A778B0). The constructor
// and AddConnection are Ego_d.pdb GPROC32 records too (43 / 26 debug bytes, header lines 121 / 123; constructor parameters first_region,
// second_region, both CNavRegion&): their names and parameters are evidenced (audit h3c2b); the bodies are retail's expanded bytes.
class CNavRegionConnection {
public:
    CNavRegion* FirstRegion;                                             // +0x0
    CNavRegion* SecondRegion;                                            // +0x4
    long        NumberOfConnections;                                     // +0x8
    CNavRegionConnection(CNavRegion& first_region, CNavRegion& second_region) : FirstRegion(&first_region), SecondRegion(&second_region), NumberOfConnections(1) { }
    CNavRegion& GetRegionConnectedTo(const CNavRegion& region) const { return &region == FirstRegion ? *SecondRegion : *FirstRegion; }
    void AddConnection() { ++NumberOfConnections; }
    void RemoveConnection()
    {
        if (--NumberOfConnections == 0) {
            FirstRegion->RemoveConnectionFromConnectionList(this);
            SecondRegion->RemoveConnectionFromConnectionList(this);
        }
    }
};

void CNavRegion::RemoveConnectionToRegion(CNavRegion& region)
{
    for (std::list<CCountedPointer<CNavRegionConnection> >::iterator it = Connections.begin(); it != Connections.end(); ++it) {
        CCountedPointer<CNavRegionConnection>& pconnection = *it;
        if (&pconnection->GetRegionConnectedTo(*this) == &region) {
            pconnection->RemoveConnection();
            return;
        }
    }
}
