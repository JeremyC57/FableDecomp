// Behaviour test for 00706510: src/00706510.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHeroAttachableAppearanceModifiers::UpdateHiddenMaterials()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroAttachableAppearanceModifiers::UpdateHiddenMaterials() -- rebuild the set of hidden material names: clear it, then for every worn modifier of the three types look up its def, take its CAppearanceModifierDef sub-def and insert each of its HideMaterials names
// Leaf: UpdateHiddenMaterials
//
// Retail 0x00706510, 304 bytes (plain `ret`; 8-byte aligned frame `and esp,-8`). Ego_r.pdb
// `?UpdateHiddenMaterials@CTCHeroAttachableAppearanceModifiers@@...` (catalog row).
// Callees: set<CCharString> _M_erase(node) @ 0x004ABA70 (inlined clear(); label "LTextTreeWalkThrough::BuildTreeArray"
// propagated), CTCBase::GetDefinitions() const @ 0x006869D0 (label "_Cons_val<...>" propagated), CDefinitionManager::
// GetPDefFromGlobalIndex(long) @ 0x009AD9E0 (hidden result), CDefClassBase::GetSubDef<CAppearanceModifierDef>
// (CDefPointer<T>&) @ 0x0057FF92 (landed; a same-TU stand-in is compiled above the function), set<CCharString>::insert
// (const CCharString&) @ 0x004AC380 (pair result dropped), the defs' slot 1 on release.
// Layout (retail, Ego_r.pdb order with 12-byte containers, as the landed IsWearingAppearanceModifier 0x00706000):
// HiddenMaterialNames +0x24 (head, count, compare), AppearanceModifiers +0x30 (vector of three vector<CModifierEntry>);
// CAppearanceModifierDef HideMaterials +0x28.
// Shape: per entry the def index is fetched, the manager taken unconditionally, and a positive index converted into a
// typed def pointer (the add-ref and the manager temporary's release share one null test); the sub-def pointer is
// dereferenced before its release, so its null test drops.
struct CCharString {
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
struct CDefPointerBase {                                    // CDefPointer<CDefClassBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    ~CDefPointerBase() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
struct CStringVector {                                      // retail vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
};
struct CAppearanceModifierDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    CStringVector HideMaterials;                            // +0x28
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
    CDefPointer& operator=(const CDefPointerBase& other)
    {
        if (m_p != (T*)other.m_p) {
            if (m_p)
                m_p->Release();
            m_p = (T*)other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointerBase GetPDefFromGlobalIndex_009ad9e0(long index);     // 0x009AD9E0 (hidden result pointer)
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>& out);   // 0x0057FF92
};
void* FindDef_GetSubDef_CAppearanceModifierDef(CDefClassBase* def);    // stand-in callee of the stand-in body below
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CAppearanceModifierDef> 0x0057FF92 (writes the out slot only inline): it tells VC7.1, as
// retail's link-time code generation knew, that the out pointer's address does not escape, so the sub-def lives in edi
// and its frame slot is reused for the insert result (together with the insert stand-in below). Only
// UpdateHiddenMaterials is graded.
bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceModifierDef>& out)
{
    void* def = FindDef_GetSubDef_CAppearanceModifierDef(this);
    if (!def)
        return false;
    out.m_p = (CAppearanceModifierDef*)def;
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
struct CThing;
struct CTCHeroAttachableAppearanceModifiers {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    void* __vftable_0c;                                     // +0x0c secondary vptr
    unsigned long FrameLastChanged;                         // +0x10
    unsigned char Flags14;                                  // +0x14
    unsigned char PrimHandles[0xc];                         // +0x18
    CCharStringSet HiddenMaterialNames;                     // +0x24
    CModifierVectorVector AppearanceModifiers;              // +0x30
    CDefinitionManager* GetDefinitions() const;             // 0x006869D0 (CTCBase)
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
            CDefinitionManager* defs = GetDefinitions();
            if (index > 0) {
                CDefPointerBase result = defs->GetPDefFromGlobalIndex_009ad9e0(index);
                def = result;
            }
            CDefPointer<CAppearanceModifierDef> modifier;
            def->GetSubDef(modifier);
            CAppearanceModifierDef* modifierDef = modifier.operator->();
            for (CCharString* name = modifierDef->HideMaterials._M_start; name != modifierDef->HideMaterials._M_finish; ++name)
                HiddenMaterialNames.insert(*name);
        }
    }
}

// ---- stubs and driver ----
void CDefVirtual::v00() {}
void CTCHeroAttachableAppearanceModifiers::v00() {}
static int g_destroyed = 0;
void CDefVirtual::Destroy() { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager* CTCHeroAttachableAppearanceModifiers::GetDefinitions() const { return &g_mgr; }
static CDefBase g_defs[8];
static long g_requested[16]; static int g_requests = 0;
CDefPointerBase CDefinitionManager::GetPDefFromGlobalIndex_009ad9e0(long index)
{   // the stub's own local is released once when it is copied out, so it takes two references
    g_requested[g_requests++ & 15] = index; CDefPointerBase r; r.m_p = &g_defs[index]; g_defs[index].m_count += 2; return r; }
static CAppearanceModifierDef g_mods[8];
void* FindDef_GetSubDef_CAppearanceModifierDef(CDefClassBase* def) { return &g_mods[(CDefBase*)def - g_defs]; }
static void* g_inserted[32]; static int g_insertCount = 0; static CCharStringSet* g_insertSet;
static CSetNode g_node;
CSetNode* InsertNode_004ac380(CCharStringSet* set, void* key, bool* inserted)
{ g_insertSet = set; g_inserted[g_insertCount++ & 31] = key; *inserted = true; return &g_node; }
static int g_erased = 0; static CSetNode* g_eraseNode;
void CCharStringSet::Erase(CSetNode* node) { ++g_erased; g_eraseNode = node; }
static CCharString g_names[6];
int main()
{
    for (int i = 0; i < 6; ++i) g_names[i].PStringData = (void*)(0x1000 + i);
    for (int i = 0; i < 8; ++i) { g_defs[i].m_count = 1; g_mods[i].m_count = 5; g_mods[i].HideMaterials._M_start = g_mods[i].HideMaterials._M_finish = 0; }
    g_mods[2].HideMaterials._M_start = &g_names[0]; g_mods[2].HideMaterials._M_finish = &g_names[2];
    g_mods[5].HideMaterials._M_start = &g_names[2]; g_mods[5].HideMaterials._M_finish = &g_names[5];
    CModifierEntry e0[2] = { { 2, false }, { 3, true } };
    CModifierEntry e2[1] = { { 5, false } };
    CModifierVector lists[3] = { { e0, e0 + 2, e0 + 2 }, { 0, 0, 0 }, { e2, e2 + 1, e2 + 1 } };
    static unsigned char store[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers& m = *(CTCHeroAttachableAppearanceModifiers*)store;
    CSetNode head; head.Parent = (CSetNode*)0x77; head.Left = head.Right = 0;
    m.HiddenMaterialNames.Head = &head; m.HiddenMaterialNames.Count = 4; m.HiddenMaterialNames.Compare = 0;
    m.AppearanceModifiers._M_start = lists; m.AppearanceModifiers._M_finish = lists + 3;
    m.UpdateHiddenMaterials();
    CHECK(g_erased == 1); CHECK(g_eraseNode == (CSetNode*)0x77); CHECK(head.Left == &head); CHECK(head.Right == &head); CHECK(head.Parent == 0);
    CHECK(m.HiddenMaterialNames.Count == 0);
    CHECK(g_requests == 3); CHECK(g_requested[0] == 2); CHECK(g_requested[1] == 3); CHECK(g_requested[2] == 5);
    CHECK(g_insertCount == 5); CHECK(g_insertSet == &m.HiddenMaterialNames);
    for (int i = 0; i < 5; ++i) CHECK(g_inserted[i] == (void*)(0x1000 + i));
    CHECK(g_defs[2].m_count == 1); CHECK(g_defs[3].m_count == 1); CHECK(g_defs[5].m_count == 1);   // the def copies released
    CHECK(g_mods[2].m_count == 4); CHECK(g_mods[3].m_count == 4); CHECK(g_mods[5].m_count == 4);   // the sub-def pointers released
    CHECK(g_destroyed == 0);
    // empty set: no erase
    g_erased = 0; m.HiddenMaterialNames.Count = 0; m.AppearanceModifiers._M_finish = lists;
    m.UpdateHiddenMaterials();
    CHECK(g_erased == 0); CHECK(g_requests == 6);          // the three lists are walked again
    if (fails == 0) printf("00706510_TEST PASS\n"); else printf("00706510_TEST FAIL\n");
    return fails;
}