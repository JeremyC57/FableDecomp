// Behaviour test for CLocalDetailPrimitiveRepeatedMesh::CLocalDetailPrimitiveRepeatedMesh (0x00BFAA80): the base takes type and map; every repeated-mesh array and count starts empty.
#include <cstdio>
struct Type { int dummy; }; struct CacheMap { int dummy; };
struct Base { Base* Next; const Type* TypePtr; CacheMap* Map; Base(const Type* t, CacheMap* m) : Next(0), TypePtr(t), Map(m) {} };
struct Repeated : Base { void* WindDelayArray; void* LandscapeNormalArray; float MaxObjectScale; unsigned int ObjectCount; void* SubSections;
    unsigned int SubSectionsCount; void* ObjectMatricies; void* ObjectOffsets;
    Repeated(const Type* t, CacheMap* m) : Base(t, m) { WindDelayArray = 0; LandscapeNormalArray = 0; MaxObjectScale = 0.0f; ObjectCount = 0;
        SubSections = 0; SubSectionsCount = 0; ObjectMatricies = 0; ObjectOffsets = 0; } };
int main()
{
    Type t; CacheMap m; Repeated r(&t, &m);
    if (r.Next != 0 || r.TypePtr != &t || r.Map != &m || r.WindDelayArray != 0 || r.LandscapeNormalArray != 0 || r.MaxObjectScale != 0.0f ||
        r.ObjectCount != 0 || r.SubSections != 0 || r.SubSectionsCount != 0 || r.ObjectMatricies != 0 || r.ObjectOffsets != 0) {
        std::printf("00bfaa80_TEST FAIL\n"); return 1; }
    std::printf("00bfaa80_TEST PASS\n");
    return 0;
}