// Behaviour test for C3DMeshFileEngineMaterialChunk::CMapFilenames::~CMapFilenames (0x00AC6210): the four map names are destroyed in reverse order, then the base.
#include <cstdio>
static int g_log[16]; static int g_n = 0; static int g_base = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() { g_log[g_n++] = 100; } unsigned long IVRefCount; };
class CMapFilenames : public CIVCountedPointeeBase { public: CCharString DiffuseMap, BumpMap, ReflectionMap, IlluminationMap; };
int main()
{
    { CMapFilenames f; f.DiffuseMap.id = 1; f.BumpMap.id = 2; f.ReflectionMap.id = 3; f.IlluminationMap.id = 4; }
    if (!(g_n == 5 && g_log[0] == 4 && g_log[1] == 3 && g_log[2] == 2 && g_log[3] == 1 && g_log[4] == 100)) { std::printf("00ac6210_TEST FAIL\n"); return 1; }
    std::printf("00ac6210_TEST PASS\n");
    return 0;
}