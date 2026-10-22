// CPlayerGui::DrawInventory(CDrawWorld* world) -- for the controlled hero (alive), ask each inventory-screen component it carries (interface types 0x11, 0x12, 0x13, 0x6f, 0x16, 0x19, 0x17, 0x14, 0x18, 0x15, 0x1b, 0xc6, 0xfc, 0x10e) to draw itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438CB0, length 832 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetControlledCreature 0x00487DC0, LowerBound 0x0040F020
// (the inlined GetInterface, bitset at +0x20, map at +0x44), the component's virtual slot 11 (+0x2c).
// Layout: retail CPlayerGui PlayerManager +0x08 (as the landed UpdateRenown 0x00439E00); CThing bitset/map as the landed
// GiveHeroGold 0x00435000.
// Shape: the key shares the dead argument slot (the argument lives in ebx); the presence test is the raw bitset word
// AND mask in the `if` (a bool-returning `!= 0` helper gives `shr ; test al,1`); the entry pick is one ternary.
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void DrawInventory(void* world);               // slot 11 (+0x2c) (descriptive)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                  // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                        // +0x20
    CInterfaceMap m_interfaces;                            // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_statusFlags;                           // +0x91 (bit 0: dead)
    __forceinline CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CPlayer {
    CThing* GetControlledCreature();                       // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                              // 0x00449970
};
struct CPlayerGui {
    unsigned char   _pad000[8];
    CPlayerManager* PlayerManager;                         // +0x08
    void DrawInventory(void* world);
};

#define DRAW_TC(t) if (hero->m_presentBits[(t) >> 5] & (1UL << ((t) & 31))) hero->GetInterface(t)->DrawInventory(world)

void CPlayerGui::DrawInventory(void* world)
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (!hero || (hero->m_statusFlags & 1))
        return;
    DRAW_TC(0x11);
    DRAW_TC(0x12);
    DRAW_TC(0x13);
    DRAW_TC(0x6f);
    DRAW_TC(0x16);
    DRAW_TC(0x19);
    DRAW_TC(0x17);
    DRAW_TC(0x14);
    DRAW_TC(0x18);
    DRAW_TC(0x15);
    DRAW_TC(0x1b);
    DRAW_TC(0xc6);
    DRAW_TC(0xfc);
    DRAW_TC(0x10e);
}
