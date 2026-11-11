#ifndef FABLE_ENGINE_CENGINEPRIMITIVESPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVESPRITE_H
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/C3DVector.h"
#ifdef _MSC_VER
enum EEnginePrimitiveBlendOp;
#else
enum EEnginePrimitiveBlendOp{};
#endif
// Layout: complete original Egod697,size72. Original ctor FW01d8e9a0.
class CEnginePrimitiveSprite:public CEnginePrimitive {
public: C3DVector WorldPos;
public: unsigned long BankIndex;
public: unsigned long Frame;
public: float SizeX;
public: float SizeY;
public: float FadeStart;
public: float Angle;
public: CRGBColour Colour;
public: unsigned short Flags;
public: EEnginePrimitiveBlendMode BlendMode;
public: EEnginePrimitiveBlendOp BlendOp;
public: C2DVector Offset;
public: CEnginePrimitiveSprite(const CEnginePrimitiveSprite & p0);
public: CEnginePrimitiveSprite(const C3DVector & p0,unsigned long p1,unsigned long p2,unsigned short p3,float p4,float p5,float p6,const C2DVector & p7,EEnginePrimitiveBlendMode p8,unsigned char p9,unsigned char p10,unsigned char p11,unsigned char p12,float p13,EEngineInterpolationSet p14,EEnginePrimitiveBlendOp p15);
public: CEnginePrimitiveSprite(const C3DVector & p0,unsigned long p1,unsigned long p2,unsigned short p3,float p4,float p5,float p6,EEnginePrimitiveBlendMode p7,unsigned char p8,unsigned char p9,unsigned char p10,unsigned char p11,float p12,EEngineInterpolationSet p13,EEnginePrimitiveBlendOp p14);
public: CEnginePrimitiveSprite();
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveSprite)==72);
#endif
