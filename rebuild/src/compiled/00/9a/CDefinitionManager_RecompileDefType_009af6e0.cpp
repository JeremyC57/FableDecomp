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
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};
class CWideString { public: void* PStringData; };

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        const _Tp* begin() const { return this->_M_start; }
        const _Tp* end() const { return this->_M_finish; }
        bool empty() const { return begin() == end(); }
    };
}

template <class T> class CArray : public std::vector<T> { };
template <class K, class V, class TCompare> class CVectorMap {     // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                      // +0x00
    TCompare   Compare;                                 // +0x0C
    bool       Dirty;                                   // +0x0D
    bool empty() const { return Data.empty(); }
};
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