// Behaviour test for 0x00A6CA00 CPhysicsMesh::GetDummyObjectPositionStatic(const CCharString&, long) const
// Model: LowerBound(name) on the dummy-object map, then the index-th entry from there; returns its C3DDummyObject.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* PData; };
class C3DDummyObject { public: float Transform[12]; long BoneIndex; };
struct Entry { CCharString first; C3DDummyObject second; };
class TestMap {
public:
    Entry* _M_start;
    Entry* _M_finish;
    const Entry* LowerBound(const CCharString& key) const
    {
        int count = (int)(_M_finish - _M_start);
        const Entry* first = _M_start;
        while (count > 0) {
            int half = count / 2;
            const Entry* middle = first + half;
            if (std::strcmp(middle->first.PData, key.PData) < 0) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
};
class CPhysicsMesh {
public:
    TestMap DummyObjects;
    const C3DDummyObject& GetDummyObjectPositionStatic(const CCharString& name, long index) const
    {
        return DummyObjects.LowerBound(name)[index].second;
    }
};
int main()
{
    Entry e[3];
    const char* names[3] = { "alpha", "beta", "gamma" };
    for (int i = 0; i < 3; ++i) { e[i].first.PData = names[i]; e[i].second.BoneIndex = 10 + i; }
    CPhysicsMesh mesh;
    mesh.DummyObjects._M_start = e; mesh.DummyObjects._M_finish = e + 3;
    CCharString key; key.PData = "beta";
    if (mesh.GetDummyObjectPositionStatic(key, 0).BoneIndex != 11) { std::printf("00a6ca00_TEST FAIL\n"); return 1; }
    if (mesh.GetDummyObjectPositionStatic(key, 1).BoneIndex != 12) { std::printf("00a6ca00_TEST FAIL\n"); return 1; }
    key.PData = "a";
    if (&mesh.GetDummyObjectPositionStatic(key, 0) != &e[0].second) { std::printf("00a6ca00_TEST FAIL\n"); return 1; }
    std::printf("00a6ca00_TEST PASS\n");
    return 0;
}