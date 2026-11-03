// Behaviour test for CUserProfileManager::IsEnoughSpaceForProfile at 0x004067C0: asks CAFile::IsRoomForFile for 0x53C000
// bytes in the user's data folder and returns its answer; the folder string is a temporary destroyed afterwards.
#include <cstdio>
static int g_dtorCalls = 0;
static long g_lastSize = 0;
static const char* g_lastPath = 0;
class CWideString { public:
    const char* m_text;
    explicit CWideString(const char* text) : m_text(text) { }
    CWideString(const CWideString& o) : m_text(o.m_text) { }
    ~CWideString() { ++g_dtorCalls; }
};
namespace NResourceDirectoryNames { CWideString GetUserDataDir() { return CWideString("C:\\Docs\\My Games\\Fable\\"); } }
static bool g_room = true;
class CAFile { public:
    static bool IsRoomForFile(long size, const CWideString& path) { g_lastSize = size; g_lastPath = path.m_text; return g_room; }
};
class CUserProfileManager { public:
    bool IsEnoughSpaceForProfile() const;
};
bool CUserProfileManager::IsEnoughSpaceForProfile() const
{
    return CAFile::IsRoomForFile(0x53C000, NResourceDirectoryNames::GetUserDataDir());
}
int main()
{
    CUserProfileManager m;
    bool a = m.IsEnoughSpaceForProfile();
    int d1 = g_dtorCalls;
    g_room = false;
    bool b = m.IsEnoughSpaceForProfile();
    if (a && !b && d1 >= 1 && g_lastSize == 0x53C000 && g_lastPath && g_lastPath[0] == 'C') {
        std::printf("CUserProfileManager_004067c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
