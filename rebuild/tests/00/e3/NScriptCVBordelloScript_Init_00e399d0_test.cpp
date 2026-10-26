// Behaviour test for CV_BordelloScript::Init (00e399d0): every field reset, the village handle released.
#include <cstdio>
#include <cstring>
static int g_destroyed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct Info { long refs; void (*destroy)(void*); void* obj; };
struct CScriptThing {
    void* Data; Info* PInfo;
    CScriptThing() : Data(0), PInfo(0) { }
    ~CScriptThing() { Release(); }
    void Release() { if (PInfo) { if (--PInfo->refs == 0) { PInfo->destroy(PInfo->obj); delete PInfo; } } Data = 0; PInfo = 0; }
    CScriptThing& operator=(const CScriptThing& o)
    {
        Info* i = o.PInfo; void* d = o.Data;
        if (PInfo != i) { Release(); Data = d; PInfo = i; if (i) ++i->refs; }
        return *this;
    }
};
struct Bordello {
    bool Flags[9]; long ClientsAlive; bool ClientInUse[3]; CScriptThing Village; bool Wig, HadSex; long BeersDrunk; bool Beer;
    void Init()
    {
        HadSex = false;
        for (int i = 0; i < 9; ++i) Flags[i] = false;
        ClientsAlive = 0; Wig = false;
        ClientInUse[0] = ClientInUse[1] = ClientInUse[2] = false;
        Village = CScriptThing();
        BeersDrunk = 0; Beer = false;
    }
};
int main()
{
    Bordello b;
    std::memset(b.Flags, 1, sizeof(b.Flags)); b.ClientsAlive = 3; b.ClientInUse[0] = b.ClientInUse[1] = b.ClientInUse[2] = true;
    b.Wig = b.HadSex = b.Beer = true; b.BeersDrunk = 9;
    Info* info = new Info; info->refs = 1; info->destroy = Destroy; info->obj = 0;
    int obj; b.Village.Data = &obj; b.Village.PInfo = info;
    b.Init();
    bool ok = g_destroyed == 1 && b.Village.Data == 0 && b.Village.PInfo == 0 && !b.Wig && !b.HadSex && !b.Beer && b.BeersDrunk == 0
        && b.ClientsAlive == 0 && !b.ClientInUse[2] && !b.Flags[8] && !b.Flags[0];
    b.Init();
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("00e399d0_TEST FAIL\n"); return 1; }
    std::printf("00e399d0_TEST PASS\n");
    return 0;
}