// CUserProfileManager::ResetAssignedInputsWASD() -- reset the passive and aggressive input assignments to the PC WASD control scheme (the no-caps-lock variant when that option is set), or to the defaults when aggressive mode is available, then apply the values
// Grade: DIFFER(ours v retail) -- regraded as-is in Q15 (FABLE_HDR_OVERLAY=$A/hdr): DIFFER(185v185), the three-register rotation of the earlier note; NOT PORTED this pass -- the draft is still a local model, so it is not landable even if it matched; port onto the shared engine headers first (see NOTES.md)
// Earlier grade note: DIFFER(185v185) -- three-register rotation this/def/controls = ebx/edi/esi v retail edi/esi/ebx (dl 43, 185v185; base = chunk 629 draft, try c)
// Leaf: ?ResetAssignedInputsWASD@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408820, 185 bytes, `ret`.
// Identity: Ego_r.pdb ?ResetAssignedInputsWASD@CUserProfileManager@@QAEXXZ (next to ?ResetAssignedInputs@ 0x004085F0); caller
//   NUISystem::CRedefinerList::ResetKeysToDefaults 0x005565C0 (landed).
// Callees: CUserProfileManager::ResetAssignedInputs 0x004085F0, CCharString::CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, CStartupDefinitionManager::Get 0x0043368D (loads
//   0x013B8760), CDefinitionManager::GetDef<CControlsDef>(const CCharString&, CDefPointer<const CControlsDef>*) const
//   0x00410820 (FableWin.pdb ??$GetDef@VCControlsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCControlsDef@@@@@Z;
//   one body for every GetDef<T>, the catalog label <CHeroPostcardGeneratorDef> is ICF-propagated),
//   vector<CActionInputControl>::operator= 0x00411B90, the def's virtual Delete (slot 1), CUserProfileManager::ApplyValues
//   0x00407370 (protected, Ego_r.pdb ?ApplyValues@CUserProfileManager@@IAEXXZ).
// Globals (bound spellings): g_bAggressiveModeAvailable 0x013B85F3, g_KeyRedefinerFlag 0x013B864F (the no-caps-lock
//   option), g_pMainGameComponent 0x013B86A0, g_value_013b8760 0x013B8760 (the startup definition manager).
//   Literals 0x0122E1FC "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK" / 0x0122E1D8 "FABLE_PC_CONTROL_SCHEME_GDD_WASD".
// Layout: CUserProfileManager AssignedInputsPassive +0x54, AssignedInputsAggressive +0x60 (12-byte retail vectors);
//   CControlsDef : CParentDefClassBase (RTTI) with Controls (vector<CActionInputControl>) at retail +0x3C (Ego_r +0x40).
// Shape: the manager choice is a __forceinline helper whose arms evaluate GetDef's arguments before the getter and return
//   constant true (VC7.1 threads them into the assignment block); the def object is read into a raw pointer local
//   (zero-initialised, the same zero as the out pointer) and released by hand after the name's destructor -- the shared
//   CDefPointer<const CControlsDef> local (engine/CDefPointer.h) re-reads Object at every use (202v185, chunk 629).
// Tried (chunk 629, on top of the sweep's pointer/ref locals, self local, split zero init): the real CDefPointer local,
//   def-before-scheme declaration order (189), a `const vector&` / pointer local for Controls (at block top, after the
//   name, in the found block), a goto out of the block, a named bool result -- all the same rotation.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CActionInputControl;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector<T, A>& operator=(const vector<T, A>& other);    // 0x00411B90
};
}
class CControlsDef : public CParentDefClassBase {           // RTTI .?AVCControlsDef@@ : CParentDefClassBase
public:
    std::vector<CActionInputControl> Controls;              // +0x3c
};
template <class T> class CDefPointer {                      // the 4-byte def handle (engine/CDefPointer.h layout)
public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};
class CDefinitionManager { public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;   // 0x00410820 (GetDef<CControlsDef>)
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};
class CStartupDefinitionManager : public CDefinitionManager { public:
    static CStartupDefinitionManager& __fastcall Get();     // 0x0043368D
};
class CMainGameComponent;
extern unsigned char       g_bAggressiveModeAvailable;      // 0x013B85F3
extern unsigned char       g_KeyRedefinerFlag;              // 0x013B864F
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
extern int                 g_value_013b8760;                // 0x013B8760

static __forceinline bool GetControlsDef(const CCharString& name, CDefPointer<const CControlsDef>* def)
{
    if (g_pMainGameComponent) {
        CGameDefinitionManager::Get().GetDef(name, def);
        return true;
    }
    if (g_value_013b8760) {
        CStartupDefinitionManager::Get().GetDef(name, def);
        return true;
    }
    return false;
}

class CUserProfileManager {
public:
    unsigned char _pad00[0x54];
    std::vector<CActionInputControl> AssignedInputsPassive;     // +0x54
    std::vector<CActionInputControl> AssignedInputsAggressive;  // +0x60
    void ResetAssignedInputs();                             // 0x004085F0
    void ResetAssignedInputsWASD();
protected:
    void ApplyValues();                                     // 0x00407370
};

void CUserProfileManager::ResetAssignedInputsWASD()
{
    if (g_bAggressiveModeAvailable) {
        ResetAssignedInputs();
        return;
    }
    CControlsDef* scheme = 0;
    CDefPointer<const CControlsDef> def;
    {
        const char* text = g_KeyRedefinerFlag ? "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK" : "FABLE_PC_CONTROL_SCHEME_GDD_WASD";
        CCharString name(text, -1);
        if (GetControlsDef(name, &def)) {
            scheme = (CControlsDef*)def.Object;
            AssignedInputsPassive = scheme->Controls;
            AssignedInputsAggressive = scheme->Controls;
        }
    }
    if (scheme) {
        if (scheme->Release() == 0)
            scheme->Delete();
    }
    ApplyValues();
}
