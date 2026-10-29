// CTCEnemy::OnInitialActivate @ 0076c830
class CTCEnemy;

// the player GUI at 0x013B8790; CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (Ego_r.pdb / FableWin.pdb, chunk 563)
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool force);   // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;          // 0x013B8790

// base OnInitialActivate (tail jmp 0x686870), __fastcall(this)
void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy);

void __fastcall OnInitialActivate(CTCEnemy* enemy)
{
    g_pPlayerGui->UpdateMiniMapMarkers(false);
    CTCEnemy_base_OnInitialActivate(enemy);
}