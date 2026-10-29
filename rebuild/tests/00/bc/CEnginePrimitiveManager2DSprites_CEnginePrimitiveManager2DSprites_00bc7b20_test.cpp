// Behaviour test for CEnginePrimitiveManager2DSprites::~CEnginePrimitiveManager2DSprites @ 0x00BC7B20 (release order and counts).
#include <cstdio>
static int g_log[16]; static int g_n = 0;
class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() { g_log[g_n++] = 99; } };
struct CPixelShaderObject { int id; long m_refCount; virtual ~CPixelShaderObject() { g_log[g_n++] = id; } };
struct CVertexShaderObject { int id; long m_refCount; virtual ~CVertexShaderObject() { g_log[g_n++] = id; } };
class CShaderBase { public: virtual ~CShaderBase() { } };
class CPixelShader : public CShaderBase { public:
    CPixelShaderObject* m_pShader;
    virtual ~CPixelShader() { CPixelShaderObject* s = m_pShader; if (s) { if (--s->m_refCount == 0) delete s; } }
};
class CVertexShader : public CShaderBase { public:
    CVertexShaderObject* m_pShader;
    virtual ~CVertexShader() { CVertexShaderObject* s = m_pShader; if (s) { if (--s->m_refCount == 0) delete s; } }
};
class CEnginePrimitiveManager2DSprites : public CBaseClassNonCopyable { public:
    unsigned char active; CVertexShader VertexShader2DSprite, VertexShader2DClockSprite;
    CPixelShader PixelShader2DClockSprite, PixelShader2DClockSpriteAdditive; unsigned short Indices[6];
    virtual ~CEnginePrimitiveManager2DSprites() { }
};
int main()
{
    CVertexShaderObject* v1 = new CVertexShaderObject; v1->id = 1; v1->m_refCount = 1;
    CVertexShaderObject* v2 = new CVertexShaderObject; v2->id = 2; v2->m_refCount = 2;
    CPixelShaderObject* p1 = new CPixelShaderObject; p1->id = 3; p1->m_refCount = 1;
    CEnginePrimitiveManager2DSprites* m = new CEnginePrimitiveManager2DSprites;
    m->VertexShader2DSprite.m_pShader = v1; m->VertexShader2DClockSprite.m_pShader = v2;
    m->PixelShader2DClockSprite.m_pShader = 0; m->PixelShader2DClockSpriteAdditive.m_pShader = p1;
    delete m;
    // reverse order: additive pixel (p1 destroyed), clock pixel (null), clock vertex (v2 2->1), sprite vertex (v1 destroyed), CBase
    int ok = g_n == 3 && g_log[0] == 3 && g_log[1] == 1 && g_log[2] == 99 && v2->m_refCount == 1;
    delete v2;
    if (!ok) { std::printf("00bc7b20_TEST FAIL\n"); return 1; }
    std::printf("00bc7b20_TEST PASS\n");
    return 0;
}