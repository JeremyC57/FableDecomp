// Behaviour test for CTCBase::Clone (0x00686FA0): the registered allocator for the class name builds a component for the same
// owner, which then copies this one; the inline name compare treats null strings as smallest.
#include <cstdio>
#include <cstring>
struct Thing { int id; };
struct TC;
typedef TC* (*AllocFn)(Thing& owner);
struct Str { const char* P; bool operator<(const Str& o) const {
    if (o.P == P) return false; if (!P) return true; if (!o.P) return false; return std::strcmp(P, o.P) < 0; } };
struct Pair { Str first; AllocFn Alloc; };
static Pair g_map[3]; static int g_count;
static Pair* LowerBound(const Str& k) { int i = 0; while (i < g_count && g_map[i].first < k) ++i; return g_map + i; }
struct TC {
    Thing* ParentThing; const char* name; const TC* copiedFrom;
    Str GetClassTypeName() const { Str s; s.P = name; return s; }
    void CopyFrom(const TC& o) { copiedFrom = &o; }
    static TC* AllocTC(const Str& n, Thing& owner)
    {
        Pair* it = LowerBound(n);
        if (it != g_map + g_count && !(n < it->first)) return it->Alloc(owner);
        return 0;
    }
    TC* Clone() { Thing* owner = ParentThing; TC* clone = AllocTC(GetClassTypeName(), *owner); clone->CopyFrom(*this); return clone; }
};
static TC g_made; static Thing* g_owner;
static TC* MakeDoor(Thing& o) { g_owner = &o; g_made.name = "CTCDoor"; return &g_made; }
static TC* MakeOther(Thing&) { return 0; }
int main()
{
    g_map[0].first.P = "CTCBed"; g_map[0].Alloc = MakeOther; g_map[1].first.P = "CTCDoor"; g_map[1].Alloc = MakeDoor; g_count = 2;
    Thing t; TC door; door.ParentThing = &t; door.name = "CTCDoor"; door.copiedFrom = 0;
    TC* c = door.Clone();
    if (c != &g_made || g_owner != &t || g_made.copiedFrom != &door) { std::printf("00686fa0_TEST FAIL\n"); return 1; }
    Str a; a.P = 0; Str b; b.P = "A";
    if (!(a < b) || b < a || a < a) { std::printf("00686fa0_TEST FAIL\n"); return 1; }
    std::printf("00686fa0_TEST PASS\n");
    return 0;
}
