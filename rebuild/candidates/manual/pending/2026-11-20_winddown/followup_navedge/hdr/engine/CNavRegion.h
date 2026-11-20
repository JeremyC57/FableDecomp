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
#ifdef FABLE_RETAIL_CNAVREGION_CONNECTIONS
    // Re-port opt-in (2026-11-19, rows 0x00A786C0 / 0x00A78060 and their callers 0x00A78A90 / 0x00A780B0 / 0x00A78800): Ego_r.pdb
    // ?SetAsConnectedToRegion@CNavRegion@@QAEXAAV1@@Z (0x00A786C0) and ?RemoveConnectionToRegion@CNavRegion@@QAEXAAV1@@Z (0x00A78060), both
    // taking the other region by REFERENCE, `ret 4`; and the two private list helpers: ?RemoveConnectionFromConnectionList@CNavRegion@@
    // AAEXPBVCNavRegionConnection@@@Z (0x00A77710, landed) and ?AddConnectionToConnectionList@CNavRegion@@AAEXAAV?$CCountedPointer@
    // VCNavRegionConnection@@@@@Z (Ego_r 0x00BBE490; no retail copy: 0x00A786C0 expands the list push_back at both sites). The connection
    // record reaches the private helpers (its inline RemoveConnection calls 0x00A77710 on both regions, 0x00A7809B / 0x00A780A4), so it
    // is a friend (Inferred: C++ access requires it; no public wrapper exists in any PDB). AddConnectionToConnectionList is an Ego_d.pdb
    // GPROC32 (28 debug bytes, lines 87..89: one statement = the push_back; parameter `pconnection`); RemoveConnectionFromConnectionList's
    // Ego_d parameter is `pconnection_to_remove` (audit h3c2b).
    void SetAsConnectedToRegion(CNavRegion& region);                     // 0x00A786C0
    void RemoveConnectionToRegion(CNavRegion& region);                   // 0x00A78060
private:
    friend class CNavRegionConnection;
    void AddConnectionToConnectionList(CCountedPointer<CNavRegionConnection>& pconnection) { Connections.push_back(pconnection); }
    void RemoveConnectionFromConnectionList(const CNavRegionConnection* pconnection_to_remove);   // 0x00A77710
public:
#endif
#ifdef FABLE_RETAIL_CNAVREGION_ISCONNECTED
    // Retail opt-in (chunk 875, row 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity): Ego_r.pdb / FableWin.pdb
    // ?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z, out of line at 0x00A77780 (`push eax ; mov ecx,eax ; call 0x00A77780`,
    // 0x00A1F18C: the breadth-first search over Connections stamped with the search counter 0x013D2D28). Declaration only.
    bool IsConnectedToRegion(const CNavRegion& region) const;            // 0x00A77780
#endif
};
#endif // FABLE_ENGINE_CNAVREGION_H
