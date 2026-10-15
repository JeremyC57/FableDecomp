// CTCSpecialAbilities::OnKill() -- when the parent thing dies, strip its bullet-time component (interface type 0x92) if it has one
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A950, length 23 bytes; vtable 0x0126683C (CTCSpecialAbilities) slot 14,
// the CTCBase OnKill slot.
// Callees: CThing::RemoveTC(long interfaceType) @ 0x004C9840 (landed naming, see CCreatureAction_WaspQueenPrimaryAttack
// 0x008C4020).
// Layout: the thing's interface presence bitset at +0x20 (type 0x92 -> word 4 = +0x30, bit 18 = 0x40000; the same bit
// SetupBulletTime 0x0072A970 tests before adding "CTCBulletTime").
// Shape: the inlined HasTC probe is the raw masked word in the `if`; RemoveTC takes an argument, so no tail jump.
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of thing-component interface types
    void RemoveTC(long interfaceType);                      // 0x004C9840
};

struct CTCSpecialAbilities {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void OnKill();
};

void CTCSpecialAbilities::OnKill()
{
    if (ParentThing->m_presentBits[0x92 >> 5] & (1UL << (0x92 & 31)))
        ParentThing->RemoveTC(0x92);
}
