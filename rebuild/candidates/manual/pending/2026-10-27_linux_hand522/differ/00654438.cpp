// NPlayerGui::CDrawQuestInfo::Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw) -- unless the GUI is in mode 1 and the draw is not forced: stack the quest-info elements down from the top-right quest-info anchor (below the clock ring while the mini-map draws), each advancing by its drawn height rounded up and cleared when it draws nothing or is the mini-game bar, frame them in a box at least the minimum height when any drew, then draw the oracle symbols
// Grade: DIFFER(347v347) -- frame-slot assignment inside the inlined ceil only: retail homes the by-value copy at [ebp-0Ch], the 0.5 local at [ebp-8] and the 1.0 local in the dead `forceDraw` slot [ebp+0Ch]; ours puts the copy in [ebp+0Ch], 0.5 at [ebp-0Ch], 1.0 at [ebp-8] (same instructions, permuted offsets); and the box's right edge is `fld [pos.X] ; fadd st(2)` where retail duplicates the width (`fld st(1) ; fadd [pos.X]`). Parked as a retail_asm candidate (the bare dword fistp at 0x006544F2). Tried: minimal asm (fld/fadd/fistp; VC folded the 1.0 into a constant, 345), the full asm through `fstp check` (this, 347), 1.0 declared before 0.5 plus scalar width/height (343, worse).
// Leaf: ?Draw@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z
// Retail-asm: 0x006544EC `fld dword ptr [ebp-0Ch]` / 0x006544EF `fadd dword ptr [ebp-8]` + 0x006544F2 `fistp dword ptr [ebp-4]` / 0x006544F5 `fild` / 0x006544F8 `fsub dword ptr [ebp+0Ch]` / 0x006544FB `fstp` -- the engine's inline-assembly ceil(float) (Ego_r.pdb ?ceil@@YAMM@Z): its by-value copy plus a 0.5 local rounded by a bare dword fistp into the address-taken result, less a homed 1.0 local (kept in memory, so it is an asm operand), then the C integer compare of the two homed floats and `--result`. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`: no C/C++ form gives the bare dword fistp (the landed ROUND helpers 0x006472B2 / 0x00643B9F of this GUI TU are the same idiom).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00654438, 347 bytes, thiscall, ebp frame (`leave`), `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Draw@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z (the only
//   two-argument Draw of the class); the body walks CDrawQuestInfo::Elements (+0x58, list<CCountedPointer<CQuestInfoElement> >),
//   tests GuiMode (+0x28) and ends in DrawOracleSymbols (Ego_r.pdb members 4 below the donor; STLport list 4 bytes).
// Callees: CPlayerGui::PeekGuiDef 0x00438570, NPlayerGui::GFAnchorToTopRight(const C2DVector&) 0x00643CA1 (CPlayerGuiDef::
//   QuestInfoPos +0x56C), CPlayerGui::IsMiniMapDrawing() const 0x00437870, the bank's virtual GetGraphicMaxHeight(unsigned long)
//   const (slot 24, +0x60; CPlayerGuiDef::ClockRingGraphic +0x30C), the element's virtual DrawAt (slot 0), Clear (slot 2) and
//   GetType (slot 3; type 6 is never drawn), CDrawQuestInfo::DrawBox(const CEnginePrimitivePrepareInfo&, const C2DBoxF&)
//   0x00643EB2 (CPlayerGuiDef::QuestInfoGraphicWidth +0x578 / QuestInfoGraphicMinHeight +0x580), DrawOracleSymbols 0x0064F6DC.
// Names: 0x00437870 reads PDrawMiniMap->Drawing (CPlayerGui +0x160 / CDrawBase +0x18) -> ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ;
//   0x00643EB2 is the DrawBox signature (prepare info, box); 0x0064F6DC tests OracleSymbols (+0x40) -> DrawOracleSymbols; bank
//   slot 24 is the max-over-frames of the frame height word (+8; slot 23 takes +6) -> GetGraphicMaxHeight. All probable.
// Label correction: catalogued as "CDrawQuestInfo::Draw" without the NPlayerGui namespace.
// Layout: local view (no engine/ header for CDrawQuestInfo, CDrawBase, CPlayerGui or CPlayerGuiDef); CPlayerGuiDef retail
//   offsets are the Ego_r.pdb ones less the debug-STL growth (vectors 16 -> 12 bytes, base 0x40 -> 0x3C).
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
};
class C2DBoxF {
public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
};
__forceinline float ceil(float value)
{
    long result;
    float half = 0.5f;
    float one = 1.0f;
    float check;
    __asm fld value
    __asm fadd half
    __asm fistp result
    __asm fild result
    __asm fsub one
    __asm fstp check
    if (*(long*)&value == *(long*)&check)
        --result;
    return (float)result;
}
class CEnginePrimitivePrepareInfo;
class CPlayerGuiDef {
public:
    unsigned char _pad_0x000[0x30c];
    long          ClockRingGraphic;                         // +0x30C
    unsigned char _pad_0x310[0x56c - 0x310];
    C2DVector     QuestInfoPos;                             // +0x56C
    float         QuestInfoBarOffsetX;                      // +0x574
    long          QuestInfoGraphicWidth;                    // +0x578
    long          QuestInfoGraphicHeight;                   // +0x57C
    float         QuestInfoGraphicMinHeight;                // +0x580
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
    bool IsMiniMapDrawing() const;                          // 0x00437870
};
class CGraphicDataBank {
public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23();
    virtual unsigned long GetGraphicMaxHeight(unsigned long graphic) const;   // slot 24 (+0x60)
};
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CEnginePrimitiveHandle;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    T* operator->() const { return Data; }
};
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CWideString { public: void* PStringData; };
enum EPlayerGuiMode { };
namespace NPlayerGui {
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& position);   // 0x00643CA1
class CDrawBase {
public:
    virtual ~CDrawBase();
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;   // +0x1C
};
class CDrawQuestInfo : public CDrawBase {
public:
    enum EElementType { };
    class CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector& position, const CEnginePrimitivePrepareInfo& prepareInfo);   // slot 0
        virtual bool Update();                              // slot 1
        virtual void Clear();                               // slot 2
        virtual EElementType GetType() const;               // slot 3
    };
    void Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw);
    void DrawBox(const CEnginePrimitivePrepareInfo& prepareInfo, const C2DBoxF& box);   // 0x00643EB2
    void DrawOracleSymbols(const CEnginePrimitivePrepareInfo& prepareInfo);            // 0x0064F6DC
    EPlayerGuiMode                GuiMode;                  // +0x28
    float                         FadeInTimer;              // +0x2C
    float                         MiniModeTimer;            // +0x30
    bool                          MiniMode;                 // +0x34
    long                          FramesUntilMiniMode;      // +0x38
    long                          MiniGameElementIndex;     // +0x3C
    std::list<long>               OracleSymbols;            // +0x40
    std::vector<CEnginePrimitiveHandle> OraclePrimitiveHandles;   // +0x44
    CWideString                   Name;                     // +0x50
    CWideString                   Text;                     // +0x54
    std::list<CCountedPointer<CQuestInfoElement> > Elements;   // +0x58
};

void CDrawQuestInfo::Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw)
{
    if (GuiMode == (EPlayerGuiMode)1 && !forceDraw)
        return;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    C2DVector pos = GFAnchorToTopRight(def.QuestInfoPos);
    if (PlayerGui->IsMiniMapDrawing())
        pos.Y = pos.Y + (float)(long)Bank->GetGraphicMaxHeight(def.ClockRingGraphic) + 16.0f;
    C2DVector drawPos = pos;
    bool drewSomething = false;
    for (std::list<CCountedPointer<CQuestInfoElement> >::iterator it = Elements.begin(); it != Elements.end(); ++it) {
        if (it->Data) {
            if ((*it)->GetType() != (EElementType)6) {
                float height = (*it)->DrawAt(drawPos, prepareInfo);
                if (height > 0.0001f) {
                    drawPos.Y += ceil(height);
                    drewSomething = true;
                }
                else
                    (*it)->Clear();
            }
            else
                (*it)->Clear();
        }
    }
    if (drewSomething) {
        C2DVector size;
        size.X = (float)def.QuestInfoGraphicWidth;
        size.Y = drawPos.Y - pos.Y;
        C2DBoxF box;
        box.TLX = pos.X;
        box.TLY = pos.Y;
        box.BRX = size.X + pos.X;
        box.BRY = size.Y + pos.Y;
        if (box.BRY - pos.Y < def.QuestInfoGraphicMinHeight)
            box.BRY = pos.Y + def.QuestInfoGraphicMinHeight;
        DrawBox(prepareInfo, box);
    }
    DrawOracleSymbols(prepareInfo);
}
}   // namespace NPlayerGui
