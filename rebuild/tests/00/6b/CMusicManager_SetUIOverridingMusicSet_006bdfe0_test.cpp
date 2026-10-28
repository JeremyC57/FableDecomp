#include <stdio.h>
class CSoundDef;
class CMusicManager {
public:
    class CState {                              // 24 bytes
    public:
        long RegionSet;                         // +0x00
        long OverrideSet;                       // +0x04
        long UIOverrideSet;                     // +0x08
        long CinematicOverrideSet;              // +0x0C
        long MapCellSet;                        // +0x10
        bool Paused;                            // +0x14
        bool Danger;                            // +0x15
        CState();
    };
    class CFadeTimes;
    CState CurrentState;                        // +0x00
    void SetUIOverridingMusicSet(long value);
protected:
    void InternalUpdate(const CState& new_state, bool force_play);   // 0x006BD9B0
};

void CMusicManager::SetUIOverridingMusicSet(long value)
{
    CState new_state = CurrentState;
    new_state.CinematicOverrideSet = value;
    InternalUpdate(new_state, false);
}

static int calls=0,bad=0; static CMusicManager* wanted; static CMusicManager::CState seen;
CMusicManager::CState::CState() { RegionSet=1; OverrideSet=2; UIOverrideSet=3; CinematicOverrideSet=4; MapCellSet=5; Paused=true; Danger=false; }
void CMusicManager::InternalUpdate(const CState& state,bool force) { ++calls; if(this!=wanted || force) bad=1; seen=state; }
static bool same(const CMusicManager::CState& a,const CMusicManager::CState& b) { return a.RegionSet==b.RegionSet && a.OverrideSet==b.OverrideSet && a.UIOverrideSet==b.UIOverrideSet && a.CinematicOverrideSet==b.CinematicOverrideSet && a.MapCellSet==b.MapCellSet && a.Paused==b.Paused && a.Danger==b.Danger; }
int main() { CMusicManager m; wanted=&m; CMusicManager::CState before=m.CurrentState, expected=before;
expected.CinematicOverrideSet=-73;
m.SetUIOverridingMusicSet(-73);
if(bad || calls!=1 || !same(seen,expected) || !same(m.CurrentState,before)) { puts("006bdfe0_TEST FAIL"); return 1; }
puts("006bdfe0_TEST PASS"); return 0; }