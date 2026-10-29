#include <cstdio>
struct GS2 { char pad[0xcc]; char fcc; };
class CUserProfileManager : public GS2 { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
static GS2 g_gs;
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *(CUserProfileManager*)(&g_gs); }
void __fastcall fn_0088e930(void* self, int _edx, char arg){ CUserProfileManager::Get().fcc = arg; }
int main(){ fn_0088e930(0,0,(char)5); if(g_gs.fcc!=5){ printf("0088e930_TEST FAIL\n"); return 1; }
 printf("0088e930_TEST PASS\n"); return 0; }