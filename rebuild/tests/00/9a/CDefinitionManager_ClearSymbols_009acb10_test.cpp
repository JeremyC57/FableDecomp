// Behaviour test for 0x009ACB10 CDefinitionManager::ClearSymbols(): forwards to Clear() on the symbol map at +0x58.
#include <stdio.h>
class CSymbolMap { public: int cleared; void Clear() { ++cleared; } };
class CDefinitionManager { public: char pad[0x58]; CSymbolMap* PSymbolMap_Data; void ClearSymbols(); };
void CDefinitionManager::ClearSymbols() { PSymbolMap_Data->Clear(); }
int main(void)
{
    CSymbolMap m; m.cleared = 0;
    CDefinitionManager d; d.PSymbolMap_Data = &m;
    d.ClearSymbols(); d.ClearSymbols();
    if (m.cleared != 2) { printf("FAIL_009acb10\n"); return 1; }
    printf("PASS_009acb10\n"); return 0;
}