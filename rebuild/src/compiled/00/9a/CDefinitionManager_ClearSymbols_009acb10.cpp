// CDefinitionManager::ClearSymbols() -- empty the symbol map the manager holds through its counted pointer PSymbolMap (+0x58)
// Leaf: ?ClearSymbols@CDefinitionManager@@QAEXXZ
// Retail: Fable.exe @ 0x009ACB10, 8 bytes: `mov ecx,[ecx+58h] ; jmp CSymbolMap::Clear` (VC7.1 tail-calls the void forward).
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?ClearSymbols@CDefinitionManager@@QAEXXZ (labels_chunk691); called by
//   CScriptDefinitionManager::InitAndCompile 0x00CD3F50 and CMainGameComponent::InitDefinitions 0x00416005 right after the compile.
//   The row had landed as a descriptive `T::Run` forwarder under a junk catalog name.
// Callees: CSymbolMap::Clear 0x009E5250 (Ego_r ?Clear@CSymbolMap@@QAEXXZ).
// Layout: engine/CDefinitionManager.h, PSymbolMap_Data +0x58 (the Data word of CCountedPointer<CSymbolMap> PSymbolMap).
class CSymbolMap { public: void Clear(); };                // 0x009E5250
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager

class CDefinitionManager : public CDefinitionManagerLayout {
public:
    void ClearSymbols();
};

void CDefinitionManager::ClearSymbols()
{
    PSymbolMap_Data->Clear();
}