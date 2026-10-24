#include <cstdio>
#include <cstring>
#include <new>
struct CDefinitionManager; struct CSymbolMap; struct CStringParser; struct CCharString; struct CMemoryDataInputStream; struct CDataOutputStream;
struct CPersistContext {
    CDefinitionManager* PDefinitionManager; CSymbolMap* PSymbolMap; CStringParser* PParser; void (__cdecl* ErrorCallback)(CCharString*); CCharString* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished; long Mode; bool Binary; CCharString* PStringToSaveTo; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    CPersistContext(CStringParser* parser, void (__cdecl* errorCallback)(CCharString*), bool useDefaults, CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean) { Init(parser, errorCallback, useDefaults, definitionManager, symbolMap, stringToClean); }
    void Init(CStringParser* parser, void (__cdecl* errorCallback)(CCharString*), bool useDefaults, CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean);
};
void CPersistContext::Init(CStringParser* parser, void (__cdecl* errorCallback)(CCharString*), bool useDefaults, CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean)
{
    Mode = 0;
    PDefinitionManager = definitionManager; PSymbolMap = symbolMap; PParser = parser; ErrorCallback = errorCallback; PStringToClean = stringToClean;
    UseDefaults = useDefaults; UseCRCCheck = false; Finished = false; SingleTransfer = false;
}
static void __cdecl OnError(CCharString*) { }
int main()
{
    unsigned char raw[sizeof(CPersistContext)]; std::memset(raw, 0xAA, sizeof(raw));
    CPersistContext& c = *new (raw) CPersistContext((CStringParser*)1, OnError, true, (CDefinitionManager*)2, (CSymbolMap*)3, (CCharString*)4);
    bool ok = c.PParser == (CStringParser*)1 && c.ErrorCallback == OnError && c.UseDefaults && c.PDefinitionManager == (CDefinitionManager*)2 && c.PSymbolMap == (CSymbolMap*)3
           && c.PStringToClean == (CCharString*)4 && c.Mode == 0 && !c.UseCRCCheck && !c.Finished && !c.SingleTransfer && c.PSaveStream == (CDataOutputStream*)0xAAAAAAAA;
    std::printf(ok ? "009badd0_TEST PASS\n" : "009badd0_TEST FAIL\n");
    return ok ? 0 : 1;
}