#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: template <class T> void Transfer(T& value); };
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value);
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value_)
{
    void* sub = (void*)&value_; (void)sub; g_sub=sub; }
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Transfer(*(unsigned long*)(reinterpret_cast<char*>(this) + 0x28)); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x28){ std::printf("004de8b2_TEST FAIL\n"); return 1;}
 std::printf("004de8b2_TEST PASS\n"); return 0;}