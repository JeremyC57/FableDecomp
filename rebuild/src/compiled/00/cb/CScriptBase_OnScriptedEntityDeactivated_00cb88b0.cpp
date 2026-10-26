// CScriptBase::OnScriptedEntityDeactivated(unsigned __int64 entityID, CScriptThing& entity, bool removing) -- an entity with an active entity script was deactivated: either flag the script to be killed (removal of a kill-on-removal script) or mark it deactivated and save it, then drop the entity's script count
// Leaf: OnScriptedEntityDeactivated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB88B0, length 128 bytes.
// PDB: ?OnScriptedEntityDeactivated@CScriptBase@@QAEX_KAAVCScriptThing@@_N@Z. EntityScripts: CVectorMap<unsigned
// __int64, CCountedPointer<CActiveEntityScriptBase> > (retail element vector at +0x08); CActiveEntityScriptBase
// Flags at +0x3C (PDB 60; bit 0 tested here), a bool at +0x05 set on deactivation.
// Callees: the map's LowerBound(const unsigned __int64&) @ 0x00CB9840 (manifest names the CANavQuadTreeNode*
// instantiation: byte-identical body), the active script's bool setter at +0x28 @ 0x00F359F0 (manifest says
// CTCPhysicsNavigator::EnableNavigator: a propagated byte-twin; descriptive name here), CScriptBase::SaveEntityScript
// @ 0x00CB83D0, CScriptThing::DecrementScriptCounter (vtable slot 73, +0x124).
// Shape: the inline find takes the key by reference (LowerBound gets the argument slot itself; the key is re-read
// after the call) and keeps a hit only for `key >= it->first`; one `mov eax,1` serves the flag test, the pushed
// true and the byte store; VC duplicates the DecrementScriptCounter tail into both arms.
class CScriptThing { public:                                // vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72();
    virtual void DecrementScriptCounter();           // slot 73 (+0x124)
};

class CActiveEntityScriptBase {
public:
    void SetKillRequested(bool kill);                // 0x00F359F0 (stores the +0x28 byte)
    unsigned char _pad00[5];
    bool          Deactivated;                       // +0x05
    unsigned char _pad06[0x3C - 0x06];
    unsigned long Flags;                             // +0x3C
};

template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
};

struct CEntityScriptEntry {                          // pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
    unsigned __int64                          first;
    CCountedPointer<CActiveEntityScriptBase>  second;
};

struct CEntityScriptMap {
    CEntityScriptEntry* _M_start;
    CEntityScriptEntry* _M_finish;
    CEntityScriptEntry* _M_end_of_storage;
    CEntityScriptEntry* end() { return _M_finish; }
    CEntityScriptEntry* LowerBound(const unsigned __int64& key);   // 0x00CB9840
    CEntityScriptEntry* find(const unsigned __int64& key)
    {
        CEntityScriptEntry* it = LowerBound(key);
        if (it != end() && key >= it->first)
            return it;
        return end();
    }
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the binary search LowerBound runs (returns the entry)
#pragma auto_inline(off)
// Same-TU stand-in for CEntityScriptMap::LowerBound @ 0x00CB9840 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) CEntityScriptEntry* CEntityScriptMap::LowerBound(const unsigned __int64& key)
{
    return (CEntityScriptEntry*)NearSink_s7_0(this, *(const long*)&key);
}
#pragma auto_inline(on)

class CScriptBase {
public:
    virtual ~CScriptBase();
    void OnScriptedEntityDeactivated(unsigned __int64 entityID, CScriptThing& entity, bool removing);
    void SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script);   // 0x00CB83D0
private:
    void*            Threads;                        // +0x04
    CEntityScriptMap EntityScripts;                  // +0x08
};
long NearSink_s7_1(const void* self, long seen);   // stand-in: the save SaveEntityScript does outside this TU (sees the id and the script flags)
#pragma auto_inline(off)
// Same-TU stand-in for CScriptBase::SaveEntityScript @ 0x00CB83D0 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CScriptBase::SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script)
{
    NearSink_s7_1(this, (long)entityID + script.Flags);
}
#pragma auto_inline(on)

void CScriptBase::OnScriptedEntityDeactivated(unsigned __int64 entityID, CScriptThing& entity, bool removing)
{
    CEntityScriptEntry* it = EntityScripts.find(entityID);
    const unsigned __int64 id = entityID;
    if (it != EntityScripts.end())
    {
        CActiveEntityScriptBase* script = it->second.Data;
        if (removing && (script->Flags & 1))
            script->SetKillRequested(true);
        else
        {
            script->Deactivated = true;
            SaveEntityScript(id, *script);
        }
        entity.DecrementScriptCounter();
    }
}