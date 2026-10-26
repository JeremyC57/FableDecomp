// CTCInGameMenu::DrawPreparePrimitivesForRendering(const CDrawPrepareContext&) -- while the menu is open, ticks every UI component by the real time since the last render and lets it prepare its primitives
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D1AC, 112 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: GFGetTime() @ 0x009E1BC0 (__fastcall, double; twice), NUISystem::CComponent vtable slots 1 (+0x04,
//          Update(float)) and 2 (+0x08, DrawPreparePrimitives(target, 0, 0, 0, 0)).
// Shape: one zero register (ebx) serves the InGameMenuOpen byte test and the four zero arguments; the frame time
// is `(float)(GFGetTime() - LastRenderTime)` into the `push ecx` frame word, then LastRenderTime is re-stamped;
// `for (unsigned i = 0; i < Components.size(); ++i)` re-reads the begin pointer for each call (`*(begin() + i)`).
// Layout: retail CTCInGameMenu with 12-byte vectors: InGameMenuOpen +0x0C, Components +0x58, LastRenderTime +0xA0
// (PDB +0x0C/+0x68/+0xC0).
#pragma optimize("y", off)
#pragma optimize("s", on)

double __fastcall GFGetTime();                       // 0x009E1BC0

struct CComponent {
    virtual void v00();
    virtual void Update(float seconds);              // slot 1 (+0x04)
    virtual void DrawPreparePrimitives(void* target, long a, long b, long c, long d);   // slot 2 (+0x08)
};

struct CDrawPrepareContext {
    unsigned char _pad_0x00[0x34];
    void*         Target;                            // +0x34
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInGameMenu { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x4];
    bool          InGameMenuOpen;                    // +0x0C
    unsigned char _pad_0x0d[0x4B];
    CRetailVector<CComponent*> Components;           // +0x58
    unsigned char _pad_0x64[0x3C];
    double        LastRenderTime;                    // +0xA0

    void DrawPreparePrimitivesForRendering(const CDrawPrepareContext& context);
};

void CTCInGameMenu::DrawPreparePrimitivesForRendering(const CDrawPrepareContext& context)
{
    if (InGameMenuOpen) {
        float seconds = (float)(GFGetTime() - LastRenderTime);
        LastRenderTime = GFGetTime();
        for (unsigned long i = 0; i < Components.size(); ++i) {
            Components[i]->Update(seconds);
            Components[i]->DrawPreparePrimitives(context.Target, 0, 0, 0, 0);
        }
    }
}