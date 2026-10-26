#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: template <class T> void Transfer(T& value); };
template <> void CPersistContext::Transfer<bool>(bool& value);
template <> void CPersistContext::Transfer<bool>(bool& value_)
{
    void* sub = (void*)&value_; (void)sub; g_sub=sub; }
struct S { char pad[0x25]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Transfer(*(bool*)(reinterpret_cast<char*>(this) + 0x25)); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x25){ std::printf("004dd989_TEST FAIL\n"); return 1;}
 std::printf("004dd989_TEST PASS\n"); return 0;}