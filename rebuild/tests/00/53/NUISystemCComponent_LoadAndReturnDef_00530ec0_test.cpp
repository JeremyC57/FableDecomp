// Behaviour test for 00530ec0: src/00530ec0.cpp copied verbatim; LoadAndReturnDef looks the component's DefIndex up in
// the front end's definition manager when it has one, else in the game definition manager, and returns the def with
// the caller's reference (the local's reference dropped).
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// NUISystem::CComponent::LoadAndReturnDef() const -- look up the component's UI definition (global def index DefIndex) through the front end's definition-manager override, or the game definition manager when there is none, and return it as a counted def pointer
// Leaf: ?LoadAndReturnDef@CComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530EC0, 98 bytes (+ int3 padding), `ret 4` (hidden result slot returned in eax).
// Label: the manifest's "CEventKilledCreature::CEventKilledCreature" is a propagated BSim guess (similarity 0.55). Its four
//   direct callers are NUISystem components (~CList 0x0053B7F0, CEditBox ctor 0x005407B0, ~CText 0x005424B0 and 0x0054F6E0:
//   a virtual called from a constructor/destructor binds statically), it reads one long at +0x130 (the last member of
//   CComponent, DefIndex, PDB +0x158) and Ego_r.pdb has `?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@
//   $$CBVCUIDef@NUISystem@@@@XZ` (virtual, const, no arguments, def pointer by value).
// Callees: NUISystem::CManager::GetInstance() @ 0x0041E5F2 (definition-manager override at +0x64), CGameDefinitionManager::Get()
//   @ 0x0044C6B0, CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042AEDA, the def's virtual slot 1 (release).
// Layout: retail CComponent DefIndex +0x130 (Ego_r.pdb +0x158: the 16-byte PDB vectors/containers before it are 12 bytes
//   in retail); no engine header for CComponent exists.
// Shape: one CDefPointer local filled by either manager's GetDef (if/else, the `&def` push hoisted above the test) and
//   returned by value: the copy's add-ref and the local's release fold into `inc edx ; mov eax,edx ; dec eax` on the same
//   object. GetDef is a same-TU noinline stand-in body (retail's LTCG saw it keep no reference to the slot), so the
//   def stays in ecx.
struct CDefBase {
    virtual void v00();
    virtual void Destroy() const;                // slot 1
    mutable long RefCount;                       // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long globalIndex, CDefPointer<T>* out);   // 0x0042AEDA
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<T>* out)
{
    void* found = Lookup_CDefinitionManager_GetDef(this, globalIndex);
    if (!found)
        return false;
    out->Object = (T*)found;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
namespace NUISystem { class CManager { public:
    unsigned char _pad[0x64];
    CDefinitionManager* m_pDefinitionManager;    // +0x64
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
}; }
namespace NUISystem {
class CUIDef : public CDefBase { public:};
class CComponent { public:
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;
    unsigned char _pad004[0x130 - 0x04];
    long DefIndex;                               // +0x130 (PDB +0x158)
};
CDefPointer<const CUIDef> CComponent::LoadAndReturnDef() const
{
    CDefPointer<const CUIDef> def;
    NUISystem::CManager* frontEnd = NUISystem::CManager::GetInstance();
    if (frontEnd->m_pDefinitionManager)
        frontEnd->m_pDefinitionManager->GetDef(DefIndex, &def);
    else
        CGameDefinitionManager::Get().GetDef(DefIndex, &def);
    return def;
}
}
// ---- harness ----
static int g_destroyed = 0; static const void* g_manager = 0; static long g_index = -1;
static void* g_found = 0;
static NUISystem::CManager g_frontEnd; static CGameDefinitionManager g_game; static CDefinitionManager g_override;
void CDefBase::v00() { }
void CDefBase::Destroy() const { ++g_destroyed; }
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_game; }
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_frontEnd; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ g_manager = manager; g_index = index; if (g_found) ++((CDefBase*)g_found)->RefCount; return g_found; }
int main()
{
    static NUISystem::CUIDef def; def.RefCount = 1;
    static NUISystem::CComponent component; component.DefIndex = 0x77;

    // no override: the game manager; the caller holds one extra reference
    g_frontEnd.m_pDefinitionManager = 0; g_found = &def;
    {
        CDefPointer<const NUISystem::CUIDef> result = component.LoadAndReturnDef();
        CHECK(result.Object == &def && def.RefCount == 2 && g_manager == &g_game && g_index == 0x77);
    }
    CHECK(def.RefCount == 1 && g_destroyed == 0);

    // the front end's override manager
    g_frontEnd.m_pDefinitionManager = &g_override;
    {
        CDefPointer<const NUISystem::CUIDef> result = component.LoadAndReturnDef();
        CHECK(result.Object == &def && g_manager == &g_override);
    }

    // lookup fails: null
    g_found = 0;
    {
        CDefPointer<const NUISystem::CUIDef> result = component.LoadAndReturnDef();
        CHECK(result.Object == 0);
    }

    // the caller's reference is the last one
    g_found = &def; def.RefCount = 0;
    {
        CDefPointer<const NUISystem::CUIDef> result = component.LoadAndReturnDef();
        CHECK(def.RefCount == 1 && g_destroyed == 0);
    }
    CHECK(g_destroyed == 1);

    if (fails == 0) printf("00530ec0_TEST PASS\n");
    return fails;
}