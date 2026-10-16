#include <cstdio>
#include <cstring>
struct Chunk { long unknown; char type[5]; Chunk(long u, const char* t) : unknown(u) { std::strcpy(type, t); } long IsUnknownType() const { return unknown; } };
struct Block { long refs; };
struct Ptr { Chunk* Object; Block* Count; Ptr() : Object(0), Count(0) {} Ptr(Chunk* o, Block* b) : Object(o), Count(b) {}
  Ptr(const Ptr& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->refs; } };
struct Super { Ptr* first; Ptr* last; unsigned long cur;
  Ptr FindNext(const Ptr& prev) {
    unsigned long count = last - first, i;
    if (cur < count && first[cur].Object == prev.Object) i = cur;
    else { for (i = 0; i < count; ++i) if (first[i].Object == prev.Object) break; }
    const char* type = prev.Object->type;
    for (++i; i < count; ++i) { Ptr& c = first[i];
      if (!c.Object->IsUnknownType() && std::strcmp(type, c.Object->type) == 0) { cur = i; return c; } }
    return Ptr(); } };
int main()
{
    Chunk a(0, "BONE"), b(1, "BONE"), c(0, "MESH"), d(0, "BONE");
    Block ba = {1}, bb = {1}, bc = {1}, bd = {1};
    Ptr arr[4] = { Ptr(&a, &ba), Ptr(&b, &bb), Ptr(&c, &bc), Ptr(&d, &bd) };
    Super s; s.first = arr; s.last = arr + 4; s.cur = 0;
    Ptr r = s.FindNext(arr[0]);           // fast path via cur
    bool ok = r.Object == &d && s.cur == 3 && bd.refs == 2;
    s.cur = 77;
    Ptr r2 = s.FindNext(arr[2]);          // linear scan, no later MESH
    ok = ok && r2.Object == 0 && s.cur == 77;
    s.cur = 77;
    Ptr r3 = s.FindNext(arr[0]);          // linear scan finds a at 0
    ok = ok && r3.Object == &d && s.cur == 3;
    if (!ok) { std::printf("00aa7ff0_TEST FAIL\n"); return 1; }
    std::printf("00aa7ff0_TEST PASS\n");
    return 0;
}
