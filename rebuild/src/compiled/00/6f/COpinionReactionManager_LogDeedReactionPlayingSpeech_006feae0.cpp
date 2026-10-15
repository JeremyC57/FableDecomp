// COpinionReactionManager::LogDeedReactionPlayingSpeech(long deedType, bool playing) -- count a deed's reaction speech starting (+1) or ending (-1), unless that deed's counter is disabled (-1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEAE0, length 45 bytes (`ret 8`).
// Layout: DeedReactionSpeechPlaying (long[92]) at retail +0x5A4 (PDB +0x5AC; retail is 8 bytes lower, see 0x00681500).
// Shape: the slot is indexed `[ecx+esi*4+5A4h]` twice (no pointer local); the delta is the ternary `playing ? 1 : -1`
// lowered to `setne dl ; lea edx,[edx+edx-1]` with the flag byte read into bl inside the taken arm.
// Leaf: LogDeedReactionPlayingSpeech
class COpinionReactionManager {
public:
    unsigned char _pad000[0x5A4];
    long          DeedReactionSpeechPlaying[92];   // +0x5A4 (PDB +0x5AC)

    void LogDeedReactionPlayingSpeech(long deedType, bool playing);
};

void COpinionReactionManager::LogDeedReactionPlayingSpeech(long deedType, bool playing)
{
    if (DeedReactionSpeechPlaying[deedType] != -1)
        DeedReactionSpeechPlaying[deedType] += playing ? 1 : -1;
}