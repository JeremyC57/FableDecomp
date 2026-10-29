// CUserProfileManager::Apply() -- push the live option values to the engine, then the video values when the profile
// asks for them on load
// Leaf: ?Apply@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040BC80, 25 bytes, thiscall, plain `ret` (the second call is a tail jump).
// Identity: Ego_r.pdb / FableWin.pdb ?Apply@CUserProfileManager@@QAEXXZ (catalog label agrees); the body calls ApplyValues
//   0x00407370 (?ApplyValues@CUserProfileManager@@IAEXXZ) and, when ApplyVideoValuesOnLoad (+0x33, struct_layouts_egor
//   +0x33) is set, 0x0040A7F0 = ApplyVideoValues (?ApplyVideoValues@CUserProfileManager@@IAEXXZ: Ego_r.pdb lays it out right
//   after Save, as retail does: Save 0x0040A740 is 175 bytes and ends at 0x0040A7EF). Callers: CMainGameComponent::Run
//   0x004189C2, CTCInGameMenu::ConfirmedVideoOptionsNotAccepted 0x0047DA50, CFrontEndManager::DoPressStart 0x00599D5C.
//   Was landed as free __fastcall stand-ins (UPM_Prep / UPM_Commit).
class CUserProfileManager {
public:
    unsigned char _pad00[0x33];
    bool          ApplyVideoValuesOnLoad;       // +0x33
    void Apply();
protected:
    void ApplyValues();                         // 0x00407370
    void ApplyVideoValues();                    // 0x0040A7F0
};

void CUserProfileManager::Apply()
{
    ApplyValues();
    if (ApplyVideoValuesOnLoad)
        ApplyVideoValues();
}
