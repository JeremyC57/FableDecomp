// Behaviour test for CTBaseSingleton<CUserProfileManager>::Get (0x0040D2A0): creates once, then returns the same object.
#include <cstdio>

static int g_constructed = 0;
struct CUserProfileManager {
    int m_id;
    CUserProfileManager() { m_id = ++g_constructed; }
};

template <class T>
struct CCountedPointer {
    T* m_pObject;
    void* m_pCount;
    void operator=(T* object) { m_pObject = object; m_pCount = 0; }
};

template <class T>
struct CTBaseSingleton {
    static CCountedPointer<T> s_instance;
    static T* Get();
};

template <class T> CCountedPointer<T> CTBaseSingleton<T>::s_instance;

template <>
CUserProfileManager* CTBaseSingleton<CUserProfileManager>::Get()
{
    if (s_instance.m_pObject == 0)
        s_instance = new CUserProfileManager;
    return s_instance.m_pObject;
}

int main()
{
    CTBaseSingleton<CUserProfileManager>::s_instance.m_pObject = 0;
    CUserProfileManager* a = CTBaseSingleton<CUserProfileManager>::Get();
    CUserProfileManager* b = CTBaseSingleton<CUserProfileManager>::Get();
    if (a == 0 || a != b || g_constructed != 1 || a->m_id != 1) {
        std::printf("0040d2a0_TEST FAIL\n");
        return 1;
    }
    delete a;
    std::printf("0040d2a0_TEST PASS\n");
    return 0;
}
