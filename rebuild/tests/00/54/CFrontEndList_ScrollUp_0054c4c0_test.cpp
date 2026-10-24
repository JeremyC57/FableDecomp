// Behaviour test for 0x0054C4C0 NUISystem::CFrontEndList::ScrollUp: denied sound for <= 1 row or at the top of a
// scrolling list; otherwise scroll sound, selection moves up (wrapping), colours rotate down one row, scrolling lists
// move every row by (0, 30) and refade rows by distance from the selection.
#include <stdio.h>
#include <stdlib.h>
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_defDestroyed = 0;
struct CDefVirtual { virtual void Destroy() { ++g_defDestroyed; } virtual ~CDefVirtual() { } };
struct CDefBase : CDefVirtual, CRefCounted { void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); } };
struct CSoundHandle { long m_id; };
struct CUIMiscThingsDef : CDefBase { CSoundHandle ListScrollSound; CSoundHandle ListDeniedSound; };
struct CMiscThingsDefPointer {
    CUIMiscThingsDef* m_p;
    CMiscThingsDefPointer(CUIMiscThingsDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CMiscThingsDefPointer(const CMiscThingsDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CMiscThingsDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CUIMiscThingsDef* operator->() const { return m_p; }
};
namespace NUISystem {
struct C2DVector { float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CComponent {
public:
    long State; CRGBColour Colour; C2DVector Pos; float LastTime;
    CComponent() : State(0), LastTime(-5.0f) { Pos.X = Pos.Y = 0.0f; }
    virtual void MoveBy(const C2DVector& offset, float time, bool) { Pos.X += offset.X; Pos.Y += offset.Y; LastTime = time; }
    virtual void SetColour(const CRGBColour& colour, float time, bool) { Colour = colour; LastTime = time; }
    virtual void SetState(long state) { State = state; }
    virtual C2DVector GetPosition() { return Pos; }
    virtual CRGBColour GetColour() { return Colour; }
};
static long g_lastSound = 0; static int g_sounds = 0;
static CUIMiscThingsDef g_def;
class CFrontEndManager {
public:
    static CFrontEndManager* GetInstance();
    virtual void PlaySound(const CSoundHandle& sound, bool) { g_lastSound = sound.m_id; ++g_sounds; }
    virtual CMiscThingsDefPointer GetMiscThingsDef() { return CMiscThingsDefPointer(&g_def); }
};
static CFrontEndManager g_fem;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_fem; }
struct CComponentVector {
    CComponent** _M_start; CComponent** _M_finish; CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CComponent* operator[](unsigned long n) const { return _M_start[n]; }
};
class CFrontEndList {
public:
    float ScrollTime; bool Scrolling;
    virtual float GetScrollTime() { return ScrollTime; }
    virtual bool IsScrollingList() { return Scrolling; }
    void ScrollUp();
    long             SelectedChild;
    CComponentVector ScrollingChildren;
    unsigned char    PDefAlphaOffset;
};
void CFrontEndList::ScrollUp()
{
    if (ScrollingChildren.size() <= 1)
    {
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->ListDeniedSound, true);
        return;
    }
    if (SelectedChild == 0 && IsScrollingList())
    {
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->ListDeniedSound, true);
        return;
    }
    CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->ListScrollSound, true);
    ScrollingChildren[SelectedChild]->SetState(4);
    long selected = SelectedChild - 1;
    SelectedChild = selected;
    if (selected < 0)
        selected = ScrollingChildren.size() - 1;
    SelectedChild = selected;
    ScrollingChildren[selected]->SetState(3);
    float time = GetScrollTime() != -1.0f ? GetScrollTime() : 0.0f;
    CRGBColour lastColour = ScrollingChildren[ScrollingChildren.size() - 1]->GetColour();
    C2DVector lastPosition = ScrollingChildren[ScrollingChildren.size() - 1]->GetPosition();
    C2DVector offset(0.0f, 30.0f);
    for (unsigned long i = ScrollingChildren.size() - 1; i != 0; --i) {
        ScrollingChildren[i]->SetColour(ScrollingChildren[i - 1]->GetColour(), time, false);
        if (IsScrollingList())
            ScrollingChildren[i]->MoveBy(offset, time, false);
    }
    ScrollingChildren[0]->SetColour(lastColour, time, false);
    if (IsScrollingList())
        ScrollingChildren[0]->MoveBy(offset, time, false);
    if (IsScrollingList()) {
        CRGBColour colour(255, 255, 255, 255);
        for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
            float alpha = 1.0f - (float)abs(SelectedChild - (long)i) * ((float)PDefAlphaOffset * (1.0f / 255.0f));
            colour.A = (unsigned char)(long)(alpha < 0.0f ? 0.0f : alpha * 255.0f);
            ScrollingChildren[i]->SetColour(colour, time, false);
        }
    }
}
}
using namespace NUISystem;
static CRGBColour Col(unsigned char v) { return CRGBColour(v, v, v, v); }
static bool Near(int a, int b) { return a - b <= 1 && b - a <= 1; }   // float->long truncation of 255 * alpha
int main()
{
    g_def.m_count = 1; g_def.ListScrollSound.m_id = 7; g_def.ListDeniedSound.m_id = 9;
    CComponent rows[4]; CComponent* table[4] = { &rows[0], &rows[1], &rows[2], &rows[3] };
    for (int k = 0; k < 4; ++k) rows[k].Colour = Col((unsigned char)(10 + k));
    CFrontEndList list; list.ScrollTime = -1.0f; list.Scrolling = false; list.PDefAlphaOffset = 51;
    list.ScrollingChildren._M_start = table; list.ScrollingChildren._M_finish = table + 1; list.ScrollingChildren._M_end_of_storage = table + 4;
    list.SelectedChild = 0;
    list.ScrollUp();                                         // one row: denied
    bool ok = g_lastSound == 9 && g_sounds == 1 && list.SelectedChild == 0 && g_def.m_count == 1;
    list.ScrollingChildren._M_finish = table + 4;
    list.ScrollUp();                                         // non-scrolling list at the top: wraps to row 3
    ok = ok && g_lastSound == 7 && list.SelectedChild == 3 && rows[0].State == 4 && rows[3].State == 3 && g_def.m_count == 1;
    ok = ok && rows[0].Colour.R == 13 && rows[1].Colour.R == 10 && rows[2].Colour.R == 11 && rows[3].Colour.R == 12;
    ok = ok && rows[0].LastTime == 0.0f && rows[1].Pos.Y == 0.0f;
    list.Scrolling = true; list.ScrollTime = 0.25f; list.SelectedChild = 0;
    list.ScrollUp();                                         // scrolling list at the top: denied
    ok = ok && g_lastSound == 9 && list.SelectedChild == 0;
    list.SelectedChild = 2;
    list.ScrollUp();                                         // scrolling: selection 1, rows move, refade by distance
    ok = ok && g_lastSound == 7 && list.SelectedChild == 1 && rows[2].State == 4 && rows[1].State == 3;
    for (int k = 0; ok && k < 4; ++k) ok = rows[k].Pos.Y == 30.0f && rows[k].LastTime == 0.25f && rows[k].Colour.R == 255;
    // alpha = 255 * (1 - |1 - k| * 51/255): k=0 -> 204, 1 -> 255, 2 -> 204, 3 -> 153
    ok = ok && Near(rows[0].Colour.A, 204) && rows[1].Colour.A == 255 && Near(rows[2].Colour.A, 204) && Near(rows[3].Colour.A, 153);
    list.PDefAlphaOffset = 200; list.SelectedChild = 3;
    list.ScrollUp();                                         // selection 2: far rows clamp to 0
    ok = ok && rows[2].Colour.A == 255 && Near(rows[1].Colour.A, 55) && rows[0].Colour.A == 0 && Near(rows[3].Colour.A, 55);
    ok = ok && g_def.m_count == 1 && g_defDestroyed == 0;
    if (!ok) { printf("0054c4c0_TEST FAIL\n"); return 1; }
    printf("0054c4c0_TEST PASS\n");
    return 0;
}
