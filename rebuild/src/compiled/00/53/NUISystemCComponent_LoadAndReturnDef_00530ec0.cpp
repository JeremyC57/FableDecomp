// NUISystem::CComponent::LoadAndReturnDef() const -- look up the component's UI definition (global def index DefIndex) through the front end's definition-manager override, or the game definition manager when there is none, and return it as a counted def pointer
// Leaf: ?LoadAndReturnDef@CComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530EC0, 98 bytes (+ int3 padding), `ret 4` (hidden result slot returned in eax).
// Label: the manifest's "CEventKilledCreature::CEventKilledCreature" is a propagated BSim guess (similarity 0.55). Its four
//   direct callers are NUISystem components (~CList 0x0053B7F0, CEditBox ctor 0x005407B0, ~CText 0x005424B0 and 0x0054F6E0:
//   a virtual called from a constructor/destructor binds statically), it reads one long at +0x130 (the last member of
//   CComponent, DefIndex, PDB +0x158) and Ego_r.pdb has `?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@
//   $$CBVCUIDef@NUISystem@@@@XZ` (virtual, const, no arguments, def pointer by value).
// Callees: CFrontEndManager::GetInstance() @ 0x0041E5F2 (definition-manager override at +0x64), CGameDefinitionManager::Get()
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
struct CFrontEndManager {
    unsigned char _pad[0x64];
    CDefinitionManager* m_pDefinitionManager;    // +0x64
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
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
    CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
    if (frontEnd->m_pDefinitionManager)
        frontEnd->m_pDefinitionManager->GetDef(DefIndex, &def);
    else
        CGameDefinitionManager::Get().GetDef(DefIndex, &def);
    return def;
}
}