// Behaviour test for 0x00CB5C70 (CScriptInfoManager constructor): both containers empty, the map clean,
// CurrentScriptID 1.
#include <cstdio>
#include <new>

struct CCharString { char* m_pData; };
struct CScriptInfoEntry;

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

struct CScriptInfoManager {
    long                      CurrentScriptID;
    CScriptInfoMap            Scripts;
    CRetailVector<CCharString> CodeSectionNames;
    CScriptInfoManager();
};

CScriptInfoManager::CScriptInfoManager()
{
    CurrentScriptID = 1;
}

int main()
{
    unsigned char raw[sizeof(CScriptInfoManager)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CScriptInfoManager* m = new (raw) CScriptInfoManager;
    if (m->CurrentScriptID != 1 || m->Scripts.Dirty || m->Scripts.m_entries.m_first || m->Scripts.m_entries.m_last ||
        m->Scripts.m_entries.m_end || m->CodeSectionNames.m_first || m->CodeSectionNames.m_last || m->CodeSectionNames.m_end) {
        std::printf("00cb5c70_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb5c70_TEST PASS\n");
    return 0;
}