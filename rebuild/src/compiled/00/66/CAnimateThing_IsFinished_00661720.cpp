// CAnimateThing::IsFinished() const -- the animation counts as finished when the queue says so or it was explicitly marked finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661720, length 32 bytes.
// Callees: CAnimateThing::IsAnimationFinished() const @ 0x0070C0E0 (manifest label; Ego_r
// ?IsAnimationFinished@CAnimateThing@@ABE_NXZ).
// Label correction: the manifest's "CTCWife::IsInLoveWithHero" is propagated (the CTCWife body is the landed 0x006CF900).
// `this` is the CAnimateThing that 0x0070C0E0 reads, and the same test is inlined at 0x006659FC, next to a call of
// SetAsFinished (0x0070D580) followed by `or byte ptr [edi+94h],1`: bit 0 of the byte at +0x94 is the "marked finished"
// flag. No code or data references this copy (an out-of-line instance of an inline accessor); the name is descriptive.
// Shape: `a || b` with a bool result gives the separate `xor eax,eax` / `mov eax,1` tails; the flag is a one-bit
// bitfield (`test byte ptr [esi+94h],1`).
class CAnimateThing { public:
    unsigned char _pad_00[0x94];
    unsigned char m_markedFinished : 1;                     // +0x94 bit 0 (set by SetAsFinished)
    bool IsAnimationFinished() const;                       // 0x0070C0E0
    bool IsFinished() const;
};

bool CAnimateThing::IsFinished() const
{
    return IsAnimationFinished() || m_markedFinished;
}