#include <cstdio>
static void* g_sub=0;
struct CPersistContext { void Xfer_004e9cba(void* sub); };
void CPersistContext::Xfer_004e9cba(void* sub){ g_sub=sub; }
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004e9cba(reinterpret_cast<char*>(this) + 0x28); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x28){ std::printf("004e9caa_TEST FAIL\n"); return 1;}
 std::printf("004e9caa_TEST PASS\n"); return 0;}