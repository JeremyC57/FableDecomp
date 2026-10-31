// CDrawTargetInfo::Draw(const float& timePassed) -- HEAD DRAFT (first block of 1426 bytes): update both fades by the time
// Grade: DIFFER(83v1426) 460 diff lines (skeleton 3v1426/478): head draft (two fade updates, hero alive, Drawn, GUI def target-entry count); per-entry loop not drafted (retry wave 2 sec2c)
// passed, and with the hero alive mark the HUD drawn and return when the GUI def's target-entry vector (0x5C-byte entries,
// +0x7FC) is empty; the per-entry loop (button-map icons, demon-door face def-name compares) is not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064B8B3, 1426 bytes; /Os ebp frame 0x3c.
#pragma optimize("s", on)
class CThing { public: unsigned char _pad00[0x20]; };
class CThingPlayerCreature : public CThing { };
bool __fastcall IS_THING_ALIVE(const CThingPlayerCreature* const& thing);   // 0x0043DBF0
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };              // 0x00449970
struct STargetEntry { unsigned char _body[0x5c]; };
class CPlayerGuiDef { public: unsigned char _pad[0x7fc]; STargetEntry* TargetsFirst; STargetEntry* TargetsLast;
    unsigned long TargetCount() const { return TargetsLast - TargetsFirst; } };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };   // 0x00438570
class CFadeInFadeOutBase { public: void Update(float timePassed); };   // 0x00434870
class CDrawTargetInfo {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                     // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool Drawn;                                                         // +0x18
    unsigned char _pad19[0x28 - 0x19];
    CFadeInFadeOutBase FadeA;                                           // +0x28
    unsigned char _pad29[0x44 - 0x29];
    CFadeInFadeOutBase FadeB;                                           // +0x44
    void Draw(const float& timePassed);
};
void CDrawTargetInfo::Draw(const float& timePassed)
{
    FadeA.Update(timePassed);
    FadeB.Update(timePassed);
    const CThingPlayerCreature* hero = PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    long index = 0;
    Drawn = true;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    if (def.TargetCount() == 0)
        return;
    (void)index;
}
