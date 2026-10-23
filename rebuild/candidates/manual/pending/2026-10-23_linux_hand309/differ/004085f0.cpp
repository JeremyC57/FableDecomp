// CUserProfileManager::ResetAssignedInputs() -- reload the passive and aggressive input assignments from the PC control-scheme defs (left-click-is-king, two-mode or one-mode scheme) through whichever definition manager is live, then apply the values
// Grade: DIFFER(622v557) -- retail loads the def pointer once into edi after GetDef and keeps it (and &Controls in ebp) across both vector assignments and the release, cross-jumping the scheme tails; ours reloads the address-taken local after every call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004085F0, length 557 bytes, plain `ret`.
// Callees: GetGameDefinitionManager 0x0044C6B0 / GetFrontendDefinitionManager 0x0043368D (__fastcall, no arguments),
//   CDefinitionManager::GetDef<CControlsDef> 0x00410820, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   vector<CActionInputControl>::operator= 0x00411B90, CUserProfileManager::ApplyValues 0x00407370; the def's release
//   through its virtual slot 1. Globals: scheme flags 0x013B85F4 (left click is king) and 0x013B85F3 (two-mode scheme),
//   g_pMainGameComponent 0x013B86A0, the front-end definition manager 0x013B8760 (presence only).
// Layout: CUserProfileManager (Ego_r.pdb, retail 12-byte vectors): AssignedInputsPassive +0x54, AssignedInputsAggressive
//   +0x60. CControlsDef (Ego_r.pdb) Controls retail +0x3c. CDefPointer / CDefBase as the landed CDefinitionManager::GetDef
//   0x00410820.
// Shape: each scheme branch probes the game then the front-end manager (VC7.1 cross-jumps the identical tails); the name
//   strings are implicit conversions (built before the pushes); the def pointer is a function-scope local.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {                                        // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CActionInputControl;
struct CActionInputControlVector {                          // STLport vector<CActionInputControl>, 12 bytes
    CActionInputControl* _M_start;
    CActionInputControl* _M_finish;
    CActionInputControl* _M_end_of_storage;
    CActionInputControlVector& operator=(const CActionInputControlVector& other);   // 0x00411B90
};
struct CControlsDef : CDefBase {
    unsigned char             _pad08[0x3c - 0x08];
    CActionInputControlVector Controls;                     // +0x3c
};
struct CDefPointer {                                        // CDefPointer<const CControlsDef>
    CControlsDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CControlsDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer* out); // 0x00410820
};
CDefinitionManager* __fastcall GetGameDefinitionManager();       // 0x0044C6B0
CDefinitionManager* __fastcall GetFrontendDefinitionManager();   // 0x0043368D
extern void* g_pMainGameComponent;                          // 0x013B86A0 (presence only)
extern void* g_pFrontendDefinitionManager;                  // 0x013B8760 (presence only)
extern bool g_bControlSchemeLeftClickIsKing;                // 0x013B85F4
extern bool g_bControlSchemeTwoModes;                       // 0x013B85F3
struct CUserProfileManager {
    unsigned char             _pad00[0x54];
    CActionInputControlVector AssignedInputsPassive;        // +0x54
    CActionInputControlVector AssignedInputsAggressive;     // +0x60
    void ApplyValues();                                     // 0x00407370
    void ResetAssignedInputs();
};

void CUserProfileManager::ResetAssignedInputs()
{
    CDefPointer def;
    if (g_bControlSchemeLeftClickIsKing) {
        if (g_pMainGameComponent) {
            GetGameDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        } else if (g_pFrontendDefinitionManager) {
            GetFrontendDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        }
    } else if (g_bControlSchemeTwoModes) {
        if (g_pMainGameComponent) {
            GetGameDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def);
            AssignedInputsPassive = def->Controls;
            GetGameDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def);
            AssignedInputsAggressive = def->Controls;
        } else if (g_pFrontendDefinitionManager) {
            GetFrontendDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def);
            AssignedInputsPassive = def->Controls;
            GetFrontendDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def);
            AssignedInputsAggressive = def->Controls;
        }
    } else {
        if (g_pMainGameComponent) {
            GetGameDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        } else if (g_pFrontendDefinitionManager) {
            GetFrontendDefinitionManager()->GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def);
            AssignedInputsPassive = def->Controls;
            AssignedInputsAggressive = def->Controls;
        }
    }
    ApplyValues();
}
