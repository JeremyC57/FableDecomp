// NUISystem::CFrontEndList::ScrollDown() -- move the selection one row down (wrapping to the top) and rotate the rows'
//   colours up one place; scrolling lists also slide every row by (0, -30) and fade rows by distance from the selection.
// Leaf: ?ScrollDown@CFrontEndList@NUISystem
// Retail: Fable.exe @ 0x0054C810, 977 bytes, /O2 FPO frame (sub esp,20h), plain `ret`.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), its slots 40 (+0xa0 PlaySound by def member) and 42
//   (+0xa8 the CUIMiscThingsDef by value: SoundUpDown +0x194, SoundError +0x1a4 = Ego_r.pdb 412/428 minus 8); own slots
//   54 (+0xd8 GetUpdateTime) and 171 (+0x2ac IsScrolling); row slots 32 (+0x80 SetPosition), 38 (+0x98 SetColour),
//   48 (+0xc0 SetCurrentState), 55 (+0xdc GetPosition), 56 (+0xe0 GetColour), by-value results through a hidden pointer.
// Layout (retail = PDB - 0x30): SelectedChild +0x15c, the row vector +0x164, the alpha step byte +0x1f8.
// Shape: the update time is `GetUpdateTime() != -1.0f ? GetUpdateTime() : 0.0f` (fucompp, the call re-made on the
//   kept arm); the white colour's ctor assigns R, G, B, A in its body (retail's +2,+1,+0,+3 store order); abs is the
//   extern "C" CRT intrinsic (cdq/xor/sub), and the wrapped selection `x < size ? x : 0` gives retail's sbb/and.
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
