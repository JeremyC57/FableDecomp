// Behaviour test for 0x007B74A0 CEnginePrimitiveSprite::CEnginePrimitiveSprite (C2DVector offset overload): the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class CRGBColour { public:                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_DEFAULT = 0 };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_DEFAULT = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT = 0 };
enum EEnginePrimitiveType { ENGINE_PRIMITIVE_SPRITE = 7 };

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    EEnginePrimitiveType             Type;               // +0x00
    EEngineInterpolationSet          InterpolationSet;   // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(EEnginePrimitiveType type, EEngineInterpolationSet interpolationSet)
        : Type(type), InterpolationSet(interpolationSet) { }
};

class CEnginePrimitiveSprite : public CEnginePrimitive { public:
    C3DVector                 WorldPos;                  // +0x0c
    unsigned long             BankIndex;                 // +0x18
    unsigned long             Frame;                     // +0x1c
    float                     SizeX;                     // +0x20
    float                     SizeY;                     // +0x24
    float                     FadeStart;                 // +0x28
    float                     Angle;                     // +0x2c
    CRGBColour                Colour;                    // +0x30
    unsigned short            Flags;                     // +0x34
    EEnginePrimitiveBlendMode BlendMode;                 // +0x38
    EEnginePrimitiveBlendOp   BlendOp;                   // +0x3c
    C2DVector                 Offset;                    // +0x40   sizeof == 0x48
    CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame, unsigned short flags,
                           float angle, float sizeX, float sizeY, const C2DVector& offset,
                           EEnginePrimitiveBlendMode blendMode, unsigned char alpha, unsigned char red,
                           unsigned char green, unsigned char blue, float fadeStart,
                           EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp);
};

CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame,
                                               unsigned short flags, float angle, float sizeX, float sizeY,
                                               const C2DVector& offset, EEnginePrimitiveBlendMode blendMode,
                                               unsigned char alpha, unsigned char red, unsigned char green,
                                               unsigned char blue, float fadeStart,
                                               EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp)
    : CEnginePrimitive(ENGINE_PRIMITIVE_SPRITE, interpolationSet)
    , WorldPos(worldPos)
    , BankIndex(bankIndex)
    , Frame(frame)
    , SizeX(sizeX)
    , SizeY(sizeY)
    , FadeStart(fadeStart)
    , Angle(angle)
    , Colour(red, green, blue, alpha)
    , Flags(flags)
    , BlendMode(blendMode)
    , BlendOp(blendOp)
    , Offset(offset)
{
}

int main()
{
    C3DVector* pos = (C3DVector*)malloc(sizeof(C3DVector)); pos->X = 1.0f; pos->Y = 2.0f; pos->Z = 3.0f;
    C2DVector* off = (C2DVector*)malloc(sizeof(C2DVector)); off->X = -4.0f; off->Y = 8.0f;
    CEnginePrimitiveSprite* s = (CEnginePrimitiveSprite*)malloc(sizeof(CEnginePrimitiveSprite));
    memset(s, 0xee, sizeof(CEnginePrimitiveSprite));
    new (s) CEnginePrimitiveSprite(*pos, 17, 3, 0x8001, 0.25f, 10.0f, 20.0f, *off, (EEnginePrimitiveBlendMode)2,
                                   0xA0, 0x11, 0x22, 0x33, 99.0f, (EEngineInterpolationSet)5, (EEnginePrimitiveBlendOp)6);
    int ok = s->Type == 7 && s->InterpolationSet == 5;
    ok &= s->ThemeChannelDesc.ChannelIndex0 == 0 && s->ThemeChannelDesc.ChannelIndex1 == 0 && s->ThemeChannelDesc.ChannelBlendFactor == 0;
    ok &= s->WorldPos.X == 1.0f && s->WorldPos.Y == 2.0f && s->WorldPos.Z == 3.0f && s->BankIndex == 17 && s->Frame == 3;
    ok &= s->SizeX == 10.0f && s->SizeY == 20.0f && s->FadeStart == 99.0f && s->Angle == 0.25f;
    ok &= s->Colour.A == 0xA0 && s->Colour.R == 0x11 && s->Colour.G == 0x22 && s->Colour.B == 0x33;
    unsigned char* c = (unsigned char*)&s->Colour;
    ok &= c[0] == 0x33 && c[1] == 0x22 && c[2] == 0x11 && c[3] == 0xA0;           // D3DCOLOR byte order
    ok &= s->Flags == 0x8001 && s->BlendMode == 2 && s->BlendOp == 6 && s->Offset.X == -4.0f && s->Offset.Y == 8.0f;
    if (ok) printf("007b74a0_TEST PASS\n");
    return ok ? 0 : 1;
}