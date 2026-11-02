// Behaviour test for CTCKraken::StartLowAttackBeam (0x007FC7A0): the beam delay is DelayBeforeLowBeam seconds in frames (truncated), then SetMode(4)
// stores the mode and starts the beam effect.
#include <cstdio>
class CMainGameComponent {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponent::ConstantFPS = 30;
class CKrakenDef { public: float DelayBeforeLowBeam; float DelayBeforeHighBeam; };
class CTCKraken {
public:
    enum EKrakenMode { };
    const CKrakenDef* PKrakenDef;
    long DelayBeforeBeam;
    EKrakenMode Mode;
    int beamStarts, killed, inits;
    long modeAtBeamStart;
    void StartBeamEffect() { ++beamStarts; modeAtBeamStart = (long)Mode; }
    void KillBeamEffect() { ++killed; }
    void KillTentacles() { ++killed; }
    void InitTentaclesOnly() { ++inits; }
    void InitTentaclesAndHead() { ++inits; }
    void SetMode(EKrakenMode mode)
    {
        Mode = mode;
        switch (mode) {
        case 1: InitTentaclesOnly(); break;
        case 2: InitTentaclesAndHead(); break;
        case 3: KillBeamEffect(); KillTentacles(); break;
        case 4:
        case 5: StartBeamEffect(); break;
        }
    }
    void StartLowAttackBeam();
};
void CTCKraken::StartLowAttackBeam()
{
    DelayBeforeBeam = (long)(PKrakenDef->DelayBeforeLowBeam * CMainGameComponent::GetConstantFPS());
    SetMode((EKrakenMode)4);
}
int main()
{
    CKrakenDef def; def.DelayBeforeLowBeam = 2.5f; def.DelayBeforeHighBeam = 1.25f;
    CTCKraken k; k.PKrakenDef = &def; k.DelayBeforeBeam = 0; k.Mode = (CTCKraken::EKrakenMode)0;
    k.beamStarts = k.killed = k.inits = 0; k.modeAtBeamStart = 0;
    k.StartLowAttackBeam();
    if (k.DelayBeforeBeam != 75 || (long)k.Mode != 4 || k.beamStarts != 1 || k.modeAtBeamStart != 4 || k.killed || k.inits) {
        std::printf("007fc7a0_TEST FAIL\n"); return 1;
    }
    std::printf("007fc7a0_TEST PASS\n");
    return 0;
}