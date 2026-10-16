// behaviour test for CTavernGameCardBaseDef::operator= (0x004EBC16): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
 struct CRGBColour { unsigned char B, G, R, A; }; struct C2DVector { float X, Y; };struct CCardPositionsVector { unsigned long m_words[3]; CCardPositionsVector& operator=(const CCardPositionsVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };struct CCardDefVector { unsigned long m_words[3]; CCardDefVector& operator=(const CCardDefVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CTavernGameCardBaseDef : CDefClassBase {
    long ControlType;
    float CardWidth;
    float CardHeight;
    float ImpulseScaleXbox;
    float ImpulseScalePC;
    long PointerModel;
    float MoveSpeed;
    float TurnSpeed;
    float PointerHeightOffset;
    long PointerParticleEffect;
    float HighlightWidth;
    CRGBColour HighlightColourGood;
    CRGBColour HighlightColourBad;
    float CardSeparation;
    float TableSeparation;
    C2DVector PackOffset;
    CCardPositionsVector CardOrders;
    CCardDefVector CardPack;
    CTavernGameCardBaseDef& operator=(const CTavernGameCardBaseDef& other);
};
CTavernGameCardBaseDef& CTavernGameCardBaseDef::operator=(const CTavernGameCardBaseDef& other)
{
    CDefClassBase::operator=(other);
    ControlType = other.ControlType;
    CardWidth = other.CardWidth;
    CardHeight = other.CardHeight;
    ImpulseScaleXbox = other.ImpulseScaleXbox;
    ImpulseScalePC = other.ImpulseScalePC;
    PointerModel = other.PointerModel;
    MoveSpeed = other.MoveSpeed;
    TurnSpeed = other.TurnSpeed;
    PointerHeightOffset = other.PointerHeightOffset;
    PointerParticleEffect = other.PointerParticleEffect;
    HighlightWidth = other.HighlightWidth;
    HighlightColourGood = other.HighlightColourGood;
    HighlightColourBad = other.HighlightColourBad;
    CardSeparation = other.CardSeparation;
    TableSeparation = other.TableSeparation;
    PackOffset = other.PackOffset;
    CardOrders = other.CardOrders;
    CardPack = other.CardPack;
    return *this;
}
int main()
{
    CTavernGameCardBaseDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CTavernGameCardBaseDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.ControlType; const unsigned char* y = (const unsigned char*)&b.ControlType; for (unsigned int i = 0; i < sizeof(a.ControlType); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CardWidth; const unsigned char* y = (const unsigned char*)&b.CardWidth; for (unsigned int i = 0; i < sizeof(a.CardWidth); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CardHeight; const unsigned char* y = (const unsigned char*)&b.CardHeight; for (unsigned int i = 0; i < sizeof(a.CardHeight); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ImpulseScaleXbox; const unsigned char* y = (const unsigned char*)&b.ImpulseScaleXbox; for (unsigned int i = 0; i < sizeof(a.ImpulseScaleXbox); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ImpulseScalePC; const unsigned char* y = (const unsigned char*)&b.ImpulseScalePC; for (unsigned int i = 0; i < sizeof(a.ImpulseScalePC); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PointerModel; const unsigned char* y = (const unsigned char*)&b.PointerModel; for (unsigned int i = 0; i < sizeof(a.PointerModel); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.MoveSpeed; const unsigned char* y = (const unsigned char*)&b.MoveSpeed; for (unsigned int i = 0; i < sizeof(a.MoveSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TurnSpeed; const unsigned char* y = (const unsigned char*)&b.TurnSpeed; for (unsigned int i = 0; i < sizeof(a.TurnSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PointerHeightOffset; const unsigned char* y = (const unsigned char*)&b.PointerHeightOffset; for (unsigned int i = 0; i < sizeof(a.PointerHeightOffset); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PointerParticleEffect; const unsigned char* y = (const unsigned char*)&b.PointerParticleEffect; for (unsigned int i = 0; i < sizeof(a.PointerParticleEffect); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HighlightWidth; const unsigned char* y = (const unsigned char*)&b.HighlightWidth; for (unsigned int i = 0; i < sizeof(a.HighlightWidth); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HighlightColourGood; const unsigned char* y = (const unsigned char*)&b.HighlightColourGood; for (unsigned int i = 0; i < sizeof(a.HighlightColourGood); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HighlightColourBad; const unsigned char* y = (const unsigned char*)&b.HighlightColourBad; for (unsigned int i = 0; i < sizeof(a.HighlightColourBad); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CardSeparation; const unsigned char* y = (const unsigned char*)&b.CardSeparation; for (unsigned int i = 0; i < sizeof(a.CardSeparation); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TableSeparation; const unsigned char* y = (const unsigned char*)&b.TableSeparation; for (unsigned int i = 0; i < sizeof(a.TableSeparation); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PackOffset; const unsigned char* y = (const unsigned char*)&b.PackOffset; for (unsigned int i = 0; i < sizeof(a.PackOffset); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CardOrders; const unsigned char* y = (const unsigned char*)&b.CardOrders; for (unsigned int i = 0; i < sizeof(a.CardOrders); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CardPack; const unsigned char* y = (const unsigned char*)&b.CardPack; for (unsigned int i = 0; i < sizeof(a.CardPack); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004ebc16_TEST FAIL\n"); return 1; }
    std::printf("004ebc16_TEST PASS\n");
    return 0;
}
