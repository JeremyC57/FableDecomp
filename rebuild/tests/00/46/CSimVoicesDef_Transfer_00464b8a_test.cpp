#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: void Xfer_00464b9a(void* sub); };
void CPersistContext::Xfer_00464b9a(void* sub){ g_sub=sub; }
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_00464b9a(reinterpret_cast<char*>(this) + 0x3c); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x3c){ std::printf("00464b8a_TEST FAIL\n"); return 1;}
 std::printf("00464b8a_TEST PASS\n"); return 0;}