#include <cstdio>
static void* g_sub=0;
class CPersistContext { public: void Xfer_00433df0(void* sub); };
void CPersistContext::Xfer_00433df0(void* sub){ g_sub=sub; }
struct S { char pad[0x28]; void AddLearnedExpression(CPersistContext* ctx); };
void S::AddLearnedExpression(CPersistContext* ctx){ ctx->Xfer_00433df0(reinterpret_cast<char*>(this) + 0x28); }
int main(){ S o; CPersistContext c; o.AddLearnedExpression(&c); if(g_sub!=(char*)&o+0x28){ std::printf("0047e186_TEST FAIL\n"); return 1;}
 std::printf("0047e186_TEST PASS\n"); return 0;}