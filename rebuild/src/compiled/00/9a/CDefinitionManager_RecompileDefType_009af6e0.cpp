// CDefinitionManager::RecompileDefType(const CCharString&) -- recompiles the definitions of one def type
// Retail: Fable.exe 0x009AF6E0, 60 bytes: ClearDefsForRecompile(type) (0x009AE6B0); if the symbol map is empty
// (PSymbolMap->SLONGMap begin == end), CreateSymbolsFromPathList(SymbolPathList, false) (0x009AEF60);
// then DoCompilePathList(CompilePathList, type) (0x009AF480); `ret 4`.
// Identity: Ego_r.pdb ?RecompileDefType@CDefinitionManager@@QAEXABVCCharString@@@Z (donor 0x00B078B0), followed in the
// PDB by RecompileAllDefs (donor 0x00B078F0) exactly as 0x009AF720 follows this row; the three callees carry the PDB
// protected signatures ClearDefsForRecompile(const CCharString&), CreateSymbolsFromPathList(const vector<CWideString>&,
// bool), DoCompilePathList(const vector<CWideString>&, const CCharString&). The emptiness test is the inline
// CSymbolMap::IsEmpty() const over SLONGMap (CVectorMap at +4: CArray data, begin +4 / end +8).
// CDefinitionManager comes from engine/CDefinitionManager.h (rename pattern): SymbolPathList +0x40, CompilePathList
// +0x4C (12-byte STLport vectors, opaque there), PSymbolMap_Data +0x58.
// Leaf: ?RecompileDefType@CDefinitionManager@@QAEXABVCCharString@@@Z
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
    void RecompileDefType(const CCharString& type);
protected:
    void ClearDefsForRecompile(const CCharString& type);                                              // 0x009AE6B0
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool flag);                 // 0x009AEF60
    void DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& type);           // 0x009AF480
    const std::vector<CWideString>& SymbolPaths() const { return *reinterpret_cast<const std::vector<CWideString>*>(SymbolPathList); }
    const std::vector<CWideString>& CompilePaths() const { return *reinterpret_cast<const std::vector<CWideString>*>(CompilePathList); }
};

void CDefinitionManager::RecompileDefType(const CCharString& type)
{
    ClearDefsForRecompile(type);
    if (PSymbolMap_Data->IsEmpty())
        CreateSymbolsFromPathList(SymbolPaths(), false);
    DoCompilePathList(CompilePaths(), type);
}