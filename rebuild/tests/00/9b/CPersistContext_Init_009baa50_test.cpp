#include <cstdio>
class CCharString { public: int dummy; };
class CMemoryDataInputStream { public: int dummy; };
class CDataOutputStream { public: int dummy; };
class CDefinitionManager { public: int dummy; };
class CPersistContext { public:
    CDefinitionManager* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback; void* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished;
    long Mode; bool Binary; CCharString* PStringToSaveTo; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void Init(CDataOutputStream* saveStream, bool)
    {
        PSaveStream = saveStream; Mode = 3; PLoadStream = 0; PDefinitionManager = 0; UseCRCCheck = false;
    }
};
static void Prefill(CPersistContext* c, CDefinitionManager* dm)
{
    c->PDefinitionManager = dm; c->PSymbolMap = 0; c->PParser = 0; c->ErrorCallback = 0; c->PStringToClean = 0;
    c->UseDefaults = true; c->UseCRCCheck = true; c->SingleTransfer = true; c->Finished = true;
    c->Mode = 99; c->Binary = true; c->PStringToSaveTo = 0; c->PLoadStream = 0; c->PSaveStream = 0;
}
int main()
{
    bool ok;
    CDefinitionManager dm; CDataOutputStream out; CMemoryDataInputStream in;
    CPersistContext c; Prefill(&c, &dm); c.PLoadStream = &in;
    c.Init(&out, true);
    ok = c.PSaveStream == &out && c.Mode == 3 && c.PLoadStream == 0 && c.PDefinitionManager == 0 && !c.UseCRCCheck
      && c.UseDefaults && c.SingleTransfer && c.Finished && c.Binary;
    if (!ok) { std::printf("009baa50_TEST FAIL\n"); return 1; }
    std::printf("009baa50_TEST PASS\n");
    return 0;
}