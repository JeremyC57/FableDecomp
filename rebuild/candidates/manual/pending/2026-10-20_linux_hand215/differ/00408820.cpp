// CUserProfileManager::ResetAssignedInputsWASD() -- reset the passive and aggressive input assignments to the PC WASD control scheme (the no-caps-lock variant when that option is set), or to the pad defaults when the pad is in use, then apply the values
// Grade: DIFFER(185v185) -- register allocation only: retail keeps this in edi, the scheme def in esi and its input list in ebx; ours this in ebx and the def in edi (a reference local for the inputs changes nothing). The forceinline lookup with constant returns and the raw scheme pointer released by hand fixed the size and the shared tails
// Leaf: ResetAssignedInputsWASD
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408820, length 185 bytes (192 with padding).
// Callees: CUserProfileManager::ResetAssignedInputs @ 0x004085F0, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0 (returns the global @ 0x013B879C),
// the pre-main definition manager getter @ 0x0043368D (returns the global @ 0x013B8760; manifest "GFGetPreMainMemoryUsed"
// is propagated), CDefinitionManager::GetDef<T>(const CCharString&, CDefPointer<T>*) @ 0x00410820, the input vector's
// operator= @ 0x00411B90, the def's virtual Destroy (slot 1), CUserProfileManager::ApplyValues @ 0x00407370.
// Globals: the pad-in-use flag @ 0x013B85F3, the no-caps-lock option @ 0x013B864F, g_pMainGameComponent @ 0x013B86A0,
// the pre-main definition manager @ 0x013B8760 (descriptive names). Literals @ 0x0122E1FC / 0x0122E1D8.
// Layout: CUserProfileManager AssignedInputsPassive +0x54, AssignedInputsAggressive +0x60 (Ego_r.pdb with 12-byte retail
// vectors); the control-scheme def's input list at +0x3c (descriptive).
// Shape: the manager choice is a `__forceinline` helper whose arms evaluate GetDef's arguments before the getter and
// return constant true (VC7.1 threads them into the assignment block); the def object is read into a raw pointer local
// (zero-initialised, the same zero as the out pointer) and released by hand after the name's destructor.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CInputVector {                                       // vector<CActionInputControl>, 12 bytes in retail
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CInputVector& operator=(const CInputVector& other);     // 0x00411B90
};
struct CControlSchemeDef {
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x3c - 0x08];
    CInputVector  Inputs;                                   // +0x3c (descriptive)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer<CControlSchemeDef>* out);   // 0x00410820
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get();                   // 0x0044C6B0
};
CDefinitionManager& GetPreMainDefinitionManager();          // 0x0043368D
struct CMainGameComponent;
extern bool                g_bPadControlsActive;           // 0x013B85F3
extern bool                g_bWASDWithoutCapsLock;         // 0x013B864F
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
extern CDefinitionManager* g_pPreMainDefinitionManager;    // 0x013B8760

__forceinline bool GetControlScheme(const CCharString& name, CDefPointer<CControlSchemeDef>* def)
{
    if (g_pMainGameComponent) {
        CGameDefinitionManager::Get().GetDef(name, def);
        return true;
    }
    if (g_pPreMainDefinitionManager) {
        GetPreMainDefinitionManager().GetDef(name, def);
        return true;
    }
    return false;
}

struct CUserProfileManager {
    unsigned char _pad00[0x54];
    CInputVector  AssignedInputsPassive;                    // +0x54
    CInputVector  AssignedInputsAggressive;                 // +0x60
    void ResetAssignedInputs();                             // 0x004085F0
    void ApplyValues();                                     // 0x00407370
    void ResetAssignedInputsWASD();
};

void CUserProfileManager::ResetAssignedInputsWASD()
{
    if (g_bPadControlsActive) {
        ResetAssignedInputs();
        return;
    }
    CControlSchemeDef* scheme = 0;
    CDefPointer<CControlSchemeDef> def;
    {
        CCharString name(g_bWASDWithoutCapsLock ? "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK" : "FABLE_PC_CONTROL_SCHEME_GDD_WASD", -1);
        if (GetControlScheme(name, &def)) {
            scheme = def.Object;
            AssignedInputsPassive = scheme->Inputs;
            AssignedInputsAggressive = scheme->Inputs;
        }
    }
    if (scheme) {
        if (--scheme->RefCount == 0)
            scheme->Destroy();
    }
    ApplyValues();
}
