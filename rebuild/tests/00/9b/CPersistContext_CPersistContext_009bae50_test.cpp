#include <cstdio>
class CCharString { public: int dummy; };
class CMemoryDataInputStream { public: int dummy; };
class CDataOutputStream { public: int dummy; };
class CDefinitionManager { public: int dummy; };
class CPersistContext { public:
    CDefinitionManager* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback; void* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished;
    long Mode; bool Binary; CCharString* PStringToSaveTo; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void Init(CDataOutputStream* s, bool) { PSaveStream = s; Mode = 3; PLoadStream = 0; PDefinitionManager = 0; UseCRCCheck = false; }
    void Construct(CDataOutputStream* s, bool crc) { Init(s, crc); }   // the constructor body
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
    CPersistContext c; Prefill(&c, &dm); c.PLoadStream = &in;
    c.Construct(&out, false);
    ok = c.Mode == 3 && c.PSaveStream == &out && c.PLoadStream == 0 && c.PDefinitionManager == 0 && !c.UseCRCCheck && c.SingleTransfer;
    if (!ok) { std::printf("009bae50_TEST FAIL\n"); return 1; }
    std::printf("009bae50_TEST PASS\n");
    return 0;
}