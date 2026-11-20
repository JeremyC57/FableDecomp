// Behaviour test for 0x00A786C0 CNavRegion::SetAsConnectedToRegion (links the REAL row object; the prologue below is the row's own).
#define FABLE_RETAIL_STLP_LIST
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

void CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection*) { ++g_fail; }   // never reached by this row
int main()
{
    {
        CNavRegion a(1, 1), b(1, 2), c(1, 3);
        a.SetAsConnectedToRegion(b);
        CHECK(Count(a.Connections) == 1); CHECK(Count(b.Connections) == 1);
        CConnectionPointer& pa = *a.Connections.begin(); CConnectionPointer& pb = *b.Connections.begin();
        CNavRegionConnection* ab = pa.Data;
        CHECK(ab->FirstRegion == &a); CHECK(ab->SecondRegion == &b); CHECK(ab->NumberOfConnections == 1);
        CHECK(pb.Data == ab); CHECK(pa.Info == pb.Info); CHECK(pa.Info->RefCount == 2);          // the local counted pointer was released
        CHECK(pa.Info->DeleteFunc == &CConnectionPointer::DeleteData); CHECK(pa.Info->Data == ab);
        a.SetAsConnectedToRegion(b);                                                            // existing: the count is bumped
        CHECK(Count(a.Connections) == 1); CHECK(Count(b.Connections) == 1); CHECK(ab->NumberOfConnections == 2);
        b.SetAsConnectedToRegion(a);                                                            // found from the other side
        CHECK(Count(b.Connections) == 1); CHECK(Count(a.Connections) == 1); CHECK(ab->NumberOfConnections == 3);
        a.SetAsConnectedToRegion(c);                                                            // a second record, appended at the back
        CHECK(Count(a.Connections) == 2); CHECK(Count(c.Connections) == 1); CHECK(Count(b.Connections) == 1);
        CConnectionList::iterator last = a.Connections.begin(); ++last;
        CHECK(last->Data->FirstRegion == &a); CHECK(last->Data->SecondRegion == &c); CHECK(last->Data->NumberOfConnections == 1);
        CHECK(last->Info->RefCount == 2); CHECK(c.Connections.begin()->Data == last->Data);
        CHECK(a.Connections.begin()->Data == ab); CHECK(pa.Info->RefCount == 2);
        c.SetAsConnectedToRegion(b);                                                            // c already has a record (with a): b is new
        CHECK(Count(c.Connections) == 2); CHECK(Count(b.Connections) == 2); CHECK(ab->NumberOfConnections == 3);
    }
    if (g_fail) { printf("00a786c0_TEST FAIL (%d)\n", g_fail); return 1; }
    puts("00a786c0_TEST PASS"); return 0;
}
