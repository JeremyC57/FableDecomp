#ifndef _MSC_VER
#define __int64 long long
#define __forceinline inline
#endif
// CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite -- initializes 2D geometry, two bank/frame colors, angles, blend state and unaligned UVs; retail 0065595c 194 bytes. Callee 004371a0 copies two C2DVectors into C2DBoxF; its propagated CRightHandedSet label describes a different layout. No wrapper, C2DBoxF, or CRGBColour header exists; internal renderer header is a distinct 0x94 object.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public: float X,Y; };
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF(const C2DVector*,const C2DVector*); }; // 0x004371a0
class CRGBColour { public:
 unsigned char B,G,R,A;
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Set(r,g,b,a); }
 __forceinline void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
 __forceinline CRGBColour(const CRGBColour& o) { Set(o.R,o.G,o.B,o.A); }
};
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){} };
class CEnginePrimitive { public:
 long Type,InterpolationSet;
 CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
 CEnginePrimitive(long type,long interpolation):Type(type),InterpolationSet(interpolation){}
};
struct CEnginePrimitive2DClockSprite : CEnginePrimitive {
 C2DBoxF ClipBox;
 unsigned long Layer,BankIndex,Frame,BankIndex2,Frame2;
 float AngleStart,AngleEnd;
 unsigned long BlendMode;
 CRGBColour Colour,Colour2;
 unsigned char ClipBoxIsPos;
 unsigned long Flags;
 unsigned char Filter;
 unsigned char UVOffsetsStart[8],UVOffsetsEnd[8];
 CEnginePrimitive2DClockSprite(unsigned __int64 unused,const C2DVector* basis,unsigned long layer,unsigned long bank,unsigned long frame,const CRGBColour* colour,unsigned long bank2,unsigned long frame2,const CRGBColour* colour2,unsigned long blend,float start,float end,const C2DVector* uvStart,const C2DVector* uvEnd,unsigned long enabled,unsigned long interpolation);
};
CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite(unsigned __int64 unused,const C2DVector* basis,unsigned long layer,unsigned long bank,unsigned long frame,const CRGBColour* colour,unsigned long bank2,unsigned long frame2,const CRGBColour* colour2,unsigned long blend,float start,float end,const C2DVector* uvStart,const C2DVector* uvEnd,unsigned long enabled,unsigned long interpolation)
 : CEnginePrimitive(0x23,interpolation),ClipBox(basis,basis),Layer(layer),BankIndex(bank),Frame(frame),BankIndex2(bank2),Frame2(frame2),AngleStart(start),AngleEnd(end),BlendMode(blend),Colour(*colour),Colour2(*colour2),ClipBoxIsPos(1),Flags(enabled),Filter(0)
{
 (void)unused;
 *reinterpret_cast<C2DVector*>(UVOffsetsStart)=*uvStart;
 *reinterpret_cast<C2DVector*>(UVOffsetsEnd)=*uvEnd;
}

#include <cstdio>
#include <cstring>
#include <cstddef>
static int boxCalls=0;
static const C2DVector *lastA=0,*lastB=0;
C2DBoxF::C2DBoxF(const C2DVector* a,const C2DVector* b) : TLX(a->X),TLY(a->Y),BRX(b->X),BRY(b->Y) { ++boxCalls;lastA=a;lastB=b; }
static int check(bool ok) { return ok?0:1; }
int main()
{
 int failures=0;
 C2DVector basis={-7.5f,15.75f},uv0={0.25f,-1.0f},uv1={2.0f,9.5f};
 for(unsigned int i=0;i<2;++i) {
  CRGBColour colour(i?0x12:0,i?0x34:0,i?0x56:0,i?0x78:0), colour2(i?0x9a:0,i?0xbc:0,i?0xde:0,i?0xf0:0);
  unsigned long layer=i?0x10:0,bank=i?0x20:0,frame=i?0x30:0,bank2=i?0x40:0,frame2=i?0x50:0,blend=i?0x60:0,enabled=i?0xfedcba98UL:0,interpolation=i?0x76543210UL:0;
  float start=i?-2.25f:0,end=i?8.75f:0;
  CEnginePrimitive2DClockSprite sprite(~static_cast<unsigned __int64>(0),&basis,layer,bank,frame,&colour,bank2,frame2,&colour2,blend,start,end,&uv0,&uv1,enabled,interpolation);
  failures+=check(boxCalls==int(i+1) && lastA==&basis && lastB==&basis);
  failures+=check(sprite.Type==0x23 && sprite.InterpolationSet==interpolation);
  failures+=check(sprite.ThemeChannelDesc.ChannelIndex0==0 && sprite.ThemeChannelDesc.ChannelIndex1==0 && sprite.ThemeChannelDesc.ChannelBlendFactor==0);
  failures+=check(sprite.ClipBox.TLX==basis.X && sprite.ClipBox.TLY==basis.Y && sprite.ClipBox.BRX==basis.X && sprite.ClipBox.BRY==basis.Y);
  failures+=check(sprite.Layer==layer && sprite.BankIndex==bank && sprite.Frame==frame && sprite.BankIndex2==bank2 && sprite.Frame2==frame2);
  failures+=check(sprite.AngleStart==start && sprite.AngleEnd==end && sprite.BlendMode==blend && sprite.Flags==enabled && sprite.ClipBoxIsPos==1 && sprite.Filter==0);
  failures+=check(sprite.Colour.R==colour.R && sprite.Colour.G==colour.G && sprite.Colour.B==colour.B && sprite.Colour.A==colour.A);
  failures+=check(sprite.Colour2.R==colour2.R && sprite.Colour2.G==colour2.G && sprite.Colour2.B==colour2.B && sprite.Colour2.A==colour2.A);
  C2DVector got0,got1;std::memcpy(&got0,sprite.UVOffsetsStart,8);std::memcpy(&got1,sprite.UVOffsetsEnd,8);
  failures+=check(got0.X==uv0.X && got0.Y==uv0.Y && got1.X==uv1.X && got1.Y==uv1.Y);
  colour.R=0xff; uv0.X+=2.0f;
  failures+=check(sprite.Colour.R!=(unsigned char)0xff && got0.X!=uv0.X);
 }
#ifdef _MSC_VER
 failures+=check(offsetof(CEnginePrimitive2DClockSprite,ClipBox)==0xc && offsetof(CEnginePrimitive2DClockSprite,Colour)==0x3c && offsetof(CEnginePrimitive2DClockSprite,UVOffsetsStart)==0x4d && offsetof(CEnginePrimitive2DClockSprite,UVOffsetsEnd)==0x55);
#endif
 if(failures) { std::printf("0065595c_TEST FAIL %d\n",failures); return 1; }
 std::printf("0065595c_TEST PASS\n");return 0;
}