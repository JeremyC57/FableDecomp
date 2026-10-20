// Behaviour test for 007ee370: src/007ee370.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCVillageMember::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCVillageMember::OnCreate() -- when the owner has a definition index, fetches its definition and resolves the village-member sub-definition into PDef
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE370, length 86 bytes. CTCVillageMember vtable 0x0124007C slot 4, the
// CTCBase OnCreate slot (as CTCTrophy::OnCreate 0x006DDA20 in slot 4; PDB `?OnCreate@CTCVillageMember@@UAEXXZ`).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0 (landed naming, 0x005BDB4D), CDefinitionManager::GetDef<
// CDefClassBase>(long, CDefPointer*) @ 0x0042AF3C (template argument propagated), CDefClassBase::GetSubDef<
// CVillageMemberDef>(CDefPointer*) @ 0x007EE6E0 (template argument propagated), the def's virtual slot 1 (destroy).
// Shape: the def index is a zero-extended word of the owner (+0x8C); the counted local is zeroed after the argument
// pushes (`mov dword ptr [esp+0Ch],0`), GetDef's arguments are pushed before the manager call; the local's object
// is read once into esi for the GetSubDef call and its release (`test ; je ; dec [esi+4] ; jne ; call [vt+4]`):
// a CDefPointer destructor re-reads the escaped local after the call, so the holder is modelled as a plain record
// whose object is copied to a pointer local and released through it.
// Layout: Ego_r.pdb CTCVillageMember (retail = PDB): PDef +0x0C.
struct CVillageMemberDef;

struct CDefPointerVillageMemberDef {
    const CVillageMemberDef* Object;
};

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
    bool GetSubDef(CDefPointerVillageMemberDef* out);   // 0x007EE6E0
};


struct CDefPointerClassBase {
    CDefClassBase* Object;
};

inline void ReleaseDef(CDefClassBase* def)
{
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}

struct CDefinitionManager {
    bool GetDef(long globalDefIndex, CDefPointerClassBase* out) const;   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalDefIndex, CDefPointerClassBase* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad[0x8C];
    unsigned short DefIndex;                // +0x8C
};

struct CTCVillageMember {
    void*                       __vftable;  // +0x00
    CThing*                     Thing;      // +0x04
    unsigned char               _pad_0x08[0x4];
    CDefPointerVillageMemberDef PDef;       // +0x0C
    const CDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void OnCreate();
};

void CTCVillageMember::OnCreate()
{
    if (Thing->DefIndex != 0) {
        CDefPointerClassBase def;
        def.Object = 0;
        CThing* thing = Thing;
        GetDefinitionManager().GetDef(thing->DefIndex, &def);
        CDefClassBase* classDef = def.Object;
        classDef->GetSubDef(&PDef);
        ReleaseDef(classDef);
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static void* g_sub = 0; static int g_destroyed = 0; static long g_index = -1; static int g_getDef = 0;
void* FindDef_CDefinitionManager_GetDef_0() { ++g_getDef; return g_def; }
bool CDefClassBase::GetSubDef(CDefPointerVillageMemberDef* out) { if (!g_sub) return false; out->Object = (const CVillageMemberDef*)g_sub; return true; }
void CDefClassBase::v00() {}
void CDefClassBase::Destroy() { ++g_destroyed; }
static CDefinitionManager g_mgr;
const CDefinitionManager& CTCVillageMember::GetDefinitionManager() const { return g_mgr; }
int main()
{
    CDefClassBase def; def.RefCount = 2; g_def = &def;
    int subStorage = 0; g_sub = &subStorage;
    CThing thing; memset(&thing, 0, sizeof(thing));
    CTCVillageMember m; memset(&m, 0, sizeof(m)); m.Thing = &thing;
    m.OnCreate(); CHECK(g_getDef == 0); CHECK(m.PDef.Object == 0);
    thing.DefIndex = 12; m.OnCreate(); CHECK(g_getDef == 1); CHECK(m.PDef.Object == (const CVillageMemberDef*)&subStorage); CHECK(def.RefCount == 1); CHECK(g_destroyed == 0);
    m.PDef.Object = 0; m.OnCreate(); CHECK(def.RefCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007ee370_TEST PASS\n"); else printf("007ee370_TEST FAIL\n");
    return fails;
}