// CV_SingingStonesScript::WatchForRegionLeaving() -- script thread: each time the hero has been in Witchwood2 and leaves it, kill every spawned singing-stone effect and empty the effect list
// Leaf: WatchForRegionLeaving
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED2A50, length 344 bytes, plain `ret`.
// Identity: CV_SingingStonesScript::Main (0x00ED10A0) spawns this thread as "WatchForRegionLeaving" (string 0x012F2B8C,
// CSpawnedFunc record at 0x00ED1357); Ego_r.pdb ?WatchForRegionLeaving@CV_SingingStonesScript@NScript@@QAEXXZ. The
// manifest's "CQ_BanditCampScript::WatchForEndOfScript" label is propagated.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, KillAllThingsInVector (__fastcall, 0x00CBED82, as the landed CQ_WizardBattleScript::CleanUp 0x00E1D690),
// vector<CScriptThing>::erase(iterator, iterator) 0x008AC9D0; through the quest's interface (+0x40): NewScriptFrame
// (slot 7), IsRegionLoaded (12).
// Strings: "Witchwood2" 0x012DCEF8.
// Layout: Ego_r.pdb CV_SingingStonesScript - 0x14: EffectList (STLport vector<CScriptThing>, 12 bytes) +0x7C.
// Shape: two WAIT_UNTIL(cond) = `while (!(cond))` waits: the first on IsRegionLoaded (`neg ; sbb ; inc`), the second
// on `!IsRegionLoaded` (the double negation gives `test al,al ; setne bl`); the list is cleared with an inline
// `erase(begin(), end())` (end pushed first).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public: unsigned char _body[12]; };

template <class T> struct vector {                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void __fastcall KillAllThingsInVector(CGameScriptInterface* gsi, vector<CScriptThing>* things, bool b);   // 0x00CBED82

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_SingingStonesScript : public CScriptBase {
public:
    CGameScriptInterface*  GSI;                         // +0x40
    void*                  Data;                        // +0x44
    unsigned char          _pad48[0x7c - 0x48];
    vector<CScriptThing>   EffectList;                  // +0x7c (PDB +0x90)
    void WatchForRegionLeaving();
};
void CV_SingingStonesScript::WatchForRegionLeaving()
{
    while (!IsActiveThreadTerminating()) {
        while (!GSI->IsRegionLoaded("Witchwood2")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!(!GSI->IsRegionLoaded("Witchwood2"))) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        KillAllThingsInVector(GSI, &EffectList, false);
        EffectList.clear();
        GSI->NewScriptFrame();
    }
}
}