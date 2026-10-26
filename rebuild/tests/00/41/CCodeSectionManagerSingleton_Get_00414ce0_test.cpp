#include <cstdio>

// Model of 0x00414CE0 CCodeSectionManagerSingleton::Get: create the CCodeSectionManager on first use into the static counted
// pointer (Reset 0x00413590 through an inline operator=) and return the object; later calls reuse it.
static int g_constructed = 0;
class CCodeSectionManager { public:
    unsigned char m_body[0x2C];
    CCodeSectionManager() { ++g_constructed; m_body[0] = 0x5A; }
};
struct CCountedCodeSectionManagerPointer {
    CCodeSectionManager* m_pObject;
    int m_resets;
    void Reset(CCodeSectionManager* object) { m_pObject = object; ++m_resets; }
    CCountedCodeSectionManagerPointer& operator=(CCodeSectionManager* object) { Reset(object); return *this; }
};
static CCountedCodeSectionManagerPointer g_CodeSectionManagerSingleton = { 0, 0 };
struct CCodeSectionManagerSingleton {
    static CCodeSectionManager* Get()
    {
        if (g_CodeSectionManagerSingleton.m_pObject == 0)
            g_CodeSectionManagerSingleton = new CCodeSectionManager;
        return g_CodeSectionManagerSingleton.m_pObject;
    }
};

int main()
{
    CCodeSectionManager* a = CCodeSectionManagerSingleton::Get();
    CCodeSectionManager* b = CCodeSectionManagerSingleton::Get();
    if (a == 0 || a != b || g_constructed != 1 || g_CodeSectionManagerSingleton.m_resets != 1 || a->m_body[0] != 0x5A) {
        std::printf("00414ce0_TEST FAIL\n");
        return 1;
    }
    delete a;
    std::printf("00414ce0_TEST PASS\n");
    return 0;
}