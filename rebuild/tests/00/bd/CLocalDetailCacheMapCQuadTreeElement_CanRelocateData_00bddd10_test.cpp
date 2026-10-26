// Behaviour test for 00bddd10 CQuadTreeElement::CanRelocateData (copy of the model).
#include <cstdio>
class CLocalDetailCacheMap { public: bool LockedFromMemoryRelocation; void* FileBlockManager; };
struct CQuadTreeElement {
    virtual ~CQuadTreeElement() {}
    void* ObjectCacheGroupCollections;
    CQuadTreeElement* Children[4];
    unsigned char BoundingSphere[0x10];
    CLocalDetailCacheMap* Map;
    bool CanRelocateData(long blockType) const;
};
bool CQuadTreeElement::CanRelocateData(long blockType) const
{
    return !Map->LockedFromMemoryRelocation;
}
int main()
{
    CLocalDetailCacheMap map; map.LockedFromMemoryRelocation = false;
    CQuadTreeElement e; e.Map = &map;
    int ok = 1;
    if (!e.CanRelocateData(0) || !e.CanRelocateData(3)) ok = 0;
    map.LockedFromMemoryRelocation = true;
    if (e.CanRelocateData(0)) ok = 0;
    if (!ok) { std::printf("00bddd10_TEST FAIL\n"); return 1; }
    std::printf("00bddd10_TEST PASS\n");
    return 0;
}