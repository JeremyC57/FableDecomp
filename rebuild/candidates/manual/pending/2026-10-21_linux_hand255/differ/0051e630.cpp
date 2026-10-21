// Grade: DIFFER(369v369) -- frame, flow, copy-not-elided names and CSE'd def pointer match; only the scratch registers rotate (retail loads the def-name index into ecx / hidden pointer edx, the loop tail reloads this into ecx; VC7.1 here picks eax/ecx/edx)
// CThingManager::DebugVerifyUniqueIDs(vector<CThing*>& things) -- for every thing with a unique ID, find the other managed things that carry the same ID and give each of them a fresh one; the first clash also builds both things' def names (debug text whose use is compiled out)
// Leaf: ?DebugVerifyUniqueIDs@CThingManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E630, length 369 bytes, `ret 4`.
//   Name and argument from the catalog / engine_api (`CThingManager::DebugVerifyUniqueIDs(vector<CThing*>*)`).
// Callees: CThing @ 0x004C7BC0 (a bool query forwarded to the global thing manager [0x013B8A1C], whose +0x9a byte it
//   returns via 0x0051E2E0; the result is unused -- manifest "_Cons_val<...>" is propagated), the base def's virtual
//   slot 10 (+0x28, returns a CDefString by hidden pointer), CDefStringTable::GetString(CDefString) @ 0x009D49B0 on the
//   table object @ 0x013CA828, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//   CThing::CreateUID @ 0x004C7FF0.
// Layout (retail): CThing UniqueID +0x18 (Ego_r.pdb +0x18), PBaseDef +0x70 (PDB +0x80, CThing 0x10 lower);
//   CThingManager +0x14 the std::list<CThing*> of all things (head +0x18; inside the PDB's unnamed base part).
// Shape: each name comes from an inline CThing::GetDefName() returning a CCharString by value, so the named local is
//   copy-constructed from the inline's temporary (not elided); the def-name index is a class-typed conditional.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString {
    long Index;
    CDefString(long index) : Index(index) {}
};
struct CDefStringTable {
    CCharString GetString(CDefString name);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CThingBaseDef {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;              // slot 10
};

template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

struct CThing {
    unsigned char      _pad00[0x18];
    unsigned __int64   UniqueID;                        // +0x18
    unsigned char      _pad20[0x70 - 0x20];
    CDefPointer<const CThingBaseDef> PBaseDef;          // +0x70
    bool IsManagerFlagSet() const;                      // 0x004C7BC0
    void CreateUID();                                   // 0x004C7FF0
    __forceinline CCharString GetDefName() const
    {
        const CDefString& name = PBaseDef ? PBaseDef->GetDefName() : CDefString(-1);
        CCharString result = g_DefStringTable.GetString(name);
        return result;
    }
};

template<class T> struct vector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() { return _Myfirst; }
    T* end() { return _Mylast; }
};
template<class T> struct list {
    struct _Node { _Node* _Next; _Node* _Prev; T _Myval; };
    void*  _Alval;
    _Node* _Myhead;
    unsigned int _Mysize;
};

struct CThingManager {
    unsigned char  _pad00[0x14];
    list<CThing*>  Things;                              // +0x14
    void DebugVerifyUniqueIDs(vector<CThing*>& things);
};

void CThingManager::DebugVerifyUniqueIDs(vector<CThing*>& things)
{
    bool reported = false;
    for (CThing** it = things.begin(); it != things.end(); ++it) {
        CThing* thing = *it;
        thing->IsManagerFlagSet();
        if (thing->UniqueID) {
            for (list<CThing*>::_Node* node = Things._Myhead->_Next; node != Things._Myhead; node = node->_Next) {
                CThing* other = node->_Myval;
                if (thing != other && thing->UniqueID == other->UniqueID && thing->UniqueID) {
                    if (!reported && (thing->PBaseDef || other->PBaseDef)) {
                        CCharString thingName = thing->GetDefName();
                        CCharString otherName = other->GetDefName();
                        reported = true;
                    }
                    other->CreateUID();
                }
            }
        }
    }
}
