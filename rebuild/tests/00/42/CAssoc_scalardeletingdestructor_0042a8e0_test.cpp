// Behaviour test for 0x0042A8E0 (NF230) on the shared headers engine/NUISystem_CManager.h / engine/CCountedPointer.h: the scalar deleting
// destructor of NUISystem::CManager's map value type pair<const CCharString, CCountedPointer<CComponent> >, reached through `delete` of a
// heap pair: the pair is destroyed (its key destructor runs once), its storage goes to operator delete exactly once with the pair's own
// address, and deleting a null pointer destroys and frees nothing. Stubbed: the out-of-line CCharString constructors / destructor
// (counted); operator new / delete are counted wrappers of malloc / free. The null counted pointer's release is the header's inline ReleaseData.
#define FABLE_RETAIL_CCHARSTRING   // test only: the retail CCharString (PStringData) as the binder rows 0x0042B7EE / 0x0042B76C use
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include <stdio.h>
#include <stdlib.h>
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
static int g_live = 0, g_news = 0, g_deletes = 0;
static void* g_last_deleted = 0;
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)text; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_live; }
CCharString::~CCharString() { --g_live; }
void* operator new(size_t n) { ++g_news; return malloc(n); }
void operator delete(void* p) { if (p) { ++g_deletes; g_last_deleted = p; } free(p); }
typedef std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > ChildPair;
int main()
{
    {
        CCharString key("Child", -1);
        CCountedPointer<NUISystem::CComponent> none;
        ChildPair* p = new ChildPair(key, none);
        Check(p != 0 && g_news == 1 && g_live == 2, "heap pair holds a live key copy");
        Check(p->second.Data == 0, "the counted pointer is null");
        void* at = p;
        delete p;
        Check(g_live == 1, "delete destroys the pair's key once");
        Check(g_deletes == 1 && g_last_deleted == at, "delete frees the pair's own storage once");
        ChildPair* q = 0;
        delete q;
        Check(g_live == 1 && g_deletes == 1, "deleting a null pair destroys and frees nothing");
    }
    Check(g_live == 0, "nothing left");
    printf(g_fails ? "FAIL_0042a8e0\n" : "ULTRA2_0042a8e0_CASSOC_SDD PASS\n");
    return g_fails ? 1 : 0;
}
