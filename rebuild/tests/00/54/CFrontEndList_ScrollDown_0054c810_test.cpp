// Behaviour test for NUISystem::CFrontEndList::ScrollDown (0x0054C810): the reconstruction (src/0054c810.cpp, verbatim)
// driven through recording row/list doubles -- error sound for a 1-row list or the last row of a scrolling list, otherwise
// the up/down sound, selection advance with wrap, colour rotation, (0,-30) slides and the distance fade with clamp.
inline void* operator new(unsigned int, void* p) { return p; }
// NUISystem::CFrontEndList::ScrollDown() -- move the selection one row down (wrapping to the top) and rotate the rows'
//   colours up one place; scrolling lists also slide every row by (0, -30) and fade rows by distance from the selection.
// Leaf: ?ScrollDown@CFrontEndList@NUISystem
// Retail: Fable.exe @ 0x0054C810, 977 bytes, /O2 FPO frame (sub esp,20h), plain `ret`.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), its slots 40 (+0xa0 PlaySound by def member) and 42
//   (+0xa8 the CUIMiscThingsDef by value: SoundUpDown +0x194, SoundError +0x1a4 = Ego_r.pdb 412/428 minus 8); own slots
//   54 (+0xd8 GetUpdateTime) and 171 (+0x2ac IsScrolling); row slots 32 (+0x80 SetPosition), 38 (+0x98 SetColour),
//   48 (+0xc0 SetCurrentState), 55 (+0xdc GetPosition), 56 (+0xe0 GetColour), by-value results through a hidden pointer.
// Layout (retail = PDB - 0x30): SelectedChild +0x15c, the row vector +0x164, the alpha step byte +0x1f8.
#define SLOTS4(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3();
#define SLOTS16(p) SLOTS4(p##0) SLOTS4(p##1) SLOTS4(p##2) SLOTS4(p##3)

extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)
class CCharString { public: void* PData; };
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CRefCounted {
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    CRefCounted Ref;
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
struct CUIMiscThingsDef : CDefBase {
    unsigned char _pad08[0x194 - 8];
    CCharString   SoundUpDown;                   // +0x194
    unsigned char _pad198[0x1a4 - 0x198];
    CCharString   SoundError;                    // +0x1a4
};
template <class T> class CDefPointer { public:
    T* Data;
    ~CDefPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};

namespace NUISystem {

class CFrontEndManager { public:
    static CFrontEndManager* __fastcall GetInstance();
    SLOTS16(m0) SLOTS16(m1) SLOTS4(m20) SLOTS4(m21)
    virtual void PlaySound(const CCharString& sound, bool a);   // slot 40 (+0xa0)
    virtual void m41();
    virtual CDefPointer<CUIMiscThingsDef> GetMiscThingsDef();   // slot 42 (+0xa8)
};

class CComponent { public:
    SLOTS16(c0) SLOTS16(c1)
    virtual void SetPosition(const C2DVector& position, float time, bool linear);   // slot 32 (+0x80)
    virtual void c33(); virtual void c34(); virtual void c35(); virtual void c36(); virtual void c37();
    virtual void SetColour(const CRGBColour& colour, float time, bool linear);      // slot 38 (+0x98)
    virtual void c39(); SLOTS4(c4a) SLOTS4(c4b)
    virtual void SetCurrentState(long state);                                      // slot 48 (+0xc0)
    virtual void c49(); virtual void c50(); virtual void c51(); virtual void c52(); virtual void c53(); virtual void c54();
    virtual C2DVector GetPosition();                                               // slot 55 (+0xdc)
    virtual CRGBColour GetColour();                                                // slot 56 (+0xe0)
};

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};

class CFrontEndList { public:
    SLOTS16(l0) SLOTS16(l1) SLOTS16(l2)
    virtual void l30(); virtual void l31(); virtual void l32(); virtual void l33(); virtual void l34(); virtual void l35();
    virtual float GetUpdateTime();                                                 // slot 54 (+0xd8)
    virtual void l37(); virtual void l38(); virtual void l39(); SLOTS4(l3a) virtual void l3e(); virtual void l3f();
    SLOTS16(l4) SLOTS16(l5) SLOTS16(l6) SLOTS16(l7) SLOTS16(l8) SLOTS16(l9)
    SLOTS4(la0) SLOTS4(la1) virtual void la8(); virtual void la9(); virtual void laa();
    virtual bool IsScrolling();                                                    // slot 171 (+0x2ac)
    virtual void ScrollDown();

    unsigned char _pad004[0x15c - 4];
    unsigned long SelectedChild;                 // +0x15c
    unsigned char _pad160[4];
    CRetailVector<CComponent*> Children;         // +0x164
    unsigned char _pad170[0x1f8 - 0x170];
    unsigned char AlphaStep;                     // +0x1f8
};

void CFrontEndList::ScrollDown()
{
    if (Children.size() <= 1)
    {
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->SoundError, true);
        return;
    }
    if (SelectedChild == Children.size() - 1 && IsScrolling())
    {
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->SoundError, true);
        return;
    }
    CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetMiscThingsDef()->SoundUpDown, true);
    Children[SelectedChild]->SetCurrentState(4);
    ++SelectedChild;
    SelectedChild = SelectedChild < Children.size() ? SelectedChild : 0;
    Children[SelectedChild]->SetCurrentState(3);
    float updateTime = GetUpdateTime() != -1.0f ? GetUpdateTime() : 0.0f;
    CRGBColour firstColour = Children[0]->GetColour();
    C2DVector firstPosition = Children[0]->GetPosition();
    C2DVector offset(0.0f, -30.0f);
    for (unsigned int i = 0; i < Children.size() - 1; ++i)
    {
        Children[i]->SetColour(Children[i + 1]->GetColour(), updateTime, false);
        if (IsScrolling())
            Children[i]->SetPosition(offset, updateTime, false);
    }
    Children[Children.size() - 1]->SetColour(firstColour, updateTime, false);
    if (IsScrolling())
        Children[Children.size() - 1]->SetPosition(offset, updateTime, false);
    if (IsScrolling())
    {
        CRGBColour colour(255, 255, 255, 255);
        for (unsigned int i = 0; i < Children.size(); ++i)
        {
            float alpha = 1.0f - abs((int)(SelectedChild - i)) * (AlphaStep * (1.0f / 255.0f));
            colour.A = (unsigned char)(alpha < 0.0f ? 0.0f : alpha * 255.0f);
            Children[i]->SetColour(colour, updateTime, false);
        }
    }
}

}

// ---- behaviour harness: the reconstruction above, run against recording doubles ----
#include <stdio.h>
#include <string.h>
#undef SLOTS4
#undef SLOTS16
#define DEF4(C, p) void C::p##0() {} void C::p##1() {} void C::p##2() {} void C::p##3() {}
#define DEF16(C, p) DEF4(C, p##0) DEF4(C, p##1) DEF4(C, p##2) DEF4(C, p##3)

void CDefBase::v00() {}
static int g_defDestroys = 0;
void CDefBase::Destroy() { ++g_defDestroys; }

static CUIMiscThingsDef g_def;
static char g_sounds[64];

namespace NUISystem {
DEF16(CFrontEndManager, m0) DEF16(CFrontEndManager, m1) DEF4(CFrontEndManager, m20) DEF4(CFrontEndManager, m21)
void CFrontEndManager::m41() {}
void CFrontEndManager::PlaySound(const CCharString& sound, bool a)
{
    strcat(g_sounds, &sound == &g_def.SoundUpDown ? "U" : &sound == &g_def.SoundError ? "E" : "?");
    if (!a) strcat(g_sounds, "!");
}
CDefPointer<CUIMiscThingsDef> CFrontEndManager::GetMiscThingsDef()
{
    CDefPointer<CUIMiscThingsDef> p;
    p.Data = &g_def;
    ++g_def.Ref.Count;
    return p;
}
static CFrontEndManager g_manager;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_manager; }

DEF16(CComponent, c0) DEF16(CComponent, c1) DEF4(CComponent, c4a) DEF4(CComponent, c4b)
void CComponent::c33() {} void CComponent::c34() {} void CComponent::c35() {} void CComponent::c36() {} void CComponent::c37() {}
void CComponent::c39() {} void CComponent::c49() {} void CComponent::c50() {} void CComponent::c51() {} void CComponent::c52() {}
void CComponent::c53() {} void CComponent::c54() {}
void CComponent::SetPosition(const C2DVector&, float, bool) {}
void CComponent::SetColour(const CRGBColour&, float, bool) {}
void CComponent::SetCurrentState(long) {}
C2DVector CComponent::GetPosition() { return C2DVector(0, 0); }
CRGBColour CComponent::GetColour() { return CRGBColour(0, 0, 0, 0); }

DEF16(CFrontEndList, l0) DEF16(CFrontEndList, l1) DEF16(CFrontEndList, l2)
void CFrontEndList::l30() {} void CFrontEndList::l31() {} void CFrontEndList::l32() {} void CFrontEndList::l33() {}
void CFrontEndList::l34() {} void CFrontEndList::l35() {} void CFrontEndList::l37() {} void CFrontEndList::l38() {}
void CFrontEndList::l39() {} void CFrontEndList::l3e() {} void CFrontEndList::l3f() {}
DEF4(CFrontEndList, l3a)
DEF16(CFrontEndList, l4) DEF16(CFrontEndList, l5) DEF16(CFrontEndList, l6) DEF16(CFrontEndList, l7) DEF16(CFrontEndList, l8)
DEF16(CFrontEndList, l9) DEF4(CFrontEndList, la0) DEF4(CFrontEndList, la1)
void CFrontEndList::la8() {} void CFrontEndList::la9() {} void CFrontEndList::laa() {}
float CFrontEndList::GetUpdateTime() { return 0; }
bool CFrontEndList::IsScrolling() { return false; }
}

using namespace NUISystem;

struct TestRow : CComponent
{
    CRGBColour Colour;
    long State;
    int Moves;
    float LastTime;
    float MoveY;
    virtual void SetPosition(const C2DVector& position, float time, bool linear)
    {
        if (position.X != 0.0f || linear) MoveY = 999.0f; else MoveY = position.Y;
        ++Moves; LastTime = time;
    }
    virtual void SetColour(const CRGBColour& colour, float time, bool linear) { Colour = colour; LastTime = linear ? -99.0f : time; }
    virtual void SetCurrentState(long state) { State = state; }
    virtual C2DVector GetPosition() { return C2DVector(1, 2); }
    virtual CRGBColour GetColour() { return Colour; }
};

struct TestList : CFrontEndList
{
    bool Scrolling;
    float Time;
    virtual float GetUpdateTime() { return Time; }
    virtual bool IsScrolling() { return Scrolling; }
};

static TestRow g_rows[4];
static CComponent* g_ptrs[4];

static void Setup(TestList& list, int n, unsigned long selected, bool scrolling, float time, unsigned char step)
{
    memset(g_rows, 0, sizeof(g_rows));
    for (int i = 0; i < 4; ++i)
    {
        new (&g_rows[i]) TestRow();
        g_rows[i].Colour = CRGBColour((unsigned char)(10 * (i + 1)), 0, 0, (unsigned char)i);
        g_rows[i].State = -1; g_rows[i].Moves = 0; g_rows[i].LastTime = -1.0f; g_rows[i].MoveY = 0.0f;
        g_ptrs[i] = &g_rows[i];
    }
    list.Children._M_start = g_ptrs;
    list.Children._M_finish = g_ptrs + n;
    list.Children._M_end_of_storage = g_ptrs + n;
    list.SelectedChild = selected;
    list.Scrolling = scrolling;
    list.Time = time;
    list.AlphaStep = step;
    g_sounds[0] = 0;
    g_defDestroys = 0;
    g_def.Ref.Count = 0;
}

static int Fail(const char* what) { printf("failed: %s (sounds %s)\n", what, g_sounds); return 0; }

int main()
{
    int ok = 1;
    TestList list;
    // One row: error sound only, nothing moves.
    Setup(list, 1, 0, true, 0.5f, 0);
    list.ScrollDown();
    if (strcmp(g_sounds, "E") || g_defDestroys != 1 || g_rows[0].State != -1) ok = Fail("single row");
    // Last row selected on a scrolling list: error sound, no change.
    Setup(list, 3, 2, true, 0.5f, 0);
    list.ScrollDown();
    if (strcmp(g_sounds, "E") || list.SelectedChild != 2 || g_rows[2].State != -1 || g_defDestroys != 1) ok = Fail("scroll end");
    // Non-scrolling list, last selected: wraps to row 0; colours rotate up; no movement; update time -1 -> 0.
    Setup(list, 3, 2, false, -1.0f, 0);
    list.ScrollDown();
    if (strcmp(g_sounds, "U") || g_defDestroys != 1) ok = Fail("move sound");
    if (list.SelectedChild != 0 || g_rows[2].State != 4 || g_rows[0].State != 3) ok = Fail("wrap selection");
    if (g_rows[0].Colour.R != 20 || g_rows[1].Colour.R != 30 || g_rows[2].Colour.R != 10) ok = Fail("colour rotate");
    if (g_rows[0].Moves || g_rows[2].Moves || g_rows[0].LastTime != 0.0f) ok = Fail("non-scrolling time/moves");
    // Scrolling list, select 0 -> 1: every row slides by (0,-30) with the list time, alpha fades by distance.
    Setup(list, 3, 0, true, 0.25f, 128);
    list.ScrollDown();
    if (list.SelectedChild != 1 || g_rows[0].State != 4 || g_rows[1].State != 3) ok = Fail("advance selection");
    for (int i = 0; i < 3; ++i)
        if (g_rows[i].Moves != 1 || g_rows[i].MoveY != -30.0f || g_rows[i].LastTime != 0.25f) ok = Fail("slide");
    // alpha = 1 - |1 - i| * 128/255: rows 0 and 2 -> 126 (float 126.99998 truncated by __ftol2), row 1 -> 255; RGB forced to 255.
    if (g_rows[1].Colour.A != 255 || g_rows[0].Colour.A != 126 || g_rows[2].Colour.A != 126) { printf("alphas %d %d %d\n", g_rows[0].Colour.A, g_rows[1].Colour.A, g_rows[2].Colour.A); ok = Fail("fade"); }
    if (g_rows[0].Colour.R != 255 || g_rows[2].Colour.G != 255 || g_rows[1].Colour.B != 255) ok = Fail("white rows");
    // A large step clamps the faded rows to 0.
    Setup(list, 4, 0, true, 0.25f, 255);
    list.ScrollDown();
    if (g_rows[1].Colour.A != 255 || g_rows[0].Colour.A != 0 || g_rows[3].Colour.A != 0 || g_rows[2].Colour.A != 0) ok = Fail("clamp");
    if (ok)
        printf("FRONTEND_0054c810_TEST PASS\n");
    else
        printf("FRONTEND_0054c810_TEST did not pass\n");
    return ok ? 0 : 1;
}
