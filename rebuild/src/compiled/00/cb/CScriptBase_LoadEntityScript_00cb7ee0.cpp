// CScriptBase::LoadEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script) -- if a persisted script text exists for the entity, feed a copy of it to the entity script's persist loader
// Leaf: LoadEntityScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7EE0, length 117 bytes (+11 int3 padding).
// Catalog name "CScriptBase::LoadOrCreatePersistInfo"; the PDB CScriptBase member list has
// ?LoadEntityScript@CScriptBase@@IAEX_KAAVCActiveEntityScriptBase@@@Z, the only (_K, CActiveEntityScriptBase&)
// member beside SaveEntityScript (0x00CB83D0, the other row of this chunk).
// Callees: CVectorMap<unsigned __int64, CPersistInfo>::LowerBound(const unsigned __int64&) @ 0x00CB97B0 (the
// RuntimeScriptedEntityPersist map, retail +0x30), CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// CScriptBase::DoEntityScriptPersist(CActiveEntityScriptBase&, CCharString&, bool) @ 0x00CB7DA0, ~CCharString
// @ 0x0099EAE0.
// Shape: the inlined CRuntimeScriptedEntityPersist::GetEntityPersistScript (find = LowerBound on a copy of the key,
// `it != end() && key >= it->first` keeps the hit (gives retail's jb/ja order), then `it == end ? 0 : &it->second.Script`), then the copy as a named local in its
// own block (VC places it in the dead entityID argument slot).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public:
        unsigned __int64 EntityID;                   // +0x00 (pair +0x08)
        CCharString      Script;                     // +0x08 (pair +0x10)
        long             Flags;                      // +0x0C (pair +0x14)
    };
    struct CEntry {                                  // pair<unsigned __int64, CPersistInfo>
        unsigned __int64 first;
        CPersistInfo     second;
    };
    struct CVectorMap {
        CEntry* _M_start;
        CEntry* _M_finish;
        CEntry* _M_end_of_storage;
        CEntry* end() { return _M_finish; }
        CEntry* LowerBound(const unsigned __int64& key);   // 0x00CB97B0
        CEntry* find(unsigned __int64 key)
        {
            unsigned __int64 k = key;
            CEntry* it = LowerBound(k);
            if (it != end() && key >= it->first)
                return it;
            return end();
        }
    };
    CVectorMap PersistScripts;

    const CCharString* GetEntityPersistScript(unsigned __int64 id)
    {
        CEntry* it = PersistScripts.find(id);
        return it == PersistScripts.end() ? 0 : &it->second.Script;
    }
};
} // namespace NScript

class CActiveEntityScriptBase;

class CScriptBase {
public:
    virtual ~CScriptBase();
    void LoadEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script);
    void DoEntityScriptPersist(CActiveEntityScriptBase& script, CCharString& text, bool saving);   // 0x00CB7DA0
private:
    unsigned char _body[0x30 - 4];
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;   // +0x30 (retail)
};

void CScriptBase::LoadEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script)
{
    const CCharString* persisted = RuntimeScriptedEntityPersist.GetEntityPersistScript(entityID);
    if (persisted)
    {
        CCharString text(*persisted);
        DoEntityScriptPersist(script, text, false);
    }
}