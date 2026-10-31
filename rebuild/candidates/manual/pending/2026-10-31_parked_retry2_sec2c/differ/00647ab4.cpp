// CDrawHealth::Draw() -- HEAD DRAFT (first block of 1910 bytes): with the hero (PeekPControlledCreature path 0x00487DC0) alive, mark
// Grade: DIFFER(87v1910) 647 diff lines (skeleton 3v1910/663): head draft (hero alive guard, Drawn, GUI def, hero TC 4); health bar and OBJECT_RESURRECTION_PHIAL layout not drafted (retry wave 2 sec2c)
// the HUD drawn, read the GUI def and fetch the hero's TC 4 (CTCHeroStats); the x87 bar layout is not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00647AB4, 1910 bytes; /Os ebp frame.
#pragma optimize("s", on)
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key) const; };                  // 0x004365B0
class CTCBitset { public: unsigned long W[9]; bool test(long pos) const; };   // 0x00410DE0
class CThing { public: unsigned char _pad00[0x20]; CTCBitset UsingComponents; CVectorMapTC Components; };
class CThingPlayerCreature : public CThing { };
bool __fastcall IS_THING_ALIVE(const CThingPlayerCreature* const& thing);   // 0x0043DBF0
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };              // 0x00449970
class CPlayerGuiDef { public: unsigned char _pad[0x630]; };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };   // 0x00438570
class CTCHeroStats;
class CDrawHealth {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                     // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool Drawn;                                                         // +0x18
    void Draw();
};
void CDrawHealth::Draw()
{
    const CThingPlayerCreature* hero = PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    Drawn = true;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    CTCHeroStats* stats;
    if (hero->UsingComponents.test(4)) {
        long key = 4;
        const CVectorMapTC::value_type* it = hero->Components.LowerBound(key);
        if (it == hero->Components._M_finish || key < it->first)
            it = hero->Components._M_finish;
        stats = (CTCHeroStats*)it->second;
    }
    (void)def; (void)stats;
}
