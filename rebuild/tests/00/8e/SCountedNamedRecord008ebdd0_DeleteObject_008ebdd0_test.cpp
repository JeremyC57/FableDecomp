#include <cstdio>
static int g_stringDtors;
struct CCharString { const char* m_text; CCharString() : m_text("") { } ~CCharString() { ++g_stringDtors; } };
struct SCountedNamedRecord_008ebdd0 {
    long m_key; CCharString m_name;
    static void DeleteObject(SCountedNamedRecord_008ebdd0* object)
    {
        if (object) { object->~SCountedNamedRecord_008ebdd0(); operator delete(object); }
    }
};
int main()
{
    SCountedNamedRecord_008ebdd0::DeleteObject(0);
    if (g_stringDtors != 0) { std::printf("008ebdd0_TEST FAIL\n"); return 1; }
    SCountedNamedRecord_008ebdd0* r = new SCountedNamedRecord_008ebdd0; r->m_key = 4;
    SCountedNamedRecord_008ebdd0::DeleteObject(r);
    if (g_stringDtors != 1) { std::printf("008ebdd0_TEST FAIL\n"); return 1; }
    std::printf("008ebdd0_TEST PASS\n");
    return 0;
}