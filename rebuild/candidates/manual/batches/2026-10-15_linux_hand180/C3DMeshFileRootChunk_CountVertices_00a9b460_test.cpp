#include <cstdio>
// Sum per-sub-mesh counts over a linked walk (GetFirst/GetNext by value), as C3DMeshFileRootChunk::CountVertices.
struct Sub { unsigned int vertices; Sub* next; unsigned int CountVertices() const { return vertices; } };
struct Root { Sub* first; Sub* GetFirst() const { return first; } Sub* GetNext(Sub* s) const { return s->next; }
    unsigned int CountVertices() const { unsigned int total = 0; for (Sub* s = GetFirst(); s; s = GetNext(s)) total += s->CountVertices(); return total; } };
int main() {
    Sub c = { 5, 0 }, b = { 7, &c }, a = { 1, &b }; Root r = { &a }; Root e = { 0 };
    if (r.CountVertices() != 13 || e.CountVertices() != 0) { std::printf("00a9b460_TEST FAIL\n"); return 1; }
    std::printf("00a9b460_TEST PASS\n"); return 0; }
