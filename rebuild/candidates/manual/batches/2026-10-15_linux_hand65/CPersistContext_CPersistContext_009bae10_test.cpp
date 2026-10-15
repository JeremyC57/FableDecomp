#include <cstdio>
struct CCharString { int dummy; };
struct CMemoryDataInputStream { int dummy; };
struct CDataOutputStream { int dummy; };
struct CDefinitionManager { int dummy; };
struct CPersistContext {
    CDefinitionManager* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback; void* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished;
    long Mode; bool Binary; CCharString* PStringToSaveTo; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void Init(CCharString* s, CDefinitionManager* dm) { Mode = 1; PStringToSaveTo = s; PDefinitionManager = dm; UseCRCCheck = false; }
    void Construct(CCharString* s) { Init(s, 0); }   // the constructor body
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
    CDefinitionManager dm; CCharString text;
    CPersistContext c; Prefill(&c, &dm);
    c.Construct(&text);
    ok = c.Mode == 1 && c.PStringToSaveTo == &text && c.PDefinitionManager == 0 && !c.UseCRCCheck && c.UseDefaults && c.Binary;
    if (!ok) { std::printf("009bae10_TEST FAIL\n"); return 1; }
    std::printf("009bae10_TEST PASS\n");
    return 0;
}
