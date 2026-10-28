// Behaviour test for CLocalDetailPrimitiveMesh::CLocalDetailPrimitiveMesh (0x00BDFB50): the local-detail base takes the collection type and cache map, with no next primitive.
#include <cstdio>
struct Type { int dummy; }; struct CacheMap { int dummy; };
struct Base { Base* Next; const Type* TypePtr; CacheMap* Map; Base(const Type* t, CacheMap* m) : Next(0), TypePtr(t), Map(m) {} };
struct Mesh : Base { Mesh(const Type* t, CacheMap* m) : Base(t, m) {} };
int main()
{
    Type t; CacheMap m; Mesh mesh(&t, &m);
    if (mesh.Next != 0 || mesh.TypePtr != &t || mesh.Map != &m) { std::printf("00bdfb50_TEST FAIL\n"); return 1; }
    std::printf("00bdfb50_TEST PASS\n");
    return 0;
}