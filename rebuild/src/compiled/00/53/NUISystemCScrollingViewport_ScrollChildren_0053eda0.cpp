// NUISystem::CScrollingViewport::ScrollChildren(bool) -- scroll the viewport's children one step: take the step from the front end's description offset (or the viewport's own speed), negated when scrolling up; build the on-screen window box; ask every active child how far it can still scroll and keep the largest; clamp the step to it (unless the front end says to skip the clamp) and move every active child by it
// Leaf: ScrollChildren
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053EDA0, length 880 bytes, ret 4 (virtual, ?ScrollChildren@CScrollingViewport@NUISystem@@UAEX_N@Z).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (__fastcall); NUISystem::CManager GetDescriptionScrollingOffset 0x0041CF40,
// GetUIScale 0x0041CF47 (C2DVector by hidden pointer), the +0xc8 flag getter 0x0041CF2C; CComponent virtuals slot 25
// (+0x64) GetRenderPosition, slot 33 (+0x84) ChangePositionDelta, slot 88 (+0x160) CanScroll, slot 98 (+0x188) IsActive.
// Layout: Children vector<CCountedPointer<CComponent>> +0xb0 (8-byte elements), the second child list
// vector<CComponent*> +0x164 (descriptive), ScrollingSpeed +0x174, Window C2DBoxF +0x178, PScrollBar +0x188,
// PScrollBarOutside +0x18c. Float pool: -1.0f 0x0122DEE0, 0.2f immediate.
#include <math.h>
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
namespace NUISystem {
class CManager { public:
    float GetDescriptionScrollingOffset();   // 0x0041CF40
    C2DVector GetUIScale();                  // 0x0041CF47
    bool IsScrollUnclamped();                // 0x0041CF2C (descriptive: the +0xc8 flag)
    static CManager* __fastcall GetInstance();                 // 0x0041E5F2
};
}
namespace NUISystem {
class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual const C2DVector& GetRenderPosition();                                          // slot 25 (+0x64)
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear);     // slot 33 (+0x84)
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual float CanScroll(bool down, C2DBoxF& bounds);                                   // slot 88 (+0x160)
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual bool IsActive();                                                               // slot 98 (+0x188)
};
struct CChildPointer {               // CCountedPointer<CComponent> (8 bytes)
    CComponent* Data;
    long*       Count;
    CComponent* operator->() const { return Data; }
};
struct CChildVector {                // STLport vector<CCountedPointer<CComponent>> (12 bytes)
    CChildPointer* _M_start; CChildPointer* _M_finish; CChildPointer* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CChildPointer& operator[](unsigned int n) { return _M_start[n]; }
};
struct CComponentVector {            // STLport vector<CComponent*> (12 bytes)
    CComponent** _M_start; CComponent** _M_finish; CComponent** _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CComponent*& operator[](unsigned int n) { return _M_start[n]; }
};
class CScrollingViewport : public CComponent { public:
    unsigned char    _pad004[0xb0 - 0x04];
    CChildVector     Children;            // +0xb0
    unsigned char    _pad0bc[0x164 - 0xbc];
    CComponentVector ScrollChildList;     // +0x164 (descriptive)
    unsigned char    _pad170[0x174 - 0x170];
    float            ScrollingSpeed;      // +0x174
    C2DBoxF          Window;              // +0x178
    CComponent*      PScrollBar;          // +0x188
    CComponent*      PScrollBarOutside;   // +0x18c
    virtual void ScrollChildren(bool up);
};

void CScrollingViewport::ScrollChildren(bool up)
{
    float delta;
    if (NUISystem::CManager::GetInstance()->GetDescriptionScrollingOffset() != -1.0f) {
        delta = up ? -NUISystem::CManager::GetInstance()->GetDescriptionScrollingOffset() : NUISystem::CManager::GetInstance()->GetDescriptionScrollingOffset();
    } else if (up) {
        delta = -ScrollingSpeed;
    } else {
        delta = ScrollingSpeed;
    }
    C2DBoxF box;
    float x = Window.TLX;
    box.TLX = x * NUISystem::CManager::GetInstance()->GetUIScale().X + GetRenderPosition().X;
    float y = Window.TLY;
    y *= NUISystem::CManager::GetInstance()->GetUIScale().Y;
    box.TLY = y + GetRenderPosition().Y;
    C2DVector size;
    size.X = Window.BRX - Window.TLX;
    box.BRX = size.X * NUISystem::CManager::GetInstance()->GetUIScale().X + box.TLX;
    size.Y = Window.BRY - Window.TLY;
    box.BRY = size.Y * NUISystem::CManager::GetInstance()->GetUIScale().Y + box.TLY;
    float scroll = 0.0f;
    unsigned int i;
    for (i = 0; i < Children.size(); ++i) {
        if (Children[i]->IsActive() && Children[i].Data != PScrollBar
            && Children[i].Data != PScrollBarOutside) {
            Children[i]->GetRenderPosition();
            float can = Children[i]->CanScroll(up, box);
            if (fabs(scroll) < fabs(can))
                scroll = can;
        }
    }
    for (i = 0; i < ScrollChildList.size(); ++i) {
        if (ScrollChildList[i]->IsActive()) {
            ScrollChildList[i]->GetRenderPosition();
            float can = ScrollChildList[i]->CanScroll(up, box);
            if (fabs(scroll) < fabs(can))
                scroll = can;
        }
    }
    if (!NUISystem::CManager::GetInstance()->IsScrollUnclamped()) {
        if (fabs(scroll) > fabs(delta))
            scroll = delta;
    }
    for (i = 0; i < Children.size(); ++i) {
        if (Children[i]->IsActive() && Children[i].Data != PScrollBar
            && Children[i].Data != PScrollBarOutside) {
            C2DVector move(0.0f, scroll);
            Children[i]->ChangePositionDelta(move, 0.2f, false);
        }
    }
    for (i = 0; i < ScrollChildList.size(); ++i) {
        if (ScrollChildList[i]->IsActive()) {
            C2DVector move(0.0f, scroll);
            ScrollChildList[i]->ChangePositionDelta(move, 0.2f, false);
        }
    }
}

}