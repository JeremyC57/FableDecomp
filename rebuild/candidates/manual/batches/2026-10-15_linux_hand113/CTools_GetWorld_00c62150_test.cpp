#include <cstdio>
// Behaviour test for CTools::GetWorld (0x00C62150): follows [ref] -> +4 -> +0 -> +4 -> +0x38.
struct CWorld { int id; };
struct CWorldRecord { int pad; CWorld* m_pWorld; };
struct CWorldLink { void* m_first; CWorldRecord* m_pRecord; };
struct CWorldLinkHolder { CWorldLink* m_pLink; };
struct CWorldOwner { void* m_first; CWorldLinkHolder* m_pHolder; };
struct CWorldOwnerRef { CWorldOwner* m_pOwner; };
static CWorld* GetWorld(const CWorldOwnerRef& ref) { return ref.m_pOwner->m_pHolder->m_pLink->m_pRecord->m_pWorld; }
int main()
{
    CWorld w; w.id = 7;
    CWorldRecord rec; rec.m_pWorld = &w;
    CWorldLink link; link.m_pRecord = &rec;
    CWorldLinkHolder h; h.m_pLink = &link;
    CWorldOwner o; o.m_pHolder = &h;
    CWorldOwnerRef r; r.m_pOwner = &o;
    if (GetWorld(r) != &w || GetWorld(r)->id != 7) { std::printf("00c62150_TEST FAIL\n"); return 1; }
    std::printf("00c62150_TEST PASS\n");
    return 0;
}
