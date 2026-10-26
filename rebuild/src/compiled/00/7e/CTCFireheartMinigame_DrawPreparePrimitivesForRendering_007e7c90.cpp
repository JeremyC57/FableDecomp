// CTCFireheartMinigame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- drop last frame's primitives; while the delayed quit runs, cover the render target with a full-screen sprite
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7C90, length 230 bytes (+ int3 padding). __fastcall, `ret 4`.
//         CTCFireheartMinigame vtable slot 11 (the catalog's "vfunc_11"); Ego_r.pdb
//         `?DrawPreparePrimitivesForRendering@CTCFireheartMinigame@@UAEXABVCEnginePrimitivePrepareInfo@@@Z`.
// Callees: vector<CEnginePrimitiveHandle>::erase(first, last) @ 0x00440D70, CDisplayManager::GetRenderTargetDimensions()
//          const @ 0x009BEDC0 (C2DExtentsI by hidden pointer) on g_pDisplayDevice @ 0x013B8390,
//          CEnginePrimitive2DSprite ctor (16 arguments) @ 0x0041BEB0, the handle vector's grow-by-one @ 0x005ED940, the
//          renderer's virtual slot 23 (+0x5C) on info +0x34.
// Shape: the box is a NAMED local (`C2DBoxF box(0, 0, w, h)`) and the two uv C2DVector(0, 0) are temporaries in the
//   sprite constructor call: the named box pins its TopLeft zero stores ABOVE the uv zero stores (a box temporary sinks
//   them below).
// Layout: retail CTCFireheartMinigame: QuitWithDelay +0x88, Handles +0x8C (12-byte STLport vector of 8-byte handles).
class CEnginePrimitiveHandle {
public:
    long  m_header;                                 // +0x00
    void* Data;                                     // +0x04
};

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void erase(T* first, T* last);                  // 0x00440D70
    void clear() { erase(_M_start, _M_finish); }
    void push_back_default();                       // 0x005ED940
};

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DBoxF { public:
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
class C2DExtentsI { public: long X, Y; };

class CDisplayManager {
public:
    C2DExtentsI GetRenderTargetDimensions() const;  // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;           // 0x013B8390

class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long a, unsigned long b, long blend,
                             long interpolation, float c, float d, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned char alpha, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
};

class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};

class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};

class CTCFireheartMinigame {
public:
    virtual void v00();
    unsigned char _pad04[0x88 - 0x04];
    bool QuitWithDelay;                             // +0x88
    unsigned char _pad89[3];
    CRetailVector<CEnginePrimitiveHandle> Handles;  // +0x8C
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCFireheartMinigame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    Handles.clear();
    if (QuitWithDelay) {
        C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
        C2DBoxF box(0.0f, 0.0f, (float)size.X, (float)size.Y);
        CEnginePrimitive2DSprite sprite(0, box, 0x4a, 0, 0, 2, 0, 0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        Handles.push_back_default();
        info.Renderer->AddPrimitive(Handles._M_finish - 1, sprite, 0x40);
    }
}