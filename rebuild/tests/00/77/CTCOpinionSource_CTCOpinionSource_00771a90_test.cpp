// Behaviour test for CTCOpinionSource::~CTCOpinionSource (0x00771A90, compiler-generated): release the opinion set (counted
// pointer: Data and Info zeroed), the faction def pointer, the def-stack vector block, then ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CInfo { unsigned long RefCount; int DeleteCalls; };
struct CCountedModel {
    void* Data; CInfo* Info;
    ~CCountedModel() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteCalls++; Log("C"); } } Data = 0; Info = 0; }
};
struct CDefPointee { long RefCount; int Deleted; };
struct CDefModel { CDefPointee* Object; ~CDefModel() { if (Object) { if (--Object->RefCount == 0) { Object->Deleted++; Log("F"); } Object = 0; } } };
struct CLongVectorModel { long* Start; ~CLongVectorModel() { if (Start) { delete[] Start; Log("V"); } } };
struct CTCBaseModel { virtual ~CTCBaseModel() { Log("B"); } };
struct CTCOpinionSourceModel : CTCBaseModel {
    CLongVectorModel DefStack;
    CDefModel RestrictToFactionAndAllies;
    CCountedModel POpinionProduced;
};
int main()
{
    CInfo info = { 1, 0 };
    CDefPointee faction = { 1, 0 };
    {
        CTCOpinionSourceModel s;
        s.DefStack.Start = new long[4];
        s.RestrictToFactionAndAllies.Object = &faction;
        s.POpinionProduced.Data = &info; s.POpinionProduced.Info = &info;
    }
    if (std::strcmp(g_log, "CFVB") != 0 || info.DeleteCalls != 1 || faction.Deleted != 1) { std::printf("00771a90_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    {
        CTCOpinionSourceModel s;
        s.DefStack.Start = 0; s.RestrictToFactionAndAllies.Object = 0; s.POpinionProduced.Data = 0; s.POpinionProduced.Info = 0;
    }
    if (std::strcmp(g_log, "B") != 0) { std::printf("00771a90_TEST FAIL\n"); return 1; }
    std::printf("00771a90_TEST PASS\n");
    return 0;
}