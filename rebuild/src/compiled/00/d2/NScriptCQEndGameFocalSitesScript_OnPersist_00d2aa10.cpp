// NScript::CQ_EndGameFocalSitesScript::OnPersist(CPersistContext& context) -- persist QuestStartScreenShown (default false)
// Leaf: ?OnPersist@CQ_EndGameFocalSitesScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2AA10, 36 bytes, thiscall, `ret 4` (virtual, level-script vtable slot 4).
// Identity (proven): slot 4 of the RTTI-less level-script vtable 0x012CBEB4, which the landed NScript::CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript 0x00D2A820 stores; level-script
//   slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase; slot 4 of 0x012CBA58 is the landed
//   CQ_EndGameScript::OnPersist 0x00D29830). Ego_r.pdb has ?OnPersist@CQ_EndGameFocalSitesScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::Transfer<bool> 0x004045C0; key 0x012CBFD8.
// Layout: Ego_r.pdb CQ_EndGameFocalSitesScript members at Ego_r offset - 0x14: QuestStartScreenShown +0x83.
// Shape: the `false` default is a byte in the pushed-ecx slot (`lea eax,[esp+3]`).
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CCharString {
public:
    void* PData;
};
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long          CurrentFocalSiteRegion;               // +0x48
    CCharString   TeleportHSPs[5];                      // +0x4c
    CCharString   RegionDialogueNames[5];               // +0x60
    float         TeleportPosition[3];                  // +0x74 C3DVector
    bool          MissionFailed;                        // +0x80
    bool          JackLeaves;                           // +0x81
    bool          EndCutScenePlayed;                    // +0x82
    bool          QuestStartScreenShown;                // +0x83
    virtual void OnPersist(CPersistContext& context);
};

void CQ_EndGameFocalSitesScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false);
}
}