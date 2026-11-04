// CDefinitionManager::RecompileAllDefs() -- recompiles every definition (RecompileDefType with an empty type name)
// Retail: Fable.exe 0x009AF720, 114 bytes: the RecompileDefType body with a temporary CCharString("") (ctor
// 0x0099EBF0 with length -1, literal 0x0122D70E, destroyed by 0x0099EAE0) in place of the type for each of
// ClearDefsForRecompile (0x009AE6B0) and DoCompilePathList (0x009AF480); CreateSymbolsFromPathList (0x009AEF60) when
// the symbol map is empty. Two separate temporaries, each destroyed right after its call.
// Identity: Ego_r.pdb ?RecompileAllDefs@CDefinitionManager@@QAEXXZ (donor 0x00B078F0, right after RecompileDefType
// 0x00B078B0, as 0x009AF720 follows 0x009AF6E0); called by tail jumps from 0x0042DBD2 / 0x00CB5AB7.
// CDefinitionManager comes from engine/CDefinitionManager.h (rename pattern): SymbolPathList +0x40, CompilePathList
// +0x4C, PSymbolMap_Data +0x58.
// Leaf: ?RecompileAllDefs@CDefinitionManager@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};
class CWideString { public: void* PStringData; };

#include "engine/CVectorMap.h"

template <class T> class CCompareString { };

class CSymbolMap {
public:
    class CSymbolInfo;
    virtual ~CSymbolMap();
    bool IsEmpty() const { return SLONGMap.empty(); }
    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SLONGMap;   // +0x04
};

#define CDefinitionManager DonorCDefinitionManager
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager

class CDefinitionManager : public DonorCDefinitionManager {
public:
    void RecompileAllDefs();
protected:
    void ClearDefsForRecompile(const CCharString& type);                                              // 0x009AE6B0
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool flag);                 // 0x009AEF60
    void DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& type);           // 0x009AF480
    const std::vector<CWideString>& SymbolPaths() const { return *reinterpret_cast<const std::vector<CWideString>*>(SymbolPathList); }
    const std::vector<CWideString>& CompilePaths() const { return *reinterpret_cast<const std::vector<CWideString>*>(CompilePathList); }
};

void CDefinitionManager::RecompileAllDefs()
{
    ClearDefsForRecompile("");
    if (PSymbolMap_Data->IsEmpty())
        CreateSymbolsFromPathList(SymbolPaths(), false);
    DoCompilePathList(CompilePaths(), "");
}