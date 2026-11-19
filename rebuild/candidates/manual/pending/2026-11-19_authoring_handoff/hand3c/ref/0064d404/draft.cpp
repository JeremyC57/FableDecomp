// NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo(CDrawContext* context) -- when quests are listed, find the first registered quest-info element of type 6 (the mini-game bar) in the element list (others are deregistered), mark the panel drawn, anchor the mini-game panel from the gui def and draw its sprites/text through the context drawer
// Grade: DIFFER(393v399) 34 diff lines (parked stub 129v399/144): the whole retail tail written out (no fistp in retail: the parked 'lane gap' was wrong): line-height offset from the +0x14 metrics vslot 0x60 or +20, the mini-game element's Draw (slot 0, float height kept on the x87 stack), the child elements walked with GetElementByIndex 0x00644FC5, then for panel type 6 the framed box via DrawBox 0x00643EB2 (min height +0x580, 16-unit margins); the element list header is re-read every iteration. Left: 0x00440D70 is a thiscall on the element's +0x10 registration words (ret 8, args +0x10 / +0x14 -- modelling it so moves this out of ebx: 394/56), height spilled to [ebp-4] where retail keeps it in st(1) across the calls. Field / method names descriptive (catalog: GetElementByIndex, DrawBox) (retry wave 2 sec2c)
// Was: DIFFER(129v399) -- parked at once: fistp/fild rounding (lane gap); only the element walk is reconstructed, the sprite tail is behind a stand-in helper
// Leaf: DrawMiniGameInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D404, length 399 bytes (frame pointer), `ret 4`.
// Callees: the element's virtual slot 0xc (GetType), CQuestInfoRegistry::Deregister @ 0x00440D70 (two words of the element), PeekGuiDef
// @ 0x00438570, CDrawBase::AnchorPoint2D @ 0x00643CA1 (`__fastcall` out, def offset), the remaining sprite/text calls of the retail tail.
// Layout: CDrawQuestInfo QuestCount +0x3c, Elements list header +0x58 (STLport list: next +0, prev +4, element +8), Drawn +0x18.
// Shape: best-effort -- parked at once: the retail body rounds the panel position with the `fistp/fild` frame-slot helper (lane gap).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };
struct CDrawContext;
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; unsigned long size() const { return _M_finish - _M_start; } };
struct CQuestInfoElement {
    virtual float Draw(const C2DVector* position, CDrawContext* context);   // slot 0
    virtual void v01(); virtual void v02();
    virtual long GetType();                // slot 0xc
    unsigned char _pad[0x10 - 4];
    long RegistryHandle;                   // +0x10
    long RegistryKey;                      // +0x14
    unsigned char _pad18[0x1c - 0x18];
    long PanelType;                        // +0x1c (6: framed)
    unsigned char _pad20[0x34 - 0x20];
    CLongVector Children;                  // +0x34
};
struct SElementNode { SElementNode* next; SElementNode* prev; CQuestInfoElement* element; };
void __cdecl DeregisterQuestInfo(long key, long handle);   // 0x00440D70
struct CPlayerGuiDef {
    unsigned char _pad[0x30c];
    long MiniGameFont;                     // +0x30c
    unsigned char _pad310[0x56c - 0x310];
    C2DVector MiniGameOffset;              // +0x56c
    unsigned char _pad574[0x578 - 0x574];
    long MiniGameWidth;                    // +0x578
    float MiniGameMinHeight;               // +0x57c
    float MiniGameMinHeight2;              // +0x580
};
CPlayerGuiDef* __fastcall PeekGuiDef();   // 0x00438570
void __fastcall AnchorPoint2D(C2DVector* out, const C2DVector* in);   // 0x00643CA1
struct CGuiState { bool IsActive(); };    // 0x00437870 (byte +0x18 of the +0x160 object, descriptive)

struct CTextMetrics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual long GetLineHeight(long font);     // slot 0x60
};
struct CRect2D { float Left, Top, Right, Bottom; };
struct CDrawQuestInfo {
    unsigned char _pad0[4];
    CGuiState* State;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CTextMetrics* Metrics;                 // +0x14
    bool Drawn;                            // +0x18
    unsigned char _pad1[0x3c - 0x19];
    long QuestCount;                       // +0x3c
    unsigned char _pad2[0x58 - 0x40];
    SElementNode* Elements;                // +0x58
    CQuestInfoElement* GetElementByIndex(long index);   // 0x00644FC5
    void DrawBox(CDrawContext* context, const CRect2D* rect);   // 0x00643EB2
    void DrawMiniGameInfo(CDrawContext* context);
};

void CDrawQuestInfo::DrawMiniGameInfo(CDrawContext* context)
{
    CQuestInfoElement* miniGame = 0;
    if (QuestCount > 0) {
        for (SElementNode* it = Elements->next; it != Elements; it = it->next) {
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
            if (State->IsActive())
                position.Y = (float)Metrics->GetLineHeight(def->MiniGameFont) + position.Y + 16.0f;
            else
                position.Y = position.Y + 20.0f;
            float height = miniGame->Draw(&position, context);
            C2DVector child;
            child.X = position.X - 12.0f;
            child.Y = position.Y;
            for (unsigned long i = 0; i < miniGame->Children.size(); ++i) {
                child.Y += height;
                CQuestInfoElement* element = GetElementByIndex(miniGame->Children._M_start[i]);
                if (element)
                    height = element->Draw(&child, context);
            }
            if (miniGame->PanelType == 6) {
                child.Y += height;
                CRect2D rect;
                rect.Top = position.Y;
                rect.Right = (float)def->MiniGameWidth + position.X;
                rect.Bottom = child.Y - position.Y + position.Y;
                if (rect.Bottom - position.Y < def->MiniGameMinHeight2)
                    rect.Bottom = position.Y + def->MiniGameMinHeight2;
                rect.Left = position.X - 16.0f;
                rect.Right += 16.0f;
                DrawBox(context, &rect);
            }
        }
    }
}
