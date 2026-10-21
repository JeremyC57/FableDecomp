// CScriptBase::OnScriptedEntityActivated(unsigned __int64 entityID, CScriptThing& entity) -- an entity with a script binding was activated: revive its existing active entity script, or create one through the binding filed under the entity's name, file it in EntityScripts and load it; then bump the entity's script count
// Leaf: OnScriptedEntityActivated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8960, length 465 bytes (`ret 0Ch`, no EH frame).
// PDB: ?OnScriptedEntityActivated@CScriptBase@@QAEX_KAAVCScriptThing@@@Z (sibling of the landed
// OnScriptedEntityDeactivated 0x00CB88B0). The catalog label "CScriptBase::AttachNavigator" is a forge autoname.
// Callees: the EntityScripts map's LowerBound(const unsigned __int64&) 0x00CB9840, the active script's bool setter
// 0x00F359F0 (manifest "CTCPhysicsNavigator::EnableNavigator", a propagated byte-twin), CScriptBase::LoadEntityScript
// 0x00CB7EE0, the binding map's LowerBound 0x00CBA190, CCharString::FastCompare 0x009E6530, the active-script vector's
// _M_insert_overflow 0x00CBC850, SGI sort 0x00CBDEF0 (__fastcall, empty comparator by value), operator delete
// 0x00BFE9BC; virtuals: CScriptThing::GetName (slot 1), SetToKillOnLevelUnload (slot 70), IncrementScriptCounter
// (slot 72); the binding's slot 1 (creates the active script, counted pointer returned by hidden pointer); the
// active script's PEntityScript slot 2.
// Layout: retail CScriptBase sits 0x10 below Ego_r.pdb (as in the landed AddEntityScriptBinding 0x00CB8230):
// EntityScripts +0x08 (12-byte vector, Compare +0x14, Dirty +0x15), EntityScriptBindings +0x18;
// CActiveEntityScriptBase PEntityScript +0x34, Flags +0x3C (Ego_r.pdb).
// Shape: CVectorMap::Add and the STLport push_back must be __forceinline (else VC emits Add out of line); the
// pair temporary forwards its four words from registers into the in-place copy, the growth call homes the
// __false_type tag in the entity argument slot; the inline Sort re-reads Dirty right after the store.
struct CCharString {
    char* m_pData;
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};
struct CScriptThing {                                // vtable 0x01238C8C
    virtual void v00();
    virtual const CCharString& GetName() const;      // slot 1 (0x004AA8B0)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void SetToKillOnLevelUnload(bool kill);  // slot 70 (0x004AB0C0)
    virtual void v71();
    virtual void IncrementScriptCounter();           // slot 72 (0x004AB0F0)
};
struct CEntityScriptBase {
    virtual void v00(); virtual void v01();
    virtual void Restart();                          // slot 2
};
class CActiveEntityScriptBase {
public:
    void SetKillRequested(bool kill);                // 0x00F359F0
    unsigned char      _pad00[0x34];
    CEntityScriptBase* PEntityScript;                // +0x34 (CCountedPointer Data)
    void*              PEntityScriptInfo;            // +0x38
    unsigned long      Flags;                        // +0x3C
    void DeleteData();
};
void __cdecl operator delete(void* block);           // 0x00BFE9BC
template <class T> struct SCountBlock {
    long m_references;
    void (T::*m_pfnDestroy)();
    T*   m_pItem;
};
template <class T> struct CCountedPointer {
    T* Data;
    SCountBlock<T>* Info;
    CCountedPointer() {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* p) { return p; }
struct CEntityScriptEntry {                          // pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
    unsigned __int64                          first;
    CCountedPointer<CActiveEntityScriptBase>  second;
    CEntityScriptEntry(const unsigned __int64& a, const CCountedPointer<CActiveEntityScriptBase>& b) : first(a), second(b) {}
    CEntityScriptEntry(const CEntityScriptEntry& p) : first(p.first), second(p.second) {}
};
struct CEntityScriptCompare { };
void __fastcall sort(CEntityScriptEntry* first, CEntityScriptEntry* last, CEntityScriptCompare comp);   // 0x00CBDEF0
struct CEntityScriptMap {
    CEntityScriptEntry* _M_start;
    CEntityScriptEntry* _M_finish;
    CEntityScriptEntry* _M_end_of_storage;
    CEntityScriptCompare Compare;                    // +0x0c
    bool                 Dirty;                      // +0x0d
    CEntityScriptEntry* end() { return _M_finish; }
    CEntityScriptEntry* LowerBound(const unsigned __int64& key);   // 0x00CB9840
    void _M_insert_overflow(CEntityScriptEntry* pos, const CEntityScriptEntry& x, const __false_type& tag, unsigned long fill_len, bool atend);   // 0x00CBC850
    CEntityScriptEntry* find(const unsigned __int64& key)
    {
        CEntityScriptEntry* it = LowerBound(key);
        if (it != end() && key >= it->first)
            return it;
        return end();
    }
    __forceinline void push_back(const CEntityScriptEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CEntityScriptEntry(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    __forceinline void Add(const unsigned __int64& key, const CCountedPointer<CActiveEntityScriptBase>& value)
    {
        push_back(CEntityScriptEntry(key, value));
        Dirty = true;
    }
    void Sort()
    {
        if (Dirty) {
            sort(_M_start, _M_finish, Compare);
            Dirty = false;
        }
    }
};
class CEntityScriptBindingBase {
public:
    virtual const CCharString& GetName() const;      // slot 0
    virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveScript(CScriptThing& entity, unsigned __int64 entityID);   // slot 1
};
struct SBindingEntry {
    CCharString                                first;
    CEntityScriptBindingBase*                  second_Data;
    void*                                      second_Info;
};
struct CBindingMap {
    SBindingEntry* _M_start;
    SBindingEntry* _M_finish;
    SBindingEntry* _M_end_of_storage;
    unsigned char  Compare;
    bool           Dirty;
    SBindingEntry* LowerBound(const CCharString& key);   // 0x00CBA190
    SBindingEntry* end() { return _M_finish; }
    SBindingEntry* find(const CCharString& key)
    {
        SBindingEntry* it = LowerBound(key);
        SBindingEntry* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

long NearSink_a(const void* self, long seen);
long NearSink_b(const void* self, long seen);
#pragma auto_inline(off)
__declspec(noinline) CEntityScriptEntry* CEntityScriptMap::LowerBound(const unsigned __int64& key)
{
    return (CEntityScriptEntry*)NearSink_a(this, *(const long*)&key);
}
__declspec(noinline) void CEntityScriptMap::_M_insert_overflow(CEntityScriptEntry* pos, const CEntityScriptEntry& x, const __false_type& tag, unsigned long fill_len, bool atend)
{
    CEntityScriptEntry* p = (CEntityScriptEntry*)NearSink_b(this, fill_len);
    if (p)
        new (p) CEntityScriptEntry(x);
}
#pragma auto_inline(on)

class CScriptBase {
public:
    virtual ~CScriptBase();
    void OnScriptedEntityActivated(unsigned __int64 entityID, CScriptThing& entity);
    void LoadEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase* script);   // 0x00CB7EE0
private:
    void*            Threads;                        // +0x04
    CEntityScriptMap EntityScripts;                  // +0x08
    CBindingMap      EntityScriptBindings;           // +0x18
};

void CScriptBase::OnScriptedEntityActivated(unsigned __int64 entityID, CScriptThing& entity)
{
    CEntityScriptEntry* it = EntityScripts.find(entityID);
    if (it != EntityScripts.end())
    {
        CActiveEntityScriptBase* script = it->second.Data;
        if (script->Flags & 1)
            script->SetKillRequested(false);
        else
        {
            script->PEntityScript->Restart();
            LoadEntityScript(entityID, script);
        }
    }
    else
    {
        const CCharString& name = entity.GetName();
        SBindingEntry* binding = EntityScriptBindings.find(name);
        CCountedPointer<CActiveEntityScriptBase> script = binding->second_Data->CreateActiveScript(entity, entityID);
        EntityScripts.Add(entityID, script);
        EntityScripts.Sort();
        if (script.Data->Flags & 1)
            entity.SetToKillOnLevelUnload(false);
        LoadEntityScript(entityID, script.Data);
    }
    entity.IncrementScriptCounter();
}
