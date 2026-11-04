// Behaviour test for 0x00CBC130 vector<pair<unsigned __int64, CPersistInfo>>::_M_insert_overflow: a portable model of the STLport 4.6
// growth path (new capacity old + max(old, fill), head copy, fill, tail copy unless at end, old block released).
#include <cstdio>
#include <string>
#include <cstddef>

struct CPersistInfo { unsigned long long UID; std::string PersistScript; };
struct Pair { unsigned long long first; CPersistInfo second; };
struct Vec { Pair* start; Pair* finish; Pair* end; };
static void InsertOverflow(Vec& v, Pair* position, const Pair& x, size_t fill, bool atEnd) {
    size_t oldSize = v.finish - v.start;
    size_t len = oldSize + (oldSize > fill ? oldSize : fill);
    Pair* newStart = len ? static_cast<Pair*>(operator new(len * sizeof(Pair))) : 0;
    Pair* newFinish = newStart;
    for (Pair* p = v.start; p != position; ++p, ++newFinish) new (newFinish) Pair(*p);
    if (fill == 1) { new (newFinish) Pair(x); ++newFinish; }
    else for (size_t i = 0; i < fill; ++i, ++newFinish) new (newFinish) Pair(x);
    if (!atEnd) for (Pair* p = position; p != v.finish; ++p, ++newFinish) new (newFinish) Pair(*p);
    for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    if (v.start) operator delete(v.start);
    v.start = newStart; v.finish = newFinish; v.end = newStart + len;
}
int main() {
    Vec v = { 0, 0, 0 };
    Pair a = { 1, { 10, "one" } }, b = { 2, { 20, "two" } }, c = { 3, { 30, "three" } };
    InsertOverflow(v, v.finish, a, 1, true);                  // empty: capacity 1
    bool ok = v.finish - v.start == 1 && v.end - v.start == 1 && v.start[0].second.PersistScript == "one";
    InsertOverflow(v, v.finish, b, 1, true);                  // 1 + max(1, 1) = 2
    ok = ok && v.end - v.start == 2 && v.start[1].first == 2;
    InsertOverflow(v, v.start + 1, c, 3, false);              // 2 + max(2, 3) = 5: head, fill x3, tail
    ok = ok && v.finish - v.start == 5 && v.end - v.start == 5 && v.start[0].first == 1 && v.start[1].second.UID == 30
         && v.start[3].second.PersistScript == "three" && v.start[4].second.PersistScript == "two";
    for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    operator delete(v.start);
    std::printf(ok ? "00cbc130_TEST PASS\n" : "00cbc130_TEST FAIL\n");
    return ok ? 0 : 1;
}