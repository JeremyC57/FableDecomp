#include <cstdio>
// CPersistContext::IsSaving: true for Mode 1 (text save) and Mode 3 (binary save)
struct CPersistContext {
    void* PDefinitionManager; void* PSymbolMap; void* PParser; void* ErrorCallback; void* PStringToClean;
    bool UseDefaults; bool UseCRCCheck; bool SingleTransfer; bool Finished;
    long Mode; bool Binary; void* PStringToSaveTo; void* PLoadStream; void* PSaveStream;
    long IsSaving() const { return Mode == 1 || Mode == 3; }
};
int main()
{
    CPersistContext c;
    const long expect[6] = { 0, 1, 0, 1, 0, 0 };
    for (long m = 0; m < 6; ++m) {
        c.Mode = m;
        if (c.IsSaving() != expect[m]) { std::printf("004aef50_TEST FAIL\n"); return 1; }
    }
    std::printf("004aef50_TEST PASS\n");
    return 0;
}