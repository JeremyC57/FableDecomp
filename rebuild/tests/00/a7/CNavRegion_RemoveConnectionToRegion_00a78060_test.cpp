// Behaviour test for 0x00A78060 CNavRegion::RemoveConnectionToRegion (links the REAL row object; the prologue below is the row's own).
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CNavRegion.h"
// CNavRegionConnection (Ego_r.pdb, 0xC bytes: FirstRegion +0, SecondRegion +4, NumberOfConnections +8; no shared header yet): the record two
// connected regions share. Members: Ego_r.pdb ?GetRegionConnectedTo@CNavRegionConnection@@QBEAAVCNavRegion@@ABV2@@Z and
// ?RemoveConnection@CNavRegionConnection@@QAEXXZ (both inline in retail). Inferred: the constructor's and AddConnection's spellings
// (no PDB record; the bodies are retail's expanded bytes).
class CNavRegionConnection {
public:
    CNavRegion* FirstRegion;                                             // +0x0
    CNavRegion* SecondRegion;                                            // +0x4
    long        NumberOfConnections;                                     // +0x8
    CNavRegionConnection(CNavRegion& first, CNavRegion& second) : FirstRegion(&first), SecondRegion(&second), NumberOfConnections(1) { }
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

// ---- test harness: links the REAL row object ----
#include <stdio.h>
#include <string.h>
static int g_fail;
#define CHECK(c) do { if (!(c)) { ++g_fail; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
CNavRegion::CNavRegion(long mapIndex, long internalIndex) : RegionID(mapIndex * 65536 + internalIndex), MostRecentSearchID(0) { }
typedef CCountedPointer<CNavRegionConnection> CConnectionPointer;
typedef std::list<CConnectionPointer> CConnectionList;
static int Count(CConnectionList& l) { int n = 0; for (CConnectionList::iterator it = l.begin(); it != l.end(); ++it) ++n; return n; }

static char g_seq[16]; static const CNavRegionConnection* g_lastConnection; static CNavRegion* g_order[8]; static int g_calls;
void CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection* connection)   // 0x00A77710 (another row): recorded only
{
    if (g_calls < 8) g_order[g_calls] = this;
    ++g_calls; g_lastConnection = connection; strcat(g_seq, "R");
}
int main()
{
    CNavRegion r1(1, 1), r2(1, 2), r3(1, 3), r4(1, 4);
    CNavRegionConnection* c12 = new CNavRegionConnection(r1, r2); c12->NumberOfConnections = 2;
    CNavRegionConnection* c31 = new CNavRegionConnection(r3, r1);                      // r1 is the SECOND region here
    {
        CConnectionPointer p12(c12), p31(c31);
        r1.Connections.push_back(p12); r2.Connections.push_back(p12);
        r1.Connections.push_back(p31); r3.Connections.push_back(p31);
    }
    r1.RemoveConnectionToRegion(r2);                                                    // still one connection left
    CHECK(c12->NumberOfConnections == 1); CHECK(g_calls == 0); CHECK(c31->NumberOfConnections == 1);
    r1.RemoveConnectionToRegion(r4);                                                    // not connected: nothing
    CHECK(g_calls == 0); CHECK(c12->NumberOfConnections == 1); CHECK(c31->NumberOfConnections == 1);
    r1.RemoveConnectionToRegion(r3);                                                    // last connection: removed from both lists
    CHECK(c31->NumberOfConnections == 0); CHECK(g_calls == 2); CHECK(strcmp(g_seq, "RR") == 0);
    CHECK(g_order[0] == &r3); CHECK(g_order[1] == &r1); CHECK(g_lastConnection == c31);
    CHECK(c12->NumberOfConnections == 1);
    r2.RemoveConnectionToRegion(r1);                                                    // from the other side (r2 is SecondRegion of c12)
    CHECK(c12->NumberOfConnections == 0); CHECK(g_calls == 4); CHECK(g_order[2] == &r1); CHECK(g_order[3] == &r2); CHECK(g_lastConnection == c12);
    r4.RemoveConnectionToRegion(r1);                                                    // empty list
    CHECK(g_calls == 4);
    // two separate records to the same region: only the FIRST is touched (the row returns after its hit)
    CNavRegion r5(1, 5), r6(1, 6);
    CNavRegionConnection* c56a = new CNavRegionConnection(r5, r6); c56a->NumberOfConnections = 3;
    CNavRegionConnection* c56b = new CNavRegionConnection(r6, r5); c56b->NumberOfConnections = 3;
    { CConnectionPointer pa(c56a), pb(c56b); r5.Connections.push_back(pa); r5.Connections.push_back(pb); }
    r5.RemoveConnectionToRegion(r6);
    CHECK(c56a->NumberOfConnections == 2); CHECK(c56b->NumberOfConnections == 3); CHECK(g_calls == 4);
    CHECK(Count(r1.Connections) == 2);                                                  // the stubbed list helper removes nothing
    if (g_fail) { printf("00a78060_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a78060_TEST PASS"); return 0;
}
