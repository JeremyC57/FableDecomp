// CNavRegion::SetAsConnectedToRegion(CNavRegion& region) -- count one more connection to a region already linked to this one, or create a shared connection record (both regions, one connection) and add it to this region's and the other region's connection lists
// Leaf: ?SetAsConnectedToRegion@CNavRegion@@QAEXAAV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A786C0, 265 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb ?SetAsConnectedToRegion@CNavRegion@@QAEXAAV1@@Z (0x00BBE570): the other region by REFERENCE (the earlier text spelled a
//   pointer). Parameter and locals: Ego_d.pdb (debug length 385): this, region; { it { pconnection } }, pconnection a
//   `CCountedPointer<CNavRegionConnection>&` (type 0x354C6) bound to *it at the top of the loop body (Ego_d line 71, 11 bytes).
// Shape: Ego_d C13 line table (67..83): 69 the for, 71 pconnection, 72 the test, 75 AddConnection, 76 / 77 return, 81 ONE statement of
//   125 bytes that builds the connection and adds it to this region's list, 82 (20 bytes) the other region's add of Connections.back().
// Inferred: the block-scoped `connection` local. Ego_d has no such local: its line 81 passes a temporary counted pointer straight to
//   AddConnectionToConnectionList(CCountedPointer<CNavRegionConnection>&), a temporary bound to a non-const reference (an MSVC extension,
//   not portable C++). Both spellings were graded RELOCATION_MATCH (ATTEMPTS.log); the portable one is written. Coordinator decision.
// Callees: operator new 0x00BFEA1A (the connection and its count block), malloc 0x00BFEA0E (list nodes), operator delete 0x00BFE9BC (count
//   block release), CCountedPointer<CNavRegionConnection>::DeleteData 0x00A7C0A0 (stored as the block's delete function; Ego_r.pdb
//   ?DeleteData@?$CCountedPointer@VCNavRegionConnection@@@@SIXPAX@Z -- the earlier text bound a made-up DeleteNavRegionConnection).
// Re-port (2026-11-19): the local CNavRegion / CCountedPointer / count-block / STLport-list stand-ins are replaced by the shared
//   engine/CNavRegion.h (opt-in FABLE_RETAIL_CNAVREGION_CONNECTIONS), engine/CCountedPointer.h and the STLport list of stlport/fable_containers.h.
#define FABLE_RETAIL_STLP_LIST
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

void CNavRegion::SetAsConnectedToRegion(CNavRegion& region)
{
    for (std::list<CCountedPointer<CNavRegionConnection> >::iterator it = Connections.begin(); it != Connections.end(); ++it) {
        CCountedPointer<CNavRegionConnection>& pconnection = *it;
        if (&pconnection->GetRegionConnectedTo(*this) == &region) {
            pconnection->AddConnection();
            return;
        }
    }
    {
        CCountedPointer<CNavRegionConnection> connection(new CNavRegionConnection(*this, region));
        AddConnectionToConnectionList(connection);
    }
    region.AddConnectionToConnectionList(Connections.back());
}
