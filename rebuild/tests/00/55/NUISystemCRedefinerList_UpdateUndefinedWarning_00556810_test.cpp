#include <cstdio>
// Behaviour model of NUISystem::CRedefinerList::UpdateUndefinedWarning @ 0x00556810 (copy of the source's logic).
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
template <class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CComponent {
    long type; C2DVector pos;
    CComponent(long t, float x) : type(t), pos(x, x + 1.0f) {}
    virtual ~CComponent() {}
    virtual C2DVector GetPosition() const { return pos; }
    virtual long GetComponentType() { return type; }
};
struct CKeyRedefiner : CComponent {
    long SubType, Definition;
    CKeyRedefiner(long s, long d) : CComponent(0x27, 0.0f), SubType(s), Definition(d) {}
    bool IsUndefined() { return SubType == 2 && Definition == 0; }
};
struct Call { int kind; CComponent* c; float start, end, x; int alpha; };
static Call g_calls[8]; static int g_n = 0;
struct CScrollableList {
    CRetailVector<CCountedPointer<CComponent> > Children;
    void HideComponent(CComponent* c, float s, float e, C2DVector p, unsigned char a)
    { Call k = { 1, c, s, e, p.X, a }; g_calls[g_n++] = k; }
    void UnhideComponent(CComponent* c, float s, float e, C2DVector p)
    { Call k = { 2, c, s, e, p.X, -1 }; g_calls[g_n++] = k; }
};
struct CRedefinerList : CScrollableList {
    bool HaveUndefinedControls; CComponent* PUndefinedWarning; CComponent* PAcceptButton;
    void UpdateUndefinedWarning(float time)
    {
        HaveUndefinedControls = false;
        if (!PUndefinedWarning)
            return;
        for (unsigned int i = 0; i < Children.size(); ++i) {
            if (Children[i]->GetComponentType() == 0x27) {
                CKeyRedefiner* redefiner = static_cast<CKeyRedefiner*>(Children[i].Data);
                if (redefiner && redefiner->IsUndefined()) {
                    HaveUndefinedControls = true;
                    break;
                }
            }
        }
        if (HaveUndefinedControls) {
            UnhideComponent(PUndefinedWarning, 0.0f, time, PUndefinedWarning->GetPosition());
            if (PAcceptButton)
                HideComponent(PAcceptButton, 0.0f, time, PAcceptButton->GetPosition(), 0x60);
        }
        else {
            HideComponent(PUndefinedWarning, 0.0f, time, PUndefinedWarning->GetPosition(), 0);
            if (PAcceptButton)
                UnhideComponent(PAcceptButton, 0.0f, time, PAcceptButton->GetPosition());
        }
    }
};
int main()
{
    CComponent warn(1, 10.0f), accept(1, 20.0f), other(5, 0.0f);
    CKeyRedefiner defined(2, 7), undefined(2, 0);
    CCountedPointer<CComponent> kids[3];
    kids[0].Data = &other; kids[1].Data = &defined; kids[2].Data = &undefined;
    CRedefinerList l;
    l.Children._M_start = kids; l.Children._M_finish = kids + 3; l.Children._M_end_of_storage = kids + 3;
    l.PUndefinedWarning = &warn; l.PAcceptButton = &accept; l.HaveUndefinedControls = true;
    l.UpdateUndefinedWarning(0.5f);
    int ok = l.HaveUndefinedControls && g_n == 2
          && g_calls[0].kind == 2 && g_calls[0].c == &warn && g_calls[0].start == 0.0f && g_calls[0].end == 0.5f && g_calls[0].x == 10.0f
          && g_calls[1].kind == 1 && g_calls[1].c == &accept && g_calls[1].alpha == 0x60 && g_calls[1].x == 20.0f;
    g_n = 0; l.Children._M_finish = kids + 2;                  // only defined controls
    l.UpdateUndefinedWarning(0.25f);
    ok = ok && !l.HaveUndefinedControls && g_n == 2 && g_calls[0].kind == 1 && g_calls[0].c == &warn && g_calls[0].alpha == 0
            && g_calls[1].kind == 2 && g_calls[1].c == &accept && g_calls[1].end == 0.25f;
    g_n = 0; l.PUndefinedWarning = 0; l.HaveUndefinedControls = true;
    l.UpdateUndefinedWarning(1.0f);
    ok = ok && !l.HaveUndefinedControls && g_n == 0;
    if (!ok) { std::printf("00556810_TEST FAIL\n"); return 1; }
    std::printf("00556810_TEST PASS\n");
    return 0;
}