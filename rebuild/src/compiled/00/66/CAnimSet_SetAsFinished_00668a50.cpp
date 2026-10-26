// CAnimSet::SetAsFinished(long framesToFadeOver) -- marks the set finished once and sets the per-frame alpha
// change so the current alpha fades out over `framesToFadeOver` frames (at least one).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00668A50, length 42 bytes. No callees.
// Layout: retail CAnimSet is the PDB layout minus 8 (the two std::vectors at +4 and +0x10 are 12 bytes each,
// not 16): CurrentAlpha +0x2c (PDB 52), ChangeAlphaBy +0x38 (PDB 64), the bit-field byte +0x51 (PDB 89):
// Looping:1, Finished:1, Forwards:1, HadUpdate:1, OwnedByMainAnim:1. The clamp writes the parameter slot
// in place (`mov [esp+4],eax` then `fild [esp+4]`), and the flag byte is read once, or'ed and stored.
class CAnimSet { public:
    unsigned char _pad0[0x2c];
    float CurrentAlpha;                                  // +0x2c
    unsigned char _pad1[0x8];
    float ChangeAlphaBy;                                 // +0x38
    unsigned char _pad2[0x15];
    unsigned char Looping : 1;                           // +0x51
    unsigned char Finished : 1;
    unsigned char Forwards : 1;
    unsigned char HadUpdate : 1;
    unsigned char OwnedByMainAnim : 1;
    void SetAsFinished(long framesToFadeOver);
};

void CAnimSet::SetAsFinished(long framesToFadeOver)
{
    if (!Finished) {
        Finished = 1;
        if (framesToFadeOver < 1)
            framesToFadeOver = 1;
        ChangeAlphaBy = CurrentAlpha / (float)framesToFadeOver;
    }
}