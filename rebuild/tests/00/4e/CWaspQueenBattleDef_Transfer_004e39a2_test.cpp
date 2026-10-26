#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: template <class T> void Transfer(T& value); };
template <> void CPersistContext::Transfer<long>(long& value);
template <> void CPersistContext::Transfer<long>(long& value_)
{
    void* sub = (void*)&value_; (void)sub; g_sub=sub; }
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x3c)); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x3c){ std::printf("004e39a2_TEST FAIL\n"); return 1;}
 std::printf("004e39a2_TEST PASS\n"); return 0;}