#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: void Xfer_00475d99(void* sub); };
void CPersistContext::Xfer_00475d99(void* sub){ g_sub=sub; }
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_00475d99(reinterpret_cast<char*>(this) + 0x3c); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x3c){ std::printf("004ebf22_TEST FAIL\n"); return 1;}
 std::printf("004ebf22_TEST PASS\n"); return 0;}