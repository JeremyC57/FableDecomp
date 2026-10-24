#include <cstdio>
#include <cstring>

class CCharString {                                    // stand-in: keeps the text pointer
public:
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    CCharString(const CCharString& other) : Text(other.Text) {}
    ~CCharString() {}
    const char* Text;
};

class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    bool IsNull() const { return Data == 0; }
    T& operator*() const { return *Data; }
    void Reset(T* data) { Data = data; ++Resets; }
    T*              Data;
    CCPPointerInfo* Info;
    static int Resets;
};
template <class T> int CCountedPointer<T>::Resets = 0;

class CCodeSectionManager {                             // stand-in recording the calls
public:
    CCodeSectionManager() : Loads(0), Unloads(0), Registers(0), LastHandle(0), LoadNow(true), CanUnload(false)
    { ++Constructed; ShortName[0] = LongName[0] = 0; }
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload)
    {
        ++Registers;
        std::strcpy(ShortName, shortName.Text);
        std::strcpy(LongName, longName.Text);
        LoadNow = loadNow;
        CanUnload = canUnload;
        return 0x5EC70000u + Registers;
    }
    void LoadSection(unsigned long handle) { ++Loads; LastHandle = handle; }
    void UnloadSection(unsigned long handle) { ++Unloads; LastHandle = handle; }
    int Loads, Unloads, Registers;
    unsigned long LastHandle;
    bool LoadNow, CanUnload;
    char ShortName[32], LongName[64];
    static int Constructed;
};
int CCodeSectionManager::Constructed = 0;

template <class T> class CTBaseSingleton {
public:
    static T& Get()
    {
        if (Instance.IsNull())
            Instance.Reset(new T);
        return *Instance;
    }
    static CCountedPointer<T> Instance;
};
template <class T> CCountedPointer<T> CTBaseSingleton<T>::Instance = { 0, 0 };

class CXMVPlayer {
public:
    static void StaticLoadXMVCode()
    {
        if (XMVCodeLoadCount == 0)
            CTBaseSingleton<CCodeSectionManager>::Get().LoadSection(XMVCodeSection);
        ++XMVCodeLoadCount;
    }
    static void StaticReleaseXMVCode()
    {
        if (--XMVCodeLoadCount == 0)
            CTBaseSingleton<CCodeSectionManager>::Get().UnloadSection(XMVCodeSection);
        if (XMVCodeLoadCount < 0)
            XMVCodeLoadCount = 0;
    }
    static void GlobalRegisterPlayer()
    {
        XMVCodeSection = CTBaseSingleton<CCodeSectionManager>::Get().RegisterSection("XMV", "Xbox Movie Player Code", false, true);
        PlayerRegistered = true;
    }
    static unsigned long XMVCodeSection;
    static long          XMVCodeLoadCount;
    static bool          PlayerRegistered;
};
unsigned long CXMVPlayer::XMVCodeSection = 0;
long CXMVPlayer::XMVCodeLoadCount = 0;
bool CXMVPlayer::PlayerRegistered = false;

static CCodeSectionManager& Mgr() { return *CTBaseSingleton<CCodeSectionManager>::Instance.Data; }

int main()
{
    CXMVPlayer::XMVCodeSection = 0x77;
    CXMVPlayer::XMVCodeLoadCount = 2;
    CXMVPlayer::StaticReleaseXMVCode();                      // 2 -> 1: still loaded (the manager is still created by nobody)
    if (CXMVPlayer::XMVCodeLoadCount != 1 || CCodeSectionManager::Constructed != 0) { std::printf("009bcfc0_TEST FAIL\n"); return 1; }
    CXMVPlayer::StaticReleaseXMVCode();                      // 1 -> 0: unload through the singleton
    if (CXMVPlayer::XMVCodeLoadCount != 0 || CCodeSectionManager::Constructed != 1 || Mgr().Unloads != 1 || Mgr().LastHandle != 0x77) {
        std::printf("009bcfc0_TEST FAIL\n"); return 1;
    }
    CXMVPlayer::StaticReleaseXMVCode();                      // unbalanced release: clamps at 0, no unload
    if (CXMVPlayer::XMVCodeLoadCount != 0 || Mgr().Unloads != 1) { std::printf("009bcfc0_TEST FAIL\n"); return 1; }
    std::printf("009bcfc0_TEST PASS\n");
    return 0;
}