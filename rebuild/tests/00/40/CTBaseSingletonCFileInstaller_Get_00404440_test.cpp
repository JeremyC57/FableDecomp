#include <cstdio>

// Model of 0x00404440 CTBaseSingleton<CFileInstaller>::Get: create the CFileInstaller on first use into the static counted
// pointer (Reset 0x00403DB0 through an inline operator=) and return the object; later calls reuse it.
static int g_constructed = 0;
static int g_resets = 0;
class CFileInstaller { public:
    unsigned char m_body[0xA0];
    CFileInstaller() { ++g_constructed; m_body[0] = 0x5A; }
};
template <class T> class CCountedPointer { public:
    T* Data;
    void* Info;
    void Reset(T* object) { Data = object; ++g_resets; }
    CCountedPointer& operator=(T* object) { Reset(object); return *this; }
};
template <class T> class CTBaseSingleton { public:
    static T& Get();
    static CCountedPointer<T> Instance;
};
template <class T> CCountedPointer<T> CTBaseSingleton<T>::Instance = { 0, 0 };
template <class T> T& CTBaseSingleton<T>::Get()
{
    if (Instance.Data == 0)
        Instance = new T;
    return *Instance.Data;
}

int main()
{
    CFileInstaller* a = &CTBaseSingleton<CFileInstaller>::Get();
    CFileInstaller* b = &CTBaseSingleton<CFileInstaller>::Get();
    if (a != b || g_constructed != 1 || g_resets != 1 || a->m_body[0] != 0x5A) {
        std::printf("00404440_TEST FAIL\n");
        return 1;
    }
    delete a;
    std::printf("00404440_TEST PASS\n");
    return 0;
}
