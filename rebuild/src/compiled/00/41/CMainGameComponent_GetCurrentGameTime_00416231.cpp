// CMainGameComponent::GetCurrentGameTime() const -- the game clock (0x009E1BC0 on this) less the game start time at +0x60
// Leaf: ?GetCurrentGameTime@CMainGameComponent@@IBENXZ
// Identity (naming wave 11): Ego_r.pdb 0x00415496 + FableWin.pdb, protected const returning double; Ego_r order GetCurrentGameTime /
//   ConvertGTToWF / GetRenderInterpolate / GetPredictedTimeSinceLastRenderFrame = retail 0x00416231 / 0x0041623E / 0x00416251 / 0x00416258.
class CMainGameComponent { public:
    char pad[0x60];
    double gameTimeBase;                    // +0x60
protected:
    double GetCurrentGameTime() const;
};
extern double __fastcall ReadCurrentGameTime(CMainGameComponent* self);
double CMainGameComponent::GetCurrentGameTime() const {
    return ReadCurrentGameTime(const_cast<CMainGameComponent*>(this)) - gameTimeBase;
}
