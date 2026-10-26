#include <cstdio>

// Behaviour model of CGameJoystickManager::Get() (0x0049BB90): lazy singleton held by the static counted pointer PClass.
static int g_news = 0;

struct CBase {
    virtual ~CBase() { }
    CBase() { }
};

template <class T>
class CCountedPointer { public:
    struct SBlock { long refs; T* obj; };
    T*      m_pObject;
    SBlock* m_pCount;
    T* Get() const { return m_pObject; }
    T& operator*() const { return *m_pObject; }
    void Reset(T* object)
    {
        if (m_pCount) {
            --m_pCount->refs;
            if (m_pCount->refs == 0) { delete m_pCount->obj; delete m_pCount; }
        }
        m_pCount = 0;
        m_pObject = object;
        if (object) { m_pCount = new SBlock; m_pCount->refs = 1; m_pCount->obj = object; }
    }
};

class CGameJoystickManager : public CBase { public:
    long m_MainPlayerJoystickDeviceNumber;
    bool m_MainPlayerJoystickDeviceNumberSet;
    CGameJoystickManager() : m_MainPlayerJoystickDeviceNumber(0), m_MainPlayerJoystickDeviceNumberSet(false) { ++g_news; }
    static CCountedPointer<CGameJoystickManager> PClass;
    static CGameJoystickManager& Get()
    {
        if (!PClass.Get())
            PClass.Reset(new CGameJoystickManager);
        return *PClass;
    }
};

CCountedPointer<CGameJoystickManager> CGameJoystickManager::PClass = { 0, 0 };

int main()
{
    if (CGameJoystickManager::PClass.Get() != 0) { std::printf("0049bb90_TEST FAIL\n"); return 1; }
    CGameJoystickManager& a = CGameJoystickManager::Get();
    if (g_news != 1 || &a != CGameJoystickManager::PClass.Get() || CGameJoystickManager::PClass.m_pCount == 0) { std::printf("0049bb90_TEST FAIL\n"); return 1; }
    if (a.m_MainPlayerJoystickDeviceNumber != 0 || a.m_MainPlayerJoystickDeviceNumberSet) { std::printf("0049bb90_TEST FAIL\n"); return 1; }
    a.m_MainPlayerJoystickDeviceNumber = 3; a.m_MainPlayerJoystickDeviceNumberSet = true;
    CGameJoystickManager& b = CGameJoystickManager::Get();          // second call: same object, no new
    if (&b != &a || g_news != 1 || b.m_MainPlayerJoystickDeviceNumber != 3) { std::printf("0049bb90_TEST FAIL\n"); return 1; }
    CGameJoystickManager::PClass.Reset(0);                           // dropped: the next Get builds a fresh one
    CGameJoystickManager& c = CGameJoystickManager::Get();
    if (g_news != 2 || c.m_MainPlayerJoystickDeviceNumber != 0) { std::printf("0049bb90_TEST FAIL\n"); return 1; }
    std::printf("0049bb90_TEST PASS\n");
    return 0;
}