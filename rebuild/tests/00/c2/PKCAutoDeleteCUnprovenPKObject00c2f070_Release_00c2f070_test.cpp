// behaviour test for 0x00C2F070 PKCAutoDelete<T>::Release() (model; the string is a std::string, the hook records the text)
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_msg; static int g_break = 0;
struct T { int v; };
struct AutoDelete {
    T* m_pObject;
    T* Release()
    {
        if (m_pObject == 0) {
            g_msg = std::string("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h") + "(" + "42" + ") : " + "Shouls have checked first..." + "\n";
            ++g_break;
        }
        T* object = m_pObject;
        m_pObject = 0;
        return object;
    }
};
int main()
{
    T t; AutoDelete a; a.m_pObject = &t;
    int ok = a.Release() == &t && a.m_pObject == 0 && g_break == 0;
    ok = ok && a.Release() == 0 && g_break == 1 && g_msg.find("(42) : Shouls") != std::string::npos;
    if (!ok) { std::printf("00c2f070_TEST FAIL\n"); return 1; }
    std::printf("00c2f070_TEST PASS\n");
    return 0;
}