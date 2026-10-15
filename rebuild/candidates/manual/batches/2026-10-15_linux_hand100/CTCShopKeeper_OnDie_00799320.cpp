// CTCShopKeeper::OnDie() -- when the dying owner is flagged (+0x6c bit 4) and the keeper has a shop, tell the shop it has no shopkeeper any more
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799320, length 41 bytes (+7 int3 padding).
// Callees: CTCShopKeeper::GetShopTC @ 0x00798F50 (called twice: the test and the call), CTCShop::SetShopKeeper
// (CThingCreatureBase*) @ 0x007751A0. Vtable: CTCShopKeeper 0x0126C154 slot 13.
// Shape: `test byte ptr [eax+6Ch],10h` on the owner (+4) read through a register; nested ifs with one shared exit.
struct CThingCreatureBase;

struct CThing {
    unsigned char _pad[0x6c];
    unsigned char m_flags;                   // +0x6c
};

struct CTCShop {
    void SetShopKeeper(CThingCreatureBase* shopKeeper);   // 0x007751A0
};

struct CTCShopKeeper {
    void*   m_vtable;                        // +0
    CThing* m_pOwner;                        // +4
    CTCShop* GetShopTC();                    // 0x00798F50
    void OnDie();
};

void CTCShopKeeper::OnDie()
{
    if ((m_pOwner->m_flags & 0x10) && GetShopTC())
        GetShopTC()->SetShopKeeper(0);
}
