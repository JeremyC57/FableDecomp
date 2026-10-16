// behaviour test for vector<pair<unsigned long, C3DMeshVolume> >::resize(n) (0x00A35580)
#include <cstdio>
static int g_strings = 0, g_released = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } ~CCharString() { --g_strings; } };
struct SCountBlock { int m_references; };
struct CCountedPointer { void* Data; SCountBlock* m_pCount; CCountedPointer() : Data(0), m_pCount(0) {}
    ~CCountedPointer() { if (m_pCount && --m_pCount->m_references == 0) ++g_released; Data = 0; m_pCount = 0; } };
struct C3DMeshVolume { CCharString Name; CCountedPointer ConvexVolume; };
struct SMeshVolumePair { unsigned int first; C3DMeshVolume second; SMeshVolumePair() : first(0), second() {} };
static unsigned int g_n = 0; static bool g_ok = false;
struct CMeshVolumeVector {
    void resize(unsigned int n, const SMeshVolumePair& v) { g_n = n; g_ok = v.first == 0 && v.second.ConvexVolume.Data == 0 && v.second.ConvexVolume.m_pCount == 0 && g_strings == 1; }
    void resize(unsigned int n) { resize(n, SMeshVolumePair()); }
};
int main()
{
    CMeshVolumeVector v; v.resize(3);
    if (g_n != 3 || !g_ok || g_strings != 0 || g_released != 0) { std::printf("00a35580_TEST FAIL\n"); return 1; }
    std::printf("00a35580_TEST PASS\n");
    return 0;
}