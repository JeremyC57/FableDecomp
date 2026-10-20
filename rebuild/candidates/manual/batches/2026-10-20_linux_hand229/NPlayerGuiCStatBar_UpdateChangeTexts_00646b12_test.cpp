// Behaviour test for NPlayerGui::CStatBar::UpdateChangeTexts (0x00646B12): in states 0/3 each change text moves
// along the box by the frame step and fades; finished or faded texts are erased; other states do nothing.
#include <cstdio>
#include <cmath>

static const float g_epsilon = 1e-4f;
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct CChangeText { C2DVector OldOffset, Offset; CRGBColour Colour; bool StartToEnd; float PercentageThrough; };
struct SNode { SNode* next; SNode* prev; CChangeText data; };
struct iterator {
    SNode* n;
    CChangeText* operator->() const { return &n->data; }
    bool operator!=(const iterator& o) const { return n != o.n; }
    iterator& operator++() { n = n->next; return *this; }
};
struct CList {
    SNode head; long count;
    CList() : count(0) { head.next = head.prev = &head; }
    void push_back(SNode* x) { x->next = &head; x->prev = head.prev; head.prev->next = x; head.prev = x; ++count; }
    iterator begin() { iterator it; it.n = head.next; return it; }
    iterator end() { iterator it; it.n = &head; return it; }
    iterator erase(iterator p) { SNode* nx = p.n->next; p.n->prev->next = nx; nx->prev = p.n->prev; --count; iterator it; it.n = nx; return it; }
};
struct COwner { C2DVector BoxTL, BoxBR; float FrameStep; };
struct CStatBar {
    COwner* Owner; long State; CList ChangeTexts;
    void UpdateChangeTexts();
};

void CStatBar::UpdateChangeTexts()
{
    if (State != 0 && State != 3)
        return;
    iterator it = ChangeTexts.begin();
    while (it != ChangeTexts.end()) {
        C2DVector start, end;
        if (it->StartToEnd) {
            start = Owner->BoxTL;
            end = Owner->BoxBR;
        } else {
            start = Owner->BoxBR;
            end = Owner->BoxTL;
        }
        C2DVector delta;
        delta = end - start;
        if (!((it->PercentageThrough += Owner->FrameStep) >= 1.0f - g_epsilon) && it->Colour.A) {
            it->OldOffset = it->Offset;
            it->Offset = start + delta * it->PercentageThrough;
            it->Colour.A = (unsigned char)((1.0f - it->PercentageThrough) * it->Colour.A);
            ++it;
        } else
            it = ChangeTexts.erase(it);
    }
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    int ok = 1;
    COwner owner; owner.BoxTL = C2DVector(0, 0); owner.BoxBR = C2DVector(100, 50); owner.FrameStep = 0.25f;
    SNode a, b, c;
    a.data.StartToEnd = true;  a.data.PercentageThrough = 0.0f; a.data.Colour.A = 200; a.data.Offset = C2DVector(1, 2);
    b.data.StartToEnd = false; b.data.PercentageThrough = 0.5f; b.data.Colour.A = 100; b.data.Offset = C2DVector(3, 4);
    c.data.StartToEnd = true;  c.data.PercentageThrough = 0.8f; c.data.Colour.A = 50;  c.data.Offset = C2DVector(5, 6);
    CStatBar bar; bar.Owner = &owner; bar.State = 5;
    bar.ChangeTexts.push_back(&a); bar.ChangeTexts.push_back(&b); bar.ChangeTexts.push_back(&c);
    bar.UpdateChangeTexts();
    ok &= bar.ChangeTexts.count == 3 && a.data.PercentageThrough == 0.0f;
    bar.State = 3;
    bar.UpdateChangeTexts();
    ok &= bar.ChangeTexts.count == 2;                                   // c reached 1.05 -> erased
    ok &= near(a.data.Offset.X, 25) && near(a.data.Offset.Y, 12.5f) && near(a.data.OldOffset.X, 1) && a.data.Colour.A == 150;
    ok &= near(b.data.Offset.X, 25) && near(b.data.Offset.Y, 12.5f) && b.data.Colour.A == 25;   // end to start, t = .75
    ok &= bar.ChangeTexts.head.next == &a && a.next == &b && b.next == &bar.ChangeTexts.head;
    std::printf(ok ? "00646b12_TEST PASS\n" : "00646b12_TEST FAIL\n");
    return ok ? 0 : 1;
}
