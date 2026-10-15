#include <cstdio>
struct CCharString { int dummy; };
struct CMemoryDataInputStream { int dummy; };
struct CDataOutputStream { int dummy; };
struct CDefinitionManager { int dummy; };
struct CPersistContext {
    CDefinitionManager* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback; void* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished;
    long Mode; bool Binary; CCharString* PStringToSaveTo; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void Init(CMemoryDataInputStream* s, bool) { PLoadStream = s; Mode = 2; PSaveStream = 0; PDefinitionManager = 0; UseCRCCheck = false; }
    void Construct(CMemoryDataInputStream* s, bool crc) { Init(s, crc); }   // the constructor body
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
    CDefinitionManager dm; CMemoryDataInputStream in; CDataOutputStream out;
    CPersistContext c; Prefill(&c, &dm); c.PSaveStream = &out;
    c.Construct(&in, true);
    ok = c.Mode == 2 && c.PLoadStream == &in && c.PSaveStream == 0 && c.PDefinitionManager == 0 && !c.UseCRCCheck && c.Finished;
    if (!ok) { std::printf("009bae30_TEST FAIL\n"); return 1; }
    std::printf("009bae30_TEST PASS\n");
    return 0;
}