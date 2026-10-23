// CSprite::Draw(CDisplayEngine*, CBatcher*, long layer, ..., long* counter) -- skip (and drop the cached primitive) when the sprite is fully transparent or zero-sized, re-emit a cached batch entry when one is given; otherwise compute the snapped on-screen rectangle (half-size offset when centred), pick the layer, and build the sprite quad either from an explicit texture or from the frame's texture in the sprite bank (refreshing the bank's texture handle), then queue it in the batcher or draw it directly
// Grade: DIFFER(1041v1455) -- lane gap: the four corner snaps are bare `fistp ; fild` (inline-asm ROUND); first draft of the visibility / cached-batch / rectangle / layer / quad flow; the inlined sprite-bank texture refresh (state refcount, lock and upload, about 400 bytes in retail) is modelled as two out-of-line calls
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041AFA0, 1456 bytes, `ret 0x14`; the four corner snaps are bare `fistp ; fild` pairs (inline-asm ROUND in the original: lane gap).
inline void* operator new(unsigned int, void* where) { return where; }
struct CRGBColour { unsigned char B, G, R, A; };
struct CRect { float X0, Y0, X1, Y1; };
struct C2DVector { float X, Y; };
struct CTextureHandle { long* RefCount; void Release(); };   // 0x004194B5
struct CBankTexture {
    unsigned char _pad00[8];
    struct CTextureState* State;                        // +0x08
    struct CTextureOwner* Owner;                        // +0x0c
    void Prepare(long a, long b);                       // 0x009FE620
    bool Upload(long flags);                            // 0x009FF6B0
};
struct CTextureState { long RefCount; unsigned char _pad04[0x1c - 4]; struct CTextureData* Data; unsigned char _pad20[0x30 - 0x20]; long Pending; unsigned char _pad34[0x38 - 0x34]; bool Dirty; unsigned char _pad39; bool Locked; unsigned char _pad3b; long Frame; void Touch(long a, long b); };   // 0x009FDE80
struct CTextureData { long RefCount; };
struct CSpriteFrame { unsigned char _pad[0x28]; CBankTexture* Texture; };
struct CSpriteBank { unsigned char _pad[0x1e0]; CSpriteFrame* FramesFirst; CSpriteFrame* FramesLast; };
struct CBankHandle { CSpriteBank* Bank; long* RefCount; };
struct CDisplayManager {
    unsigned char _pad[0x10];
    CBankHandle SpriteBank;                             // +0x10
};
CDisplayManager* GetDisplayManager();                   // 0x0041E5F2
struct CLayer;
struct CLayerSource {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1c();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2c(); virtual void s30(); virtual void s34(); virtual void s38(); virtual void s3c();
    virtual void s40(); virtual void s44(); virtual void s48(); virtual void s4c(); virtual void s50(); virtual void s54(); virtual void s58(); virtual void s5c();
    virtual void s60(); virtual void s64(); virtual void s68(); virtual void s6c(); virtual void s70(); virtual void s74(); virtual void s78(); virtual void s7c();
    virtual void s80(); virtual void s84(); virtual void s88(); virtual void s8c();
    virtual CLayer* GetLayer(long index);               // +0x90
};
struct SSpriteQuad {
    unsigned char _data[0x44];
    SSpriteQuad(long a, long b, C2DVector* uv, CLayer* layer, void* texture, long c, long d, long e, long f, CRGBColour col, long g, CRect* rect, void* h);   // 0x0041BF60
    SSpriteQuad(long a, long b, C2DVector* uv, CLayer* layer, long frame, long c, long d, long e, long f, CRGBColour col, long g, CRect* rect, void* h, int);   // 0x0041BEB0
};
struct CBatcher {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1c();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2c(); virtual void s30(); virtual void s34(); virtual void s38(); virtual void s3c();
    virtual void s40(); virtual void s44(); virtual void s48(); virtual void s4c(); virtual void s50(); virtual void s54(); virtual void s58();
    virtual void Draw(void* primitive, void* quad, long size);   // +0x5c
    virtual void s60(); virtual void s64(); virtual void s68(); virtual void s6c();
    virtual void Queue(long batch, long index, void* quad, long size);   // +0x70
    virtual void s74();
    virtual void Requeue(long batch, long index);       // +0x78
};
struct CPrimitiveHolder {
    virtual void s00(); virtual void Release(); virtual void s08();
    virtual void OnLastHandleReleased(void* handle);    // +0x0c
    long RefCount;
};
long ROUND(float value);

class CSprite {
public:
    virtual void s000();
    unsigned char _pad04[0x7c - 0x04];
    float ScaleX;                                       // +0x7c
    float ScaleY;                                       // +0x80
    unsigned char _pad84[0x94 - 0x84];
    CRGBColour Colour;                                  // +0x94
    unsigned char _pad98[0xcc - 0x98];
    float Width;                                        // +0xcc
    float Height;                                       // +0xd0
    unsigned char _padd4[0xf8 - 0xd4];
    float X;                                            // +0xf8
    float Y;                                            // +0xfc
    unsigned char _pad100[0x108 - 0x100];
    float SizeX;                                        // +0x108
    float SizeY;                                        // +0x10c
    unsigned char _pad110[0x12f - 0x110];
    signed char LayerOffset;                            // +0x12f
    unsigned char _pad130[0x15c - 0x130];
    void* PrimitiveHandle;                              // +0x15c (vtable)
    CPrimitiveHolder* Primitive;                        // +0x160
    long Blend;                                         // +0x164
    unsigned long OverrideWidth;                        // +0x168
    unsigned long OverrideHeight;                       // +0x16c
    bool Visible;                                       // +0x170
    unsigned char _pad171[0x174 - 0x171];
    long Flags;                                         // +0x174
    unsigned long Frame;                                // +0x178
    void* Texture;                                      // +0x17c
    virtual bool IsAbsolute();                          // +0x194 (stand-in order)
    virtual bool IsScreenSpace();                       // +0x1a0
    virtual bool IsCentred();                           // +0x1a8
    void Draw(CBatcher* batcher, long batch, long layer, long* counter, long unused);
};

void CSprite::Draw(CBatcher* batcher, long batch, long layer, long* counter, long unused)
{
    if (Colour.A == 0 || ScaleX <= 0.0f || ScaleY <= 0.0f) {
        Visible = false;
    } else if (Visible == true) {
        if (batch) {
            long index = (*counter)++;
            batcher->Requeue(batch, index);
            return;
        }
        if (Primitive && Primitive->RefCount == 1)
            Primitive->OnLastHandleReleased(&PrimitiveHandle);
        if (Primitive) {
            if (--Primitive->RefCount == 0)
                Primitive->Release();
            Primitive = 0;
        }
        Primitive = 0;
        return;
    } else
        Visible = true;
    float w = OverrideWidth == 0 ? Width : (float)OverrideWidth;
    float h = OverrideHeight == 0 ? Height : (float)OverrideHeight;
    CRect rect;
    w *= SizeX;
    h *= SizeY;
    CRGBColour colour = Colour;
    rect.X0 = X;
    rect.Y0 = Y;
    rect.X1 = w + X;
    rect.Y1 = h + Y;
    if (IsCentred()) {
        float x0 = X - w * 0.5f;
        float y0 = Y - h * 0.5f;
        rect.X1 = x0 - X + rect.X1;
        rect.Y1 = y0 - Y + rect.Y1;
        rect.X0 = x0;
        rect.Y0 = y0;
    }
    rect.X0 = (float)ROUND(rect.X0);
    rect.Y0 = (float)ROUND(rect.Y0);
    rect.X1 = (float)ROUND(rect.X1);
    rect.Y1 = (float)ROUND(rect.Y1);
    bool absolute = IsAbsolute();
    bool screen = IsScreenSpace();
    CLayer* target;
    if (!absolute && !screen)
        target = ((CLayerSource*)GetDisplayManager())->GetLayer(LayerOffset + layer);
    else
        target = ((CLayerSource*)GetDisplayManager())->GetLayer(LayerOffset);
    C2DVector uv[2];
    unsigned char quad[0x44];
    if (Texture) {
        uv[0].X = uv[0].Y = 0.0f;
        uv[1].X = uv[1].Y = 0.0f;
        new (quad) SSpriteQuad(0, 0, uv, target, Texture, 2, 0, Blend, Blend, colour, Flags, &rect, &w);
    } else {
        if (Frame) {
            CBankHandle bank = GetDisplayManager()->SpriteBank;
            if (bank.RefCount)
                ++*bank.RefCount;
            CBankTexture* texture = 0;
            unsigned long count = bank.Bank->FramesLast - bank.Bank->FramesFirst;
            if (Frame < count)
                texture = bank.Bank->FramesFirst[Frame].Texture;
            ((CTextureHandle*)&bank.RefCount)->Release();
            texture->Prepare(0, 0x10);
            texture->Upload(0x11);
        }
        uv[0].X = uv[0].Y = 0.0f;
        uv[1].X = uv[1].Y = 0.0f;
        new (quad) SSpriteQuad(0, 0, uv, target, Frame, 0, 2, 0, Blend, colour, Flags, &rect, &w, 0);
    }
    if (batch && counter) {
        long index = (*counter)++;
        batcher->Queue(batch, index, quad, 0xc0);
        return;
    }
    batcher->Draw(&PrimitiveHandle, quad, 0xc0);
}
