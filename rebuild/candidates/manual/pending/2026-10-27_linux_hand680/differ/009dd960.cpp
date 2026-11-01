// CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags) -- queue a screen-space line as two RHW vertices in the colour, with the blend mode from the flags and the colour's alpha
// Grade: DIFFER(227v229) -- the RHW copy: retail mov edx,[0139652Ch] ; mov eax,edx ; mov [esp+20h],eax ... mov [esp+40h],edx (integer copies, one load); built fld / fst / fstp (FPU copy of v1.RHW into v0.RHW).
// Next step: both vertices reading the global directly gives integer copies but two loads (233v229) or, adjacent, one load with the
//   copy in the wrong register (229v229, 26 lines); try the landed Draw2DTriangle/Draw2DBoxColours vertex model (array, field
//   order) with the first-vertex RHW read back through the second, and a by-value CTVertexRHWColSpecTex1 assignment.
// Leaf: ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z
// Retail: Fable.exe @ 0x009DD960, 229 bytes. Callees: GFVXToAX 0x009E1E10, GFVYToAY 0x009E1E40, CRenderManager2D::Add2DLine 0x009DC2C0.
//   The float at 0x0139652C is a .data float (100.0); landed siblings spell it g_ScreenRHW (descriptive; ChatGPT's
//   BIGGEST_RHW_VERTEX_VALUE is in neither PDB name set); respelled here to the landed g_ScreenRHW (one address, one name).
// Model: ChatGPT pass 10 attempt 2, unchanged apart from this header.
class C2DVector {public: float X, Y; };
class CRGBColour {public: unsigned char B, G, R, A; };
class CTexture;

class CTVertexRHWColSpecTex1Base {public:                 // 0x20 bytes
    float         X, Y, Z;                      // +0x00
    float         RHW;                          // +0x0c
    unsigned long DiffuseColour;                       // +0x10 D3DCOLOR
    unsigned long SpecularColour;                     // +0x14
    float         U1, V1;                         // +0x18
};

template<class T>class CGenericVertex:public T{public:void SetRHW(float value){this->RHW=value;}float GetRHW()const{return this->RHW;}};
class CTVertexRHWColSpecTex1:public CGenericVertex<CTVertexRHWColSpecTex1Base>{};
float __fastcall GFVXToAX(float x);              // 0x009E1E10
float __fastcall GFVYToAY(float y);              // 0x009E1E40
extern float g_ScreenRHW;                     // 0x0139652C

class CRenderManager2D {private:
    void Add2DLine(const CTVertexRHWColSpecTex1& start, const CTVertexRHWColSpecTex1& end, long mode, CTexture* texture);   // 0x009DC2C0
public:bool Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = ((unsigned long)colour.A << 24) | ((unsigned long)colour.R << 16) | ((unsigned long)colour.G << 8) | colour.B;
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v1.RHW = g_ScreenRHW;
    v0.DiffuseColour = argb;
    v1.DiffuseColour = argb;
    v0.SpecularColour = 0;
    v1.SpecularColour = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v0.RHW = v1.RHW;
    v1.U1 = 0.0f;
    v0.U1 = 0.0f;
    v1.V1 = 0.0f;
    v0.V1 = 0.0f;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    }
    else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(v0, v1, mode, 0);
    return true;
}
