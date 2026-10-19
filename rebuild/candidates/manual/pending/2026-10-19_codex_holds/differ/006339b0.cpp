// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/63/CPlayer_UninitModeBerserk_006339b0.cpp, flags , pass pattern 006339b0_TEST PASS
// CPlayer::UninitModeBerserk(EPlayerMode, CProcessedInput*) -- shuts down all active input processes used by Berserk mode.
// Retail 0x006339B0, 176 bytes. The selected scoped pointers occupy their retail offsets and all call the shared cleanup routine 0x00A0D2D0.
struct CProcessedInput;
enum EPlayerMode { PlayerModeDummy = 0 };
struct CInputProcessCleanup { void Shutdown(); };
struct CPlayer {
    unsigned char pad104[0x104]; void* ControlCreature; // +104
    unsigned char pad108[0x48]; void* CreatureMovement; // +150
    unsigned char pad154[4]; void* Block; // +158
    void* Debug; // +15c
    void* Quick; // +160
    void* Combat; // +164
    void* Special; // +168
    void* RightStick; // +16c
    void* RightLook; // +170
    void* Cycle; // +174
    unsigned char pad178[4]; void* Jump; // +17c
    unsigned char pad180[4]; void* UseRanged; // +184
    void* ActivateZ; // +188
    unsigned char pad18c[0x14]; void* SetRanged; // +1a0
    void UninitModeBerserk(EPlayerMode, CProcessedInput*);
};
void CPlayer::UninitModeBerserk(EPlayerMode, CProcessedInput*) {
    reinterpret_cast<CInputProcessCleanup*>(ControlCreature)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(ActivateZ)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(RightStick)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(RightLook)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Cycle)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(CreatureMovement)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Block)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Debug)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Quick)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Combat)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(UseRanged)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Special)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(Jump)->Shutdown();
    reinterpret_cast<CInputProcessCleanup*>(SetRanged)->Shutdown();
}