// CMainGameComponent::GetCurrentGameTime() const -- the game clock (0x009E1BC0 on this) less the game start time at +0x60
// Leaf: ?GetCurrentGameTime@CMainGameComponent@@IBENXZ
// Identity (naming wave 11): Ego_r.pdb 0x00415496 + FableWin.pdb, protected const returning double; Ego_r order GetCurrentGameTime /
//   ConvertGTToWF / GetRenderInterpolate / GetPredictedTimeSinceLastRenderFrame = retail 0x00416231 / 0x0041623E / 0x00416251 / 0x00416258.
// Name fix (agentNF6 2026-11-10): GFGetTime 0x009E1BC0 is ?GFGetTime@@YINXZ (Ego_r.pdb / FableWin.pdb: global __fastcall, double, no arguments).
class CMainGameComponent { public:
    char pad[0x60];
    double gameTimeBase;                    // +0x60
protected:
    double GetCurrentGameTime() const;
};
double __fastcall GFGetTime();                     // 0x009E1BC0 ?GFGetTime@@YINXZ
double CMainGameComponent::GetCurrentGameTime() const {
    return GFGetTime() - gameTimeBase;
}
