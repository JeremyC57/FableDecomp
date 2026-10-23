// CRenderManager2D::Draw2DGraphic(const C2DVector& pos, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frame, unsigned long flags, unsigned long colour) -- queue a bank graphic frame at its natural size with its top-left at a screen position as two triangles of one colour, nudging its UVs by 0.01 texel, flipping them for flags 0x10 / 0x20 and picking the blend mode from the flags and the frame data
// Leaf: Draw2DGraphic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DCA80, length 971 bytes (ret 0x18); PDB
// ?Draw2DGraphic@CRenderManager2D@@QAE_NABVC2DVector@@ABVCGraphicDataBank@@KKKK@Z.
// Callees: GFAXToVX 0x009E1E10 / GFAYToVY 0x009E1E40 (__fastcall, float on the stack), the bank's virtual frame width
// (slot 21, +0x54) and height (slot 22, +0x58) as unsigned longs, CGraphicInfo::GetFrame 0x009FF8D0,
// CGraphicFrameData::GetTextureQuadUVs 0x009FC810 (hidden result), CTexture::GetDimensions 0x009F9DB0 (hidden result),
// CGraphicFrameData::Uninitialise 0x009FCBA0 and operator delete 0x00BFE9BC (the inline ~CGraphicFrame), Add2DTri
// 0x009DC030 twice. Data: the corner orders {0,1,2,3} 0x0129B94C, {1,0,3,2} 0x0129B95C, {3,2,1,0} 0x0129B96C; 0.01f
// 0x0129B944, 0.0f 0x0122DEDC, -0.01f 0x01231564, 1.0f 0x0122DED8, 2^32 0x0122DCB4, the screen RHW 0x0139652C.
// Layout: CGraphicDataBank's 0x2c-byte CGraphicInfo vector at +0x1e0; CGraphicFrameData {ReferenceCount +0, PTexture
// {Data +8, Info +0xc}, flags +0x1a} as the landed CGraphicFrame::operator= 0x0041C0E0.
// Shape: the sibling Draw2DScaledGraphic 0x009DD210 with the box built from the position plus the bank's frame size
// (each width/height use calls the bank again).
struct C2DVector { float X, Y; };
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
    virtual void b00() const; virtual void b01() const; virtual void b02() const; virtual void b03() const;
    virtual void b04() const; virtual void b05() const; virtual void b06() const; virtual void b07() const;
    virtual void b08() const; virtual void b09() const; virtual void b10() const; virtual void b11() const;
    virtual void b12() const; virtual void b13() const; virtual void b14() const; virtual void b15() const;
    virtual void b16() const; virtual void b17() const; virtual void b18() const; virtual void b19() const;
    virtual void b20() const;
    virtual unsigned long GetFrameWidth(unsigned long graphic, unsigned long frame) const;    // slot 21 (+0x54)
    virtual unsigned long GetFrameHeight(unsigned long graphic, unsigned long frame) const;   // slot 22 (+0x58)
    unsigned char               _pad[0x1e0 - 4];
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
    bool Draw2DGraphic(const C2DVector& pos, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frameIndex, unsigned long flags, unsigned long colour);
};

bool CRenderManager2D::Draw2DGraphic(const C2DVector& pos, const CGraphicDataBank& bank, unsigned long graphic, unsigned long frameIndex, unsigned long flags, unsigned long colour)
{
    const unsigned long* order = s_CornerOrderNormal;
    CTVertexRHWColSpecTex1 v[4];
    CTextureQuadUVs uvs;
    v[0].X = GFAXToVX(pos.X);
    v[0].Y = GFAYToVY(pos.Y);
    v[0].Diffuse = colour;
    v[1].X = v[0].X + bank.GetFrameWidth(graphic, frameIndex);
    v[1].Y = v[0].Y;
    v[1].Diffuse = colour;
    v[2].X = v[0].X + bank.GetFrameWidth(graphic, frameIndex);
    v[2].Y = v[0].Y + bank.GetFrameHeight(graphic, frameIndex);
    v[2].Diffuse = colour;
    v[3].X = v[0].X;
    v[3].Y = v[0].Y + bank.GetFrameHeight(graphic, frameIndex);
    v[3].Diffuse = colour;
    CGraphicFrame frame;
    if (!bank.Get(graphic)->GetFrame(frame, frameIndex, 0, 1))
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
