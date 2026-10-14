// CNavRegion::RemoveConnectionToRegion(CNavRegion* other) -- walk the connection list at +4 (a sentinel; each node's connection at +8 holds the two regions and a reference count at +8): the first connection whose other end is `other` loses one reference, and at zero both regions drop it (0x00A77710); the walk stops at the first match
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A78060, length 80 bytes.
// Leaf: RemoveConnectionToRegion
// Callees: 0x00A77710 (thiscall on each region with the connection; the manifest label `remove_if<...>` is propagated). Shape: the sentinel in edi, the other region in ebx, the far end chosen with a two-way branch, `ret 4`.
struct CNavRegion;
struct SNavConnection_00a78060 {
    CNavRegion* m_pRegionA;                        // +0x00
    CNavRegion* m_pRegionB;                        // +0x04
    long        m_references;                      // +0x08
};
struct SConnectionNode_00a78060 {
    SConnectionNode_00a78060* m_pNext;             // +0x00
    SConnectionNode_00a78060* m_pPrev;             // +0x04
    SNavConnection_00a78060*  m_pConnection;       // +0x08
};

struct CNavRegion {
    long                      m_0;
    SConnectionNode_00a78060* m_pConnections;      // +0x04: the sentinel
    void RemoveConnection(SNavConnection_00a78060* connection);   // 0x00A77710
    void RemoveConnectionToRegion(CNavRegion* other);
};

void CNavRegion::RemoveConnectionToRegion(CNavRegion* other)
{
    for (SConnectionNode_00a78060* node = m_pConnections->m_pNext; node != m_pConnections; node = node->m_pNext) {
        SNavConnection_00a78060* connection = node->m_pConnection;
        CNavRegion* farEnd = this == connection->m_pRegionA ? connection->m_pRegionB : connection->m_pRegionA;
        if (farEnd == other) {
            SNavConnection_00a78060* shared = node->m_pConnection;
            if (--shared->m_references == 0) {
                shared->m_pRegionA->RemoveConnection(shared);
                shared->m_pRegionB->RemoveConnection(shared);
            }
            return;
        }
    }
}
