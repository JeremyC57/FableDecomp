#include <cstdio>
#include <cstring>
#include <new>

static int g_baseRuns = 0;
class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() { ++g_baseRuns; } };
class CGameJoystickManager : public CBaseClassNonCopyable { public:
    long m_MainPlayerJoystickDeviceNumber; bool m_MainPlayerJoystickDeviceNumberSet;
    CGameJoystickManager() : m_MainPlayerJoystickDeviceNumber(0), m_MainPlayerJoystickDeviceNumberSet(false) {}
};
int main()
{
    unsigned char raw[sizeof(CGameJoystickManager)]; std::memset(raw, 0xaa, sizeof raw);
    CGameJoystickManager* m = new (raw) CGameJoystickManager;
    if (g_baseRuns != 1 || m->m_MainPlayerJoystickDeviceNumber != 0 || m->m_MainPlayerJoystickDeviceNumberSet) { std::printf("0049bb60_TEST FAIL\n"); return 1; }
    m->~CGameJoystickManager();
    std::printf("0049bb60_TEST PASS\n");
    return 0;
}