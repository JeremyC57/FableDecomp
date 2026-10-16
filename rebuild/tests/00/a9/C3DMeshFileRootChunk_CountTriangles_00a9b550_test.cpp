#include <cstdio>
// Sum per-sub-mesh counts over a linked walk (GetFirst/GetNext by value), as C3DMeshFileRootChunk::CountTriangles.
struct Sub { unsigned int triangles; Sub* next; unsigned int CountTriangles() const { return triangles; } };
struct Root { Sub* first; Sub* GetFirst() const { return first; } Sub* GetNext(Sub* s) const { return s->next; }
    unsigned int CountTriangles() const { unsigned int total = 0; for (Sub* s = GetFirst(); s; s = GetNext(s)) total += s->CountTriangles(); return total; } };
int main() {
    Sub c = { 5, 0 }, b = { 7, &c }, a = { 1, &b }; Root r = { &a }; Root e = { 0 };
    if (r.CountTriangles() != 13 || e.CountTriangles() != 0) { std::printf("00a9b550_TEST FAIL\n"); return 1; }
    std::printf("00a9b550_TEST PASS\n"); return 0; }