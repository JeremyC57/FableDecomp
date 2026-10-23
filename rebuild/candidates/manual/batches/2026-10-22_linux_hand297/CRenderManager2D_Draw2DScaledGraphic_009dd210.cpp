// CRenderManager2D::Draw2DScaledGraphic(const C2DBoxF& box, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frame, unsigned long flags) -- queue a bank graphic frame stretched over a screen-space box as two white triangles, nudging its UVs by 0.01 texel, flipping them for flags 0x10 / 0x20 and picking the blend mode from the flags and the frame data
// Leaf: Draw2DScaledGraphic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DD210, length 860 bytes (ret 0x14); PDB
// ?Draw2DScaledGraphic@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCGraphicDataBank@@KKK@Z.
// Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__fastcall, float on the stack), CGraphicInfo::GetFrame 0x009FF8D0,
// CGraphicFrameData::GetTextureQuadUVs 0x009FC810 (hidden result), CTexture::GetDimensions 0x009F9DB0 (hidden result),
// CGraphicFrameData::Uninitialise 0x009FCBA0 and operator delete 0x00BFE9BC (the inline ~CGraphicFrame), Add2DTri
// 0x009DC030 twice. Data: the corner orders {0,1,2,3} 0x0129B94C, {1,0,3,2} 0x0129B95C, {3,2,1,0} 0x0129B96C; 0.01f
// 0x0129B944, 0.0f 0x0122DEDC, -0.01f 0x01231564, 1.0f 0x0122DED8, the screen RHW 0x0139652C.
// Layout: CGraphicDataBank's 0x2c-byte CGraphicInfo vector at +0x1e0; CGraphicFrameData {ReferenceCount +0, PTexture
// {Data +8, Info +0xc}, flags +0x1a} as the landed CGraphicFrame::operator= 0x0041C0E0.
// Shape: the counted frame release of the landed CEnginePrimitiveManagerSpriteTrails::RenderSubPrimitive 0x00BCB800.
// The vertex positions are set in a block (its `left` copy shares the frame's slot); chained `a = b = f()` stores b
// first; the UVs are copied from a const-reference-bound temporary (`lea esi` source; plain assignment copies from the
// returned pointer, direct initialisation drops the copy); the texture goes through an inline GetTexture() accessor
// (loads `this` before the pushes); the 0.01-texel nudge divides inside the loop so both quotients are hoisted onto the
// x87 stack; the RHW is a void helper over the vertex reference whose three arms assign (the first an integer copy) and
// whose z reads go back to memory.
void __cdecl operator delete(void* block);             // 0x00BFE9BC
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CTVertexRHWColSpecTex1 {                        // 32 bytes
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI();
};
struct CTexture {
    C2DExtentsI GetDimensions(unsigned long mip) const;   // 0x009F9DB0
};
struct CTextureQuadUVs {
    float U[4];                                        // TL, TR, BR, BL
    float V[4];
};
struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;                            // +0x00
    SCountBlock* m_pCount;                             // +0x04
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;           // +0x00
    void*                    Data;                     // +0x04
    CCountedPointer_CTexture PTexture;                 // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;                  // +0x1a
    void Uninitialise();                               // 0x009FCBA0
    CTextureQuadUVs GetTextureQuadUVs() const;         // 0x009FC810
    CTexture* GetTexture() const { return PTexture.m_pObject; }
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            --data->ReferenceCount;
            if (data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData = 0;
        }
    }
};
struct CGraphicInfo {                                  // 0x2c bytes
    unsigned char _pad[0x2c];
    bool GetFrame(CGraphicFrame& frame, unsigned long frameIndex, unsigned long a, unsigned long flags) const;   // 0x009FF8D0
};
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    const T* begin() const { return m_pFirst; }
    const T* end() const { return m_pLast; }
    unsigned long size() const { return end() - begin(); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CGraphicDataBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<CGraphicInfo> m_records;             // +0x1e0
    __forceinline const CGraphicInfo* Get(unsigned long index) const { return index < m_records.size() ? &m_records[index] : 0; }
};
float __fastcall GFAXToVX(float x);                    // 0x009E1E10
float __fastcall GFAYToVY(float y);                    // 0x009E1E40
extern float g_ScreenRHW;                              // 0x0139652C
static const unsigned long s_CornerOrderNormal[4] = { 0, 1, 2, 3 };    // 0x0129B94C
static const unsigned long s_CornerOrderFlipX[4]  = { 1, 0, 3, 2 };    // 0x0129B95C
static const unsigned long s_CornerOrderFlipY[4]  = { 3, 2, 1, 0 };    // 0x0129B96C

static __forceinline float GetRHWForZ(const float& z)
{
    if (z >= 0.0f && z < 0.01f)
        return g_ScreenRHW;
    if (z >= -0.01f && z < 0.0f)
        return -g_ScreenRHW;
    return 1.0f / z;
}

static __forceinline void SetRHWForZ(CTVertexRHWColSpecTex1& vert)
{
    if (vert.Z >= 0.0f && vert.Z < 0.01f)
        vert.RHW = g_ScreenRHW;
    else if (vert.Z >= -0.01f && vert.Z < 0.0f)
        vert.RHW = -g_ScreenRHW;
    else
        vert.RHW = 1.0f / vert.Z;
}
struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long blendMode, CTexture* texture);  // 0x009DC030
    bool Draw2DScaledGraphic(const C2DBoxF& box, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frameIndex, unsigned long flags);
};

bool CRenderManager2D::Draw2DScaledGraphic(const C2DBoxF& box, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frameIndex, unsigned long flags)
{
    const unsigned long* order = s_CornerOrderNormal;
    CTVertexRHWColSpecTex1 v[4];
    CTextureQuadUVs uvs;
    {
        float left = box.TLX;
        v[0].X = GFAXToVX(left);
        v[0].Y = GFAYToVY(box.TLY);
        v[0].Diffuse = 0xffffffff;
        v[2].X = v[1].X = GFAXToVX(box.BRX);
        v[1].Y = v[0].Y;
        v[1].Diffuse = 0xffffffff;
        v[3].Y = v[2].Y = GFAYToVY(box.BRY);
        v[3].X = v[0].X;
        v[2].Diffuse = 0xffffffff;
        v[3].Diffuse = 0xffffffff;
    }
    CGraphicFrame frame;
    if (!bank.Get(graphic)->GetFrame(frame, frameIndex, 0, 0x11))
        return false;
    const CTextureQuadUVs& quad = frame.FrameData->GetTextureQuadUVs();
    uvs = quad;
    if (flags & 0x10)
        order = s_CornerOrderFlipX;
    else if (flags & 0x20)
        order = s_CornerOrderFlipY;
    long width = frame.FrameData->GetTexture()->GetDimensions(0).X;
    long height = frame.FrameData->GetTexture()->GetDimensions(0).Y;
    int i;
    for (i = 0; i < 4; ++i) {
        uvs.U[i] += 0.01f / width;
        uvs.V[i] += 0.01f / height;
    }
    for (i = 0; i < 4; ++i) {
        v[i].U = uvs.U[order[i]];
        v[i].V = uvs.V[order[i]];
        v[i].Z = 0.0f;
        SetRHWForZ(v[i]);
    }
    long mode;
    if (flags & 1)
        mode = 3;
    else if (flags & 2)
        mode = 2;
    else if ((frame.FrameData->m_flags & 2) || (flags & 4))
        mode = 4;
    else
        mode = (frame.FrameData->m_flags & 1) == 1 ? 3 : 1;
    Add2DTri(v[0], v[1], v[2], mode, frame.FrameData->PTexture.m_pObject);
    Add2DTri(v[0], v[2], v[3], mode, frame.FrameData->PTexture.m_pObject);
    return true;
}
