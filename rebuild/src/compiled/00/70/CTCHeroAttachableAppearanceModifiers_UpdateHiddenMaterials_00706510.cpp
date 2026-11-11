// CTCHeroAttachableAppearanceModifiers::UpdateHiddenMaterials() -- rebuild the set of hidden material names: clear it, then for every worn modifier of the three types look up its def, take its CAppearanceModifierDef sub-def and insert each of its HideMaterials names
// Leaf: UpdateHiddenMaterials
//
// Retail 0x00706510, 304 bytes (plain `ret`; 8-byte aligned frame `and esp,-8`). Ego_r.pdb
// `?UpdateHiddenMaterials@CTCHeroAttachableAppearanceModifiers@@...` (catalog row).
// Callees: set<CCharString> _M_erase(node) @ 0x004ABA70 (inlined clear(); label "LTextTreeWalkThrough::BuildTreeArray"
// propagated), CTCBase::GetDefinitionManager() const @ 0x006869D0 (label "_Cons_val<...>" propagated), CDefinitionManager::
// GetPDefFromGlobalIndex(long) @ 0x009AD9E0 (hidden result), CDefClassBase::GetSubDef<CAppearanceModifierDef>
// (CDefPointer<const T>*) const @ 0x0057FF92 (landed; a same-TU stand-in is compiled above the function), set<CCharString>::insert
// (const CCharString&) @ 0x004AC380 (pair result dropped), the defs' slot 1 on release.
// Layout (retail, Ego_r.pdb order with 12-byte containers, as the landed IsWearingAppearanceModifier 0x00706000):
// HiddenMaterialNames +0x24 (head, count, compare), AppearanceModifiers +0x30 (vector of three vector<CModifierEntry>);
// CAppearanceModifierDef HideMaterials +0x28.
// Shape: per entry the def index is fetched, the manager taken unconditionally, and a positive index converted into a
// typed def pointer (the add-ref and the manager temporary's release share one null test); the sub-def pointer is
// dereferenced before its release, so its null test drops.
class CCharString { public:
    void* PStringData;
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer;
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> { public:   // the manager's by-value return type (released and cleared)
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
struct CStringVector {                                      // retail vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
};
class CAppearanceModifierDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CStringVector HideMaterials;                            // +0x28
};
template <class T> class CDefPointer { public:
    CDefBase* m_p;                                          // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return static_cast<T*>(m_p); }
    CDefPointer& operator=(const CDefPointer<const CDefClassBase>& other)
    {
        if (m_p != other.m_p) {
            if (m_p)
                m_p->Release();
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE; hidden result pointer)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 GetSubDef<CAppearanceModifierDef> (Ego_r / FableWin template)
};
void* FindDef_GetSubDef_CAppearanceModifierDef(const CDefClassBase* def);    // stand-in callee of the stand-in body below
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CAppearanceModifierDef> 0x0057FF92 (writes the out slot only inline): it tells VC7.1, as
// retail's link-time code generation knew, that the out pointer's address does not escape, so the sub-def lives in edi
// and its frame slot is reused for the insert result (together with the insert stand-in below). Only
// UpdateHiddenMaterials is graded.
template <> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    void* def = FindDef_GetSubDef_CAppearanceModifierDef(this);
    if (!def)
        return false;
    out->m_p = (CDefBase*)def;
    return true;
}
#pragma auto_inline(on)

struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};
struct CModifierVector {
    CModifierEntry* _M_start;
    CModifierEntry* _M_finish;
    CModifierEntry* _M_end_of_storage;
};
struct CModifierVectorVector {
    CModifierVector* _M_start;
    CModifierVector* _M_finish;
    CModifierVector* _M_end_of_storage;
    CModifierVector& operator[](long i) { return _M_start[i]; }
};
struct CSetNode {
    unsigned char Color;
    CSetNode* Parent;                                       // +0x04
    CSetNode* Left;                                         // +0x08
    CSetNode* Right;                                        // +0x0c
};
struct SSetIterator { CSetNode* m_pNode; };
struct SSetInsertResult {                                // pair<iterator, bool>
    SSetIterator first;
    bool second;
    SSetInsertResult() { }
};
struct CCharStringSet {                                     // 12 bytes: head, count, compare
    CSetNode* Head;
    unsigned long Count;
    unsigned long Compare;
    void Erase(CSetNode* node);                             // 0x004ABA70
    SSetInsertResult insert(const CCharString& value);      // 0x004AC380
    void clear()
    {
        if (Count) {
            Erase(Head->Parent);
            Head->Left = Head;
            Head->Parent = 0;
            Head->Right = Head;
            Count = 0;
        }
    }
};
class CThing;
class CTCBase { public:                                     // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCHeroAttachableAppearanceModifiers : public CTCBase { public:
    void* __vftable_0c;                                     // +0x0c secondary vptr
    unsigned long FrameLastChanged;                         // +0x10
    unsigned char Flags14;                                  // +0x14
    unsigned char PrimHandles[0xc];                         // +0x18
    CCharStringSet HiddenMaterialNames;                     // +0x24
    CModifierVectorVector AppearanceModifiers;              // +0x30
    void UpdateHiddenMaterials();
};

CSetNode* InsertNode_004ac380(CCharStringSet* set, void* key, bool* inserted);
#pragma auto_inline(off)
// Same-TU stand-in for set<CCharString>::insert 0x004AC380 (pair returned by hidden pointer): with only a declaration
// VC7.1 gives the dropped pair its own 8 frame bytes (0x20 frame); with a body it packs it into the sub-def slot (0x18).
__declspec(noinline) SSetInsertResult CCharStringSet::insert(const CCharString& value)
{
    SSetInsertResult result;
    bool inserted;
    result.first.m_pNode = InsertNode_004ac380(this, value.PStringData, &inserted);
    result.second = inserted;
    return result;
}
#pragma auto_inline(on)
void CTCHeroAttachableAppearanceModifiers::UpdateHiddenMaterials()
{
    HiddenMaterialNames.clear();
    for (long type = 0; type < 3; ++type) {
        CModifierVector& list = AppearanceModifiers[type];
        for (CModifierEntry* entry = list._M_start; entry != list._M_finish; ++entry) {
            long index = entry->DefIndex;
            CDefPointer<CDefClassBase> def;
            const CGameDefinitionManager* defs = &GetDefinitionManager();
            if (index > 0) {
                CDefPointer<const CDefClassBase> result = defs->GetPDefFromGlobalIndex(index);
                def = result;
            }
            CDefPointer<const CAppearanceModifierDef> modifier;
            def->GetSubDef(&modifier);
            const CAppearanceModifierDef* modifierDef = modifier.operator->();
            for (CCharString* name = modifierDef->HideMaterials._M_start; name != modifierDef->HideMaterials._M_finish; ++name)
                HiddenMaterialNames.insert(*name);
        }
    }
}