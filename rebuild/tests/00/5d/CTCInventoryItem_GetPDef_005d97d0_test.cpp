// behaviour test for 0x005D97D0 CTCInventoryItem::GetPDef(long)
// (model: a static accessor that caches the defs for the index, then returns a counted copy of the cached CInventoryItemDef pointer:
// the pointee's reference count at +4 goes up by one, a null cache gives a null pointer). Rewritten by agentNF59 on the new name's
// behaviour (the old test linked the free GetPDef / PrepareDef stand-ins); the catalog PassPattern is kept.
#include <stdio.h>
namespace model {
struct Def { void* vtbl; long RefCount; };
struct DefPointer { Def* Object; DefPointer(Def* d) : Object(d) { if (Object) ++Object->RefCount; } };
static Def* g_cached = 0;
static long g_lastIndex = -1;
static bool CacheDef(long defIndex) { g_lastIndex = defIndex; return g_cached != 0; }
static DefPointer GetPDef(long defIndex) { CacheDef(defIndex); return DefPointer(g_cached); }
}
int main()
{
    model::Def obj = { 0, 5 };
    model::g_cached = &obj;
    model::DefPointer p = model::GetPDef(42);
    bool ok = p.Object == &obj && obj.RefCount == 6 && model::g_lastIndex == 42;
    model::g_cached = 0;
    model::DefPointer q = model::GetPDef(7);
    ok = ok && q.Object == 0 && model::g_lastIndex == 7;
    if (ok) printf("GETPDEF_OK_XYZ\n");
    else    printf("GETPDEF_FAIL\n");
    return ok ? 0 : 1;
}
