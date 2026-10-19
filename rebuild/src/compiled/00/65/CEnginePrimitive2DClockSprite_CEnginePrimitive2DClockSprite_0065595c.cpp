// CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite -- initializes 2D geometry, two bank/frame colors, angles, blend state and unaligned UVs; retail 0065595c 194 bytes. Callee 004371a0 copies two C2DVectors into C2DBoxF; its propagated CRightHandedSet label describes a different layout. No wrapper, C2DBoxF, or CRGBColour header exists; internal renderer header is a distinct 0x94 object.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X,Y; };
struct C2DBoxF { float TLX,TLY,BRX,BRY; C2DBoxF(const C2DVector*,const C2DVector*); }; // 0x004371a0
struct CRGBColour {
 unsigned char B,G,R,A;
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Set(r,g,b,a); }
 __forceinline void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
 __forceinline CRGBColour(const CRGBColour& o) { Set(o.R,o.G,o.B,o.A); }
};
struct CEnginePrimitiveThemeChannelDesc { unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){} };
struct CEnginePrimitive {
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