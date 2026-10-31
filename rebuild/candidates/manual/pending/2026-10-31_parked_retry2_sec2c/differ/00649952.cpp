// CDrawBettingHUD::Draw() -- HEAD DRAFT (first block of 1337 bytes): fetch the hero's CTCGamePlayer (TC 0xD0) and, when it
// Grade: DIFFER(123v1337) 413 diff lines (skeleton 3v1337/434): head draft (hero CTCGamePlayer TC 0xD0, tavern game IsBetting, Drawn flag, GUI def); the screen-space layout and font calls not drafted (retry wave 2 sec2c)
// has a tavern game, tell the game (0x005E9C30); mark the HUD drawn and read the GUI def. The screen-space layout and font
// calls (0x006499C6..) are not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00649952, 1337 bytes; /Os ebp frame 0x9c.
#pragma optimize("s", on)
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key) const; };                  // 0x004365B0
class CTCBitset { public: unsigned long W[9]; bool test(long pos) const; };   // 0x00410DE0
class CThing { public: unsigned char _pad00[0x20]; CTCBitset UsingComponents; CVectorMapTC Components; };
class CThingPlayerCreature : public CThing { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };   // 0x00487DD0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };              // 0x00449970
class CTCTavernGame { public: bool IsBetting() const; };                // 0x005E9C30
class CTCGamePlayer { public: CTCTavernGame* GetGame() const; };         // 0x005EDC90
class CPlayerGuiDef { public: unsigned char _pad[0x630]; };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };   // 0x00438570
class CDrawBettingHUD {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                     // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool Drawn;                                                         // +0x18
    void Draw();
};
void CDrawBettingHUD::Draw()
{
    const CThingPlayerCreature* hero = PPlayerManager->GetMainPlayer().PeekPControlledCreature();
    const CTCGamePlayer* gamePlayer;
    long type = 0xd0;
    if (hero->UsingComponents.test(type)) {
        long key = type;
        const CVectorMapTC::value_type* it = hero->Components.LowerBound(key);
        if (it == hero->Components._M_finish || key < it->first)
            it = hero->Components._M_finish;
        gamePlayer = (const CTCGamePlayer*)it->second;
    }
    if (gamePlayer->GetGame())
        gamePlayer->GetGame()->IsBetting();
    Drawn = true;
    CPlayerGui::PeekGuiDef();
}
