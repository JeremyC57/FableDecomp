// CDisplayManager::ChangePresentationInterval(bool, long, bool) -- builds a CPresentationIntervalDesc from its three fields and applies it
// Leaf: ?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFF10, length 41 bytes (the 48-byte catalog row ends in int3 padding).
// Callees: CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc&) @ 0x009BFE70.
// Callers: 0x00418A35 and 0x00B2477A (`push 1 ; push 1 ; push <bool> ; call` on the display manager global).
// Label: the manifest's "CTCInventoryAbilities::AddLearnedExpression" is propagated; the callee and the callers
//        identify the class (the field order comes from the PDB CPresentationIntervalDesc: PresentImmediate +0,
//        PresentImmediateIfSyncMissed +1, VSyncCount +4).
// Shape: the descriptor is an 8-byte frame local filled by an inline constructor (the two bytes stored first,
//        the dword after the address push) and passed by reference.

struct CPresentationIntervalDesc {
    bool PresentImmediate;                        // +0x00
    bool PresentImmediateIfSyncMissed;            // +0x01
    long VSyncCount;                              // +0x04
    CPresentationIntervalDesc(bool presentImmediate, long vsyncCount, bool presentImmediateIfSyncMissed)
        : PresentImmediate(presentImmediate), PresentImmediateIfSyncMissed(presentImmediateIfSyncMissed), VSyncCount(vsyncCount) { }
};

struct CDisplayManager {
    void ChangePresentationInterval(const CPresentationIntervalDesc& desc);   // 0x009BFE70
    void ChangePresentationInterval(bool presentImmediate, long vsyncCount, bool presentImmediateIfSyncMissed);
};

void CDisplayManager::ChangePresentationInterval(bool presentImmediate, long vsyncCount, bool presentImmediateIfSyncMissed)
{
    CPresentationIntervalDesc desc(presentImmediate, vsyncCount, presentImmediateIfSyncMissed);
    ChangePresentationInterval(desc);
}
