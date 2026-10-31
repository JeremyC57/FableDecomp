// CDrawSpiritsHealth::Draw() -- HEAD DRAFT (first block of 1395 bytes): mark the HUD drawn, read the GUI def, empty the
// Grade: DIFFER(80v1395) 490 diff lines (skeleton 3v1395/508): head draft (Drawn, GUI def, handle erase/resize 0x28, player index loop skipping the main player); per-player spirit layout not drafted (retry wave 2 sec2c)
// primitive handle vector and resize it to 0x28, then walk the players (index loop, count 0x20) skipping the main player;
// the x87 spirit-health layout per player is not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064E2CC, 1395 bytes; /Os frame `lea ebp,[esp-74h]`, 0x358 locals.
#pragma optimize("s", on)
class CThing { public: unsigned char _pad00[0x20]; };
class CThingPlayerCreature : public CThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; bool IsMainPlayer() const; };   // 0x00487DC0 / 0x00487520
class CPlayerManager { public: CPlayer& GetPlayer(long index); };       // 0x004498C0
class CPlayerGuiDef { public: unsigned char _pad[0x630]; };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };   // 0x00438570
class CEnginePrimitiveHandle { public: void* Data; };
class CRetailHandleVector { public: CEnginePrimitiveHandle* First; CEnginePrimitiveHandle* Last; CEnginePrimitiveHandle* End;
    void erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void resize(unsigned long count); };                                   // 0x00443F90
class CDrawSpiritsHealth {
public:
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                                     // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool Drawn;                                                         // +0x18
    unsigned char _pad19[0x1c - 0x19];
    CRetailHandleVector Handles;                                        // +0x1c
    void Draw();
};
void CDrawSpiritsHealth::Draw()
{
    Drawn = true;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    Handles.erase(Handles.First, Handles.Last);
    Handles.resize(0x28);
    float offset = 0.0f;
    long count = 0x20;
    unsigned long alpha = 0xff;
    for (long i = 0; i < count; ++i) {
        CThingPlayerCreature* creature = PPlayerManager->GetPlayer(i).GetPControlledCreature();
        if (PPlayerManager->GetPlayer(i).IsMainPlayer())
            continue;
        (void)creature;
    }
    (void)def; (void)offset; (void)alpha;
}
