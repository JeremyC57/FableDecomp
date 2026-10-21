// NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo(CDrawContext* context) -- when quests are listed, find the first registered quest-info element of type 6 (the mini-game bar) in the element list (others are deregistered), mark the panel drawn, anchor the mini-game panel from the gui def and draw its sprites/text through the context drawer
// Grade: DIFFER(129v399) -- parked at once: fistp/fild rounding (lane gap); only the element walk is reconstructed, the sprite tail is behind a stand-in helper
// Leaf: DrawMiniGameInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D404, length 399 bytes (frame pointer), `ret 4`.
// Callees: the element's virtual slot 0xc (GetType), CQuestInfoRegistry::Deregister @ 0x00440D70 (two words of the element), PeekGuiDef
// @ 0x00438570, CDrawBase::AnchorPoint2D @ 0x00643CA1 (`__fastcall` out, def offset), the remaining sprite/text calls of the retail tail.
// Layout: CDrawQuestInfo QuestCount +0x3c, Elements list header +0x58 (STLport list: next +0, prev +4, element +8), Drawn +0x18.
// Shape: best-effort -- parked at once: the retail body rounds the panel position with the `fistp/fild` frame-slot helper (lane gap).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };
struct CQuestInfoElement {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long GetType();                // slot 0xc
    unsigned char _pad[0x10 - 4];
    long RegistryHandle;                   // +0x10
    long RegistryKey;                      // +0x14
};
struct SElementNode { SElementNode* next; SElementNode* prev; CQuestInfoElement* element; };
void __cdecl DeregisterQuestInfo(long key, long handle);   // 0x00440D70
struct CPlayerGuiDef { unsigned char _pad[0x56c]; C2DVector MiniGameOffset; };
CPlayerGuiDef* __fastcall PeekGuiDef();   // 0x00438570
void __fastcall AnchorPoint2D(C2DVector* out, const C2DVector* in);   // 0x00643CA1
struct CDrawContext;
struct CDrawQuestInfo {
    unsigned char _pad0[0x18];
    bool Drawn;                            // +0x18
    unsigned char _pad1[0x3c - 0x19];
    long QuestCount;                       // +0x3c
    unsigned char _pad2[0x58 - 0x40];
    SElementNode* Elements;                // +0x58
    void DrawMiniGamePanel(CDrawContext* context, CQuestInfoElement* element, CPlayerGuiDef* def, const C2DVector* position);
    void DrawMiniGameInfo(CDrawContext* context);
};

void CDrawQuestInfo::DrawMiniGameInfo(CDrawContext* context)
{
    CQuestInfoElement* miniGame = 0;
    if (QuestCount > 0) {
        SElementNode* header = Elements;
        for (SElementNode* it = header->next; it != header; it = it->next) {
            CQuestInfoElement* element = it->element;
            if (element->GetType() == 6)
                miniGame = element;
            else
                DeregisterQuestInfo(element->RegistryKey, element->RegistryHandle);
        }
        if (miniGame) {
            Drawn = true;
            CPlayerGuiDef* def = PeekGuiDef();
            C2DVector position;
            AnchorPoint2D(&position, &def->MiniGameOffset);
            DrawMiniGamePanel(context, miniGame, def, &position);
        }
    }
}
