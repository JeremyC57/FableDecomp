// Behaviour test for 005b413d: src/005b413d.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryClothing::SetAsWearing(const CCharString& defName)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryClothing::SetAsWearing(const CCharString& defName) -- makes the hero wear the named clothing item: unless the piece already worn in that suit part cannot be removed, selects the item and dresses the owner in the selected clothes
// Retail 0x005B413D, 286 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCBase::GetDefinitionManager 0x006869D0 (x3), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&)
// 0x009AD410, GetDef(long, CDefPointer<CThingObjectDef>*) 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>
// 0x0057FF92 (x2), map<EClothingSuitPart, long>::find 0x005BA986 (returns the node; manifest "RBTree_FindNode"),
// CTCInventoryBase::SelectItemWithDefIndex(long) 0x005BDFB1, SetThingAsWearingSelectedClothes(CThing*) 0x005B406D,
// the defs' slot 1 on release.
// Shape: four CDefPointer locals with inline null-tested releases; the item's modifier is homed in the dead `defName`
// slot and the `wear` flag in its high byte; the worn piece's def and modifier are zeroed together and the modifier's
// release folds away on the GetDef failure path; the suit part key shares a frame word with the worn modifier;
// UseMannequin is cleared around SelectItemWithDefIndex.
// Layout (retail): CurrentClothingSelected +0x140, UseMannequin +0x15C; ParentThing +0x04; CAppearanceModifierDef
// IsRemoveable +0x44, SuitPart +0x4C (Ego_r.pdb, 8 below the PDB); the map node's value +0x14.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CAppearanceModifierDef : public CDefPointeeBase { public:
    unsigned char _pad_0x08[0x44 - 0x08];
    bool IsRemoveable;                               // +0x44
    unsigned char _pad_0x45[0x4c - 0x45];
    long SuitPart;                                   // +0x4c EClothingSuitPart
};
class CThingObjectDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out);   // 0x0057FF92
};
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CThingObjectDef::GetSubDef(CDefPointer<CAppearanceModifierDef>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
    bool GetDef(long globalIndex, CDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
void* FindDef_s7r_1();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_1 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_1 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_1()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<CThingObjectDef>* out)
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct SSuitPartNode {                               // _Rb_tree_node<pair<const EClothingSuitPart, long>>
    void* _M_links[4];
    long key;                                        // +0x10
    long value;                                      // +0x14
};
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    SSuitPartNode* _M_header; unsigned long _M_node_count; long _M_key_compare;
    SSuitPartNode* end() { return _M_header; }
    SSuitPartNode* find(const long& part);           // 0x005BA986
};
long TmplSink_s7_2(const void* self, long seen);   // stand-in: the work find does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the find instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) SSuitPartNode* CSuitPartMap::find(const long& part)
{
    long r = TmplSink_s7_2(this, 0 + *(const long*)&part);
    return (SSuitPartNode*)r;
}
#pragma auto_inline(on)

class CTCInventoryClothing { public:
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x140 - 0x08];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    unsigned char _pad_0x14c[0x15c - 0x14c];
    bool UseMannequin;                               // +0x15c
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void SelectItemWithDefIndex(long defIndex);      // 0x005BDFB1 (CTCInventoryBase)
    void SetThingAsWearingSelectedClothes(CThing* thing);   // 0x005B406D
    void SetAsWearing(const CCharString& defName);
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work SetThingAsWearingSelectedClothes does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CTCInventoryClothing::SetThingAsWearingSelectedClothes @ 0x005B406D (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CTCInventoryClothing::SetThingAsWearingSelectedClothes(CThing* thing)
{
    NearSink_s7_0(this, 0);
}
#pragma auto_inline(on)

void CTCInventoryClothing::SetAsWearing(const CCharString& defName)
{
    long defIndex = GetDefinitionManager()->GetDefGlobalIndexFromName(defName);
    CDefPointer<CThingObjectDef> def;
    CDefPointer<CAppearanceModifierDef> modifier;
    if (GetDefinitionManager()->GetDef(defIndex, &def)) {
        if (def->GetSubDef(&modifier)) {
            bool wear = true;
            long part = modifier->SuitPart;
            SSuitPartNode* worn = CurrentClothingSelected.find(part);
            if (worn != CurrentClothingSelected.end()) {
                CDefPointer<CThingObjectDef> wornDef;
                CDefPointer<CAppearanceModifierDef> wornModifier;
                if (GetDefinitionManager()->GetDef(worn->value, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier))
                        wear = wornModifier->IsRemoveable;
                }
            }
            if (wear) {
                UseMannequin = false;
                SelectItemWithDefIndex(defIndex);
                SetThingAsWearingSelectedClothes(ParentThing);
                UseMannequin = true;
            }
        }
    }
}

// ---- stubs and driver ----
void CDefPointeeBase::v00() {}
CCharString::CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) {}
CCharString::~CCharString() {}
static int g_destroyed = 0;
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static void* g_subDefs[4]; static int g_nSub = 0, g_iSub = 0;
long TmplSink_s7_0(const void* self, long seen) { return g_iSub < g_nSub ? (long)g_subDefs[g_iSub++] : 0; }
static void* g_defs[4]; static int g_nDef = 0, g_iDef = 0; static long g_defIdx[4];
long TmplSink_s7_1(const void* self, long seen) { return g_iDef < g_nDef ? (long)g_defs[g_iDef++] : 0; }
static SSuitPartNode* g_found = 0; static long g_findKey = -1;
long TmplSink_s7_2(const void* self, long seen) { g_findKey = seen; return (long)(g_found ? g_found : ((CSuitPartMap*)self)->_M_header); }
static int g_released = 0; void ReleaseDef_s7(void* def) { ++g_released; }
static int g_wearCalls = 0; static bool g_mannequinAtWear = true;
static CTCInventoryClothing* g_inv = 0;
long NearSink_s7_0(const void* self, long seen) { ++g_wearCalls; g_mannequinAtWear = g_inv->UseMannequin; return 0; }
static CDefinitionManager g_dm;
CDefinitionManager* CTCInventoryClothing::GetDefinitionManager() const { return &g_dm; }
static const CCharString* g_name = 0;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_name = &name; return 77; }
static long g_selected = -1; static bool g_mannequinAtSelect = true;
void CTCInventoryClothing::SelectItemWithDefIndex(long defIndex) { g_selected = defIndex; g_mannequinAtSelect = UseMannequin; }
static void Reset() { g_iSub = g_iDef = 0; g_nSub = g_nDef = 0; g_wearCalls = 0; g_selected = -1; g_found = 0; }
int main()
{
    static CTCInventoryClothing inv; g_inv = &inv; static SSuitPartNode header, node; inv.CurrentClothingSelected._M_header = &header;
    inv.UseMannequin = true; CCharString name(*(CCharString*)&g_name);
    static CThingObjectDef def, wornDef; static CAppearanceModifierDef mod, wornMod;
    mod.SuitPart = 4; node.value = 55;
    // no def
    Reset(); inv.SetAsWearing(name); CHECK(g_name == &name); CHECK(g_wearCalls == 0); CHECK(g_selected == -1);
    // def without the appearance-modifier sub-def: def released
    Reset(); def.RefCount = 1; g_defs[0] = &def; g_nDef = 1; inv.SetAsWearing(name);
    CHECK(g_wearCalls == 0); CHECK(def.RefCount == 0); CHECK(g_destroyed == 1);
    // part not worn: wear
    Reset(); def.RefCount = 2; mod.RefCount = 2; g_defs[0] = &def; g_nDef = 1; g_subDefs[0] = &mod; g_nSub = 1;
    inv.SetAsWearing(name);
    CHECK(g_findKey == 4); CHECK(g_selected == 77); CHECK(g_mannequinAtSelect == false); CHECK(g_wearCalls == 1); CHECK(g_mannequinAtWear == false);
    CHECK(inv.UseMannequin == true); CHECK(def.RefCount == 1); CHECK(mod.RefCount == 1);
    // part worn by a non-removable item: no wear
    Reset(); g_found = &node; wornDef.RefCount = 1; wornMod.RefCount = 1; wornMod.IsRemoveable = false;
    g_defs[0] = &def; g_defs[1] = &wornDef; g_nDef = 2; g_subDefs[0] = &mod; g_subDefs[1] = &wornMod; g_nSub = 2;
    inv.SetAsWearing(name); CHECK(g_wearCalls == 0); CHECK(wornDef.RefCount == 0); CHECK(wornMod.RefCount == 0);
    // worn but removable: wear
    Reset(); g_found = &node; wornDef.RefCount = 1; wornMod.RefCount = 1; wornMod.IsRemoveable = true;
    g_defs[0] = &def; g_defs[1] = &wornDef; g_nDef = 2; g_subDefs[0] = &mod; g_subDefs[1] = &wornMod; g_nSub = 2;
    def.RefCount = 5; mod.RefCount = 5; inv.SetAsWearing(name); CHECK(g_wearCalls == 1); CHECK(g_selected == 77);
    // worn item whose def is gone: wear
    Reset(); g_found = &node; g_defs[0] = &def; g_nDef = 1; g_subDefs[0] = &mod; g_nSub = 1; inv.SetAsWearing(name); CHECK(g_wearCalls == 1);
    if (fails == 0) printf("005b413d_TEST PASS\n"); else printf("005b413d_TEST FAIL\n");
    return fails;
}