#include <cstdio>
#include <new>
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                 // 0x00413520 for CConsole
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
    static CCountedPointer<T> Instance;                  // 0x013CAA40 for CConsole
    static T& __fastcall Get()
    {
        if (!Instance.Data)
            Instance.Reset(new T);
        return *Instance.Data;
    }
};
class CConsole : public CTBaseSingleton<CConsole> {
public:
    CConsole();                                          // 0x009ECD80
    unsigned char _body[0x9c - 4];
};
template CConsole& __fastcall CTBaseSingleton<CConsole>::Get();

static int g_ctor = 0, g_reset = 0;
CConsole::CConsole() { ++g_ctor; }
template <class T> void CCountedPointer<T>::Reset(T* data) { ++g_reset; Data = data; Info = 0; }
template <class T> CCountedPointer<T> CTBaseSingleton<T>::Instance;
int main()
{
    CConsole& a = CTBaseSingleton<CConsole>::Get();
    CConsole& b = CTBaseSingleton<CConsole>::Get();
    bool ok = &a == &b && g_ctor == 1 && g_reset == 1 && CTBaseSingleton<CConsole>::Instance.Data == &a;
    if (ok) std::printf("00414c90_TEST PASS\n");
    return 0;
}