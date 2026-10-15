// NUISystem::CRedefinerList::Refresh() -- rebuild the control-redefinition list from the profile's assigned inputs, re-lay the visible rows and the undefined warning, then refresh three sound-system categories
// Leaf: Refresh@CRedefinerList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557000, length 123 bytes.
// Callees: CRedefinerList::ClearList @ 0x005567B0, CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (static),
// CUserProfileManager::GetAssignedInputsPassive @ 0x004088E0, CRedefinerList::RefreshScriptThings(const vector&)
// @ 0x00556A40 (catalog name; it rebuilds the rows from the inputs), CScrollableList::UpdateVisibleList(float, float)
// @ 0x00555A90 (the base list, `this` at offset 0), CRedefinerList::UpdateUndefinedWarning(float) @ 0x00556810;
// the sound-system global @ 0x013B8394 (as in the landed CThingSoundEmitter rows), its vtable slot 46 (+0xb8) called
// with category flags 0x10, 0x80, 0x40.
// Shape: straight-line; `this` is dead before the global test (esi popped there); each virtual call re-reads the
// global (the call may change it).
struct CActionInputControl;
struct CActionInputVector;                         // vector<CActionInputControl>

struct CUserProfileManager {
    static CUserProfileManager& Get();             // 0x0040D2A0
    const CActionInputVector& GetAssignedInputsPassive();   // 0x004088E0
};

struct CSoundSystem {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void RefreshCategory(long category, long a, long b);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;               // 0x013B8394

namespace NUISystem {

struct CScrollableList {
    void UpdateVisibleList(float offset, float time);   // 0x00555A90
};

struct CRedefinerList : CScrollableList {
    void ClearList();                              // 0x005567B0
    void RefreshScriptThings(const CActionInputVector& inputs);   // 0x00556A40
    void UpdateUndefinedWarning(float time);       // 0x00556810
    void Refresh();
};

void CRedefinerList::Refresh()
{
    ClearList();
    RefreshScriptThings(CUserProfileManager::Get().GetAssignedInputsPassive());
    UpdateVisibleList(0.0f, 0.2f);
    UpdateUndefinedWarning(0.2f);
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0, 0);
        g_pSoundSystem->RefreshCategory(0x80, 0, 0);
        g_pSoundSystem->RefreshCategory(0x40, 0, 0);
    }
}

} // namespace NUISystem