// CScriptBase::OnScriptedEntityDeactivated(unsigned __int64 entityID, CScriptThing& entity, bool removing) -- an entity with an active entity script was deactivated: either flag the script to be killed (removal of a kill-on-removal script) or mark it deactivated and save it, then drop the entity's script count
// Leaf: OnScriptedEntityDeactivated
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
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
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

class CActiveEntityScriptBase {
public:
    void SetKillRequested(bool kill);                // 0x00F359F0 (stores the +0x28 byte)
    unsigned char _pad00[5];
    bool          Deactivated;                       // +0x05
    unsigned char _pad06[0x3C - 0x06];
    unsigned long Flags;                             // +0x3C
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