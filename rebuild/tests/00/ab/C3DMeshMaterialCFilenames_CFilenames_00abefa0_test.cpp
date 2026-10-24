// Behaviour test for C3DMeshMaterial::CFilenames::~CFilenames (0x00ABEFA0): the four filenames are destroyed in reverse order, then the base.
#include <cstdio>
static int g_log[16]; static int g_n = 0; static int g_base = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() { g_log[g_n++] = 100; } unsigned long IVRefCount; };
class CFilenames : public CIVCountedPointeeBase { public: CCharString DiffuseMapFilename, BumpMapFilename, ReflectionMapFilename, IlluminationMapFilename; };
int main()
{
    { CFilenames f; f.DiffuseMapFilename.id = 1; f.BumpMapFilename.id = 2; f.ReflectionMapFilename.id = 3; f.IlluminationMapFilename.id = 4; }
    if (!(g_n == 5 && g_log[0] == 4 && g_log[1] == 3 && g_log[2] == 2 && g_log[3] == 1 && g_log[4] == 100)) { std::printf("00abefa0_TEST FAIL\n"); return 1; }
    std::printf("00abefa0_TEST PASS\n");
    return 0;
}