#include <cstdio>
#include <cstring>
struct Chunk { long unknown; char type[5]; Chunk(long u, const char* t) : unknown(u) { std::strcpy(type, t); } long IsUnknownType() const { return unknown; } };
struct Block { long refs; };
struct Ptr { Chunk* Object; Block* Count; Ptr() : Object(0), Count(0) {} Ptr(Chunk* o, Block* b) : Object(o), Count(b) {}
  Ptr(const Ptr& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->refs; } };
struct Super { Ptr* first; Ptr* last; unsigned long cur;
  unsigned long size() const { return last - first; }
  Ptr FindFirst(const char* type) {
    for (unsigned long i = 0; i < size(); ++i) { Ptr& c = first[i];
      if (!c.Object->IsUnknownType() && std::strcmp(type, c.Object->type) == 0) { cur = i; return c; } }
    return Ptr(); } };
int main()
{
    Chunk a(0, "MESH"), b(1, "BONE"), c(0, "BONE"), d(0, "BONE");
    Block ba = {1}, bb = {1}, bc = {1}, bd = {1};
    Ptr arr[4] = { Ptr(&a, &ba), Ptr(&b, &bb), Ptr(&c, &bc), Ptr(&d, &bd) };
    Super s; s.first = arr; s.last = arr + 4; s.cur = 99;
    Ptr r = s.FindFirst("BONE");
    Ptr n = s.FindFirst("XXXX");
    if (r.Object != &c || s.cur != 2 || bc.refs != 2 || n.Object != 0 || n.Count != 0 || s.cur != 2) { std::printf("00aa80e0_TEST FAIL\n"); return 1; }
    std::printf("00aa80e0_TEST PASS\n");
    return 0;
}