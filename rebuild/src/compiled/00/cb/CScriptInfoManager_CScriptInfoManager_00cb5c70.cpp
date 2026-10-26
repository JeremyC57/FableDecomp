// CScriptInfoManager::CScriptInfoManager() -- constructor: empty Scripts map (clean), empty CodeSectionNames, CurrentScriptID 1
// Leaf: ??0CScriptInfoManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5C70, length 32 bytes. No callees, no vtable.
// Layout (PDB names, retail sizes): CurrentScriptID +0; Scripts = CVectorMap<CCharString, CScriptInfo,
// CKeyPairCompareLess> +4 {vector +4..+0xf (retail vectors are 12 bytes, the PDB's 16), the empty Compare
// +0x10, bool Dirty +0x11}; CodeSectionNames = vector<CCharString> +0x14..+0x1f (sizeof 0x20, PDB 0x28).
// Shape: the member objects' inline constructors zero the six vector words and Dirty through the shared
// zero register in declaration order; CurrentScriptID = 1 is the LAST store, an immediate: it is a body
// assignment (in the initialiser list it is stored first, before the member constructors).
// No engine header exists for CScriptInfoManager; the containers are modelled with their retail layout.
class CCharString { public: char* m_pData; };
struct CScriptInfoEntry;                              // pair<CCharString, CScriptInfo>

template <class T>
struct CRetailVector {
    T* m_first;
    T* m_last;
    T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) { }
};

struct CKeyPairCompareLess { };

struct CScriptInfoMap {
    CRetailVector<CScriptInfoEntry> m_entries;
    CKeyPairCompareLess             Compare;
    bool                            Dirty;
    CScriptInfoMap() : Dirty(false) { }
};

class CScriptInfoManager { public:
    long                      CurrentScriptID;
    CScriptInfoMap            Scripts;
    CRetailVector<CCharString> CodeSectionNames;
    CScriptInfoManager();
};

CScriptInfoManager::CScriptInfoManager()
{
    CurrentScriptID = 1;
}