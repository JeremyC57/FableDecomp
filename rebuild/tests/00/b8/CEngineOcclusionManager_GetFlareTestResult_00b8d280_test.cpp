// behaviour test for 00b8d280 (standalone copy of the model)
#include <cstdio>

struct COccluderFlareEntry { float Position[3]; float Radius; unsigned int CircleSegments; unsigned int MaxPixelCount; unsigned int OccluderHandle; };
struct CArr { COccluderFlareEntry* _M_start; COccluderFlareEntry* _M_finish; COccluderFlareEntry* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } COccluderFlareEntry& operator[](unsigned int i) { return _M_start[i]; } };
class CEngineOcclusionManager { public: CArr FlareTests;
    float Get(unsigned long handle) {
        float result = 0.0f;
        if (handle < FlareTests.size()) { COccluderFlareEntry& e = FlareTests[handle]; result = 0.0f / (float)e.MaxPixelCount; if (result > 1.0f) result = 1.0f; }
        return result; } };

int main()
{
    COccluderFlareEntry e[2]; e[0].MaxPixelCount = 100; e[1].MaxPixelCount = 7;
    CEngineOcclusionManager m; m.FlareTests._M_start = e; m.FlareTests._M_finish = e + 2; m.FlareTests._M_end_of_storage = e + 2;
    if (!(m.Get(0) == 0.0f && m.Get(1) == 0.0f && m.Get(2) == 0.0f && m.Get(1000) == 0.0f)) { std::printf("00b8d280_TEST FAIL\n"); return 1; }
    std::printf("00b8d280_TEST PASS\n");
    return 0;
}