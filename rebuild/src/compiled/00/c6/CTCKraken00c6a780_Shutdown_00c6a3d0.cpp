// CTCKraken_00c6a780::Shutdown() -- clear the +0x294 flag, free and null the +0x280 block, release the base block, clear the +0x25C holder
// Retail 0x00C6A3D0, 58 bytes. Callers: 0x00C53B06 (a jump), 0x00C54179, 0x00C6AEBE. It is the
// same sequence the landed destructor 0x00C6A780 (CTCKraken_00c6a780, classes named by address)
// runs before its member destructors, so the model is that file's; the name is descriptive (the
// manifest label "CTCInventoryItem::OnDie" is propagated).
// Shape: m_294 = 0; a non-null m_pBlock (+0x280) goes to the module's `free` 0x00C3C370 (`__cdecl`,
// `add esp,4`) and is nulled; CCountedBlock::ReleaseBlock 0x00C77470 on `this`; the holder at +0x25C
// is cleared by a tail jump to CHolder::Clear 0x00C75000.

// Leaf: Shutdown

void __cdecl Free_00c6a780(void* block);           // 0x00C3C370: the module's `free`

struct CHolder_00c6a780 {                          // +0x25c, 0x24 bytes
    unsigned char _body[0x24];
    void Clear();                                  // 0x00C75000
};

struct CCountedBlock_00c6a780 {                    // the base, 0x28 bytes
    virtual void v00();
    unsigned char _body[0x24];
    void ReleaseBlock();                           // 0x00C77470
};

struct CTCKraken_00c6a780 : CCountedBlock_00c6a780 {
    unsigned char _body_28[0x234];                 // +0x28 .. +0x25C (members as the landed destructor)
    CHolder_00c6a780 m_25c;                        // +0x25C
    void* m_pBlock;                                // +0x280
    long m_284;
    long m_288;
    long m_28c;
    long m_290;
    unsigned char m_294;                           // +0x294
    void Shutdown();
};

void CTCKraken_00c6a780::Shutdown()
{
    m_294 = 0;
    if (m_pBlock) {
        Free_00c6a780(m_pBlock);
        m_pBlock = 0;
    }
    ReleaseBlock();
    m_25c.Clear();
}