// CVtable_012B5870::SetSign8C(long mode) -- select -1.0 for mode 1 and +1.0 otherwise; when that differs from the stored sign at +0x8c, store it and raise the dirty bits 0xBF8000 at +0xa4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D310, length 69 bytes (80 with int3 padding).
// Leaf: SetSign8C
// Callees: none. Float pool: -1.0f @ 0x0122DEE0 (mode == 1), 1.0f @ 0x0122DED8.
// Identity: the manifest label "CParticleMorphs::CEntry::SetMorphScale" is propagated (PDB CParticleMorphs::CEntry
// is at most 36 bytes and has nothing at +0x8c/+0xa4). The only reference is the thunk 0x00C33600
// (`mov ecx,0x143BB58 ; jmp 0x00C3D310`), i.e. a member call on the static object at 0x0143BB58 -- the no-RTTI
// class with vtable 0x012B5870 that the landed CVtable_012B5870::GetPair80 (0x00C3D2D0) and Cleanup (0x00C3D160)
// model; its neighbour 0x00C3D2F0 is the same "store if changed, then OR a dirty bit into +0xa4" setter for the
// word at +0x88. No PDB class matches, so the class keeps the vtable name and the members are named by offset.
// Shape: the selected constant is evaluated first (`cmp ; jne ; fld [-1] ; jmp ; fld [1]`), compared with the
// member (`fld st(1) ; fucompp ; test ah,44h ; jnp` = equal -> pop and return), then the dirty word is loaded
// before the float store and OR-ed after it.
struct CVtable_012B5870 {
    virtual void v00();
    unsigned char _body[0x84];                                            // +0x04..+0x88
    long          m_value88;                                              // +0x88
    float         m_sign8C;                                               // +0x8c
    unsigned char _pad90[0x14];                                           // +0x90..+0xa4
    unsigned long m_dirtyFlags;                                           // +0xa4
    void SetSign8C(long mode);
};

void CVtable_012B5870::SetSign8C(long mode)
{
    float sign = mode == 1 ? -1.0f : 1.0f;
    if (sign != m_sign8C) {
        m_sign8C = sign;
        m_dirtyFlags |= 0xbf8000;
    }
}