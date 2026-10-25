// CThingSoundEmitter::ReleaseLoop(unsigned long sampleID) -- while the game state exists and the emitter is not dead, release the looping sample through the emitter's sound object
// Leaf: ?ReleaseLoop@CThingSoundEmitter@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008368F0, 32 bytes, `ret 4` (the virtual call is a tail `jmp [eax+34h]`).
// Naming: Ego_r.pdb's CThingSoundEmitter TU defines ?StopSpecificSample@CThingSoundEmitter@@QAEXK@Z immediately followed by
//   ?ReleaseLoop@CThingSoundEmitter@@QAEXK@Z (two 16-byte donor bodies in that order); retail has the landed
//   StopSpecificSample 0x008368D0 (sound-object slot 9) immediately followed by this byte twin (slot 13), inside the
//   CThingSoundEmitter TU (OnCreate 0x00836740 .. Create 0x008369F0); both are the only (unsigned long) void members
//   of the class. FINDINGS chunk 401. Was landed as the generic "ConditionalVirtualDispatch_OnLevelUnload".
// Layout: as the landed sibling 0x008368D0: CThing dead bit = bit 0 of +0x91, the sound object +0x98 (retail-only
//   member); g_pGameState 0x013B8394.
// Shape: one inline `g && !Dead` guard; the call is the last statement, so it becomes the tail jump.
extern void* g_pGameState;                               // 0x013B8394

struct CSoundPlayer {                                    // the emitter's sound object at +0x98
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void StopSpecificSample(unsigned long sampleID);   // slot 9 (+0x24)
    virtual void v010(); virtual void v011(); virtual void v012();
    virtual void ReleaseLoop(unsigned long sampleID);          // slot 13 (+0x34)
};

class CThingSoundEmitter {
public:
    unsigned char  _pad00[0x91];
    unsigned char  Dead : 1;                             // +0x91 bit 0 (CThing flags, PDB +0xa1)
    unsigned char  _flags91_1 : 7;
    unsigned char  _pad92[0x6];
    CSoundPlayer*  m_pSoundPlayer;                       // +0x98 (retail-only member, not in Ego_r.pdb)
    bool IsActive() const { return g_pGameState && !Dead; }
    void ReleaseLoop(unsigned long sampleID);
};

void CThingSoundEmitter::ReleaseLoop(unsigned long sampleID)
{
    if (IsActive())
        m_pSoundPlayer->ReleaseLoop(sampleID);
}
