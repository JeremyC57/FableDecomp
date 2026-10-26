// Behaviour test for 00cb8d30: CCountedPointer<CEntityScriptBindingBase>::DeleteData -- the block goes to operator delete exactly once
#include <cstdio>
#include <cstdlib>
static int g_deletes = 0; static void* g_last = 0;
void operator delete(void* p) { ++g_deletes; g_last = p; std::free(p); }
class CEntityScriptBindingBase { public: int m_word; };
template <class T> class CCountedPointer { public: static void __fastcall DeleteData(void* data) { delete (T*)data; } };
int main()
{
    CEntityScriptBindingBase* b = new CEntityScriptBindingBase; void* raw = b;
    CCountedPointer<CEntityScriptBindingBase>::DeleteData(raw);
    int ok = g_deletes == 1 && g_last == raw;
    std::printf(ok ? "PASS_00cb8d30\n" : "FAIL_00cb8d30\n");
    return ok ? 0 : 1;
}
