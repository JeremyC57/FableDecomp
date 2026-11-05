// Leaf: ?CGameScriptInterface_SetToDisplayTutorialsDuringCutscenes@@YIXPAXHD@Z
struct GS2 { char pad[0xcc]; char fcc; };
class CUserProfileManager : public GS2 { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
void __fastcall CGameScriptInterface_SetToDisplayTutorialsDuringCutscenes(void* self, int _edx, char arg){ CUserProfileManager::Get().fcc = arg; }
// Stand-in (owner rule 2026-11-05): CGameScriptInterface_SetToDisplayTutorialsDuringCutscenes for CGameScriptInterface::SetToDisplayTutorialsDuringCutscenes; RTTI vtable slot ?SetToDisplayTutorialsDuringCutscenes@CGameScriptInterface@@UBEX_N@Z + PDB name set (egor/fablewin).
