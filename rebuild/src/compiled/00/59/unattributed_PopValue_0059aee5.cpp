// _unattributed::PopValue_0059aee5 -- pops one dword from a block stack: when the cursor sits at the block start the previous block is restored, otherwise the cursor steps back
// Leaf: PopValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AEE5, length 20 bytes (unaligned, no padding: the 0x595000.. module is
// built without function alignment; seven callers, e.g. 0x00595268, 0x005967E9).
// Callees: the block-restore @ 0x0059AD35 (frees the current block, re-reads the previous one from the block table at +0x1c,
// recomputes +0x14 start / +0x18 end / +0x10 cursor; manifest label "CTCCarryable::OnKill" is propagated noise) as a tail jump.
// Label: the manifest's "CCreatureAction_FireMissileWeapon::FrameUpdate" cannot be right: +0x10/+0x14 of a creature action are
// TotalTime/ActionTime and the body is a cursor/limit pair. `this` layout: +0x10 cursor, +0x14 current block start.
struct CBlockStack {
    unsigned char _pad[0x10];
    long* m_cursor;                         // +0x10
    long* m_blockStart;                     // +0x14
    void PopBlock();                        // 0x0059AD35
    void PopValue();
};

void CBlockStack::PopValue()
{
    if (m_cursor != m_blockStart)
        m_cursor = m_cursor - 1;
    else
        PopBlock();
}