// NUISystem::CManager::CreateComponent(const CCharString&, bool) -- create a UI component from its def name, then name it
// Grade: DIFFER(71v71) -- `push eax ; mov ecx,esi` (lane) vs retail `mov ecx,esi ; push eax` before CreateComponent(long) 0x0041D21B; everything else byte-exact
// Leaf: ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z
// Retail: Fable.exe @ 0x0041DB1D, 71 bytes (/Os TU: packed address, ebp frame), `ret 8`. Callees:
// CStartupDefinitionManager::Get 0x0043368D and CGameDefinitionManager::Get 0x0044C6B0 (static __fastcall, both
// return a reference; PDB `?Get@...@@SIAAV1@XZ`), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410 (const),
// CManager::CreateComponent(long) 0x0041D21B, then CComponent::SetName (vtable slot 83, +0x14C; the slot body
// 0x0052F0F0 is `add ecx,20h ; jmp CCharString::operator=`).
// Identity: Ego_r.pdb `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z` (the long
// overload is 0x0041D21B, called here); ~80 callers in the front end.
// Shape: three whole GetDefGlobalIndexFromName calls (startup defs / the manager's own definition manager +0x64 /
// the game's). VC7.1 hoists their common `push name` above the first branch and lets the +0x64 path enter the
// shared call with the pointer already in ecx, as retail; one call on a selected pointer pushes after the selection.
// /Os with frame pointers: `#pragma optimize("s", on)` + `#pragma optimize("y", off)` (retail's push ebp frame).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString { public: void* PStringData; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;      // 0x009AD410
};
class CStartupDefinitionManager : public CDefinitionManager { public:
    static CStartupDefinitionManager& __fastcall Get();                 // 0x0043368D
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
};
namespace NUISystem {
class CComponent { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void SetName(const CCharString& name);                     // slot 83 (+0x14C)
};
class CManager { public:
    virtual ~CManager();
    unsigned char _pad04[0x60];
    CDefinitionManager* m_pDefinitionManager;                           // +0x64
    CComponent* CreateComponent(long defIndex);                         // 0x0041D21B
    CComponent* CreateComponent(const CCharString& name, bool startupDefs);
};
}

NUISystem::CComponent* NUISystem::CManager::CreateComponent(const CCharString& name, bool startupDefs)
{
    long defIndex;
    if (startupDefs)
        defIndex = CStartupDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    else if (m_pDefinitionManager)
        defIndex = m_pDefinitionManager->GetDefGlobalIndexFromName(name);
    else
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    CComponent* component = CreateComponent(defIndex);
    component->SetName(name);
    return component;
}
