#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = "";
class CWorld; class CPlayer; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBaseInit { public: CWorld* World; CPlayer* MyPlayer; CGameDefinitionManager* DefinitionManager; CMainGameComponent* Component; };
class CAInputProcess { public: virtual void v00() { } void* PPlayerInterface; void* InputProcessListPos_Data; void* InputProcessListPos_Next; void* InputProcessListPos_Previous; CAInputProcess() { std::strcat(g_seq, "A"); PPlayerInterface = 0; } };
struct CInputProcessGameBase : CAInputProcess { virtual void v00() { } CWorld* World; CPlayer* MyPlayer; CGameDefinitionManager* DefinitionManager; CMainGameComponent* Component; CGamePlayerInterface* PGamePlayerInterface;
    CInputProcessGameBase(const CInputProcessGameBaseInit& init) : CAInputProcess() { World = init.World; MyPlayer = init.MyPlayer; DefinitionManager = init.DefinitionManager; Component = init.Component; PGamePlayerInterface = 0; } };
int main()
{
    CInputProcessGameBaseInit init; init.World = (CWorld*)1; init.MyPlayer = (CPlayer*)2; init.DefinitionManager = (CGameDefinitionManager*)3; init.Component = (CMainGameComponent*)4;
    CInputProcessGameBase b(init);
    if (std::strcmp(g_seq, "A") || b.World != (CWorld*)1 || b.MyPlayer != (CPlayer*)2 || b.DefinitionManager != (CGameDefinitionManager*)3 || b.Component != (CMainGameComponent*)4 || b.PGamePlayerInterface != 0 || (g_is32 && (char*)&b.World - (char*)&b != 0x14) || (g_is32 && sizeof(b) != 0x28)) { std::printf("00687a30_TEST FAIL\n"); return 1; }
    std::printf("00687a30_TEST PASS\n");
    return 0;
}