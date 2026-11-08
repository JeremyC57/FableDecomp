// CEngineInternalPrimitive2DClockSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a 2D clock sprite descriptor and size its clip box from the bank frames
// Leaf: ?Update@CEngineInternalPrimitive2DClockSprite@@UAEXABVCEnginePrimitive@@K@Z
// Flags:
// Shape: Colour2 is a plain member copy and the interpolation set binds the by-value PDB getter
//   (?GetInterpolationSet@CEnginePrimitive@@QBE?AW4EEngineInterpolationSet@@XZ) to a const reference; the bound
//   temporary restarts the eax/edx copy alternation so the set and the filter bit both load into eax as retail.
#include "fable_compiler.h"
#include "rebuild_abi.h"
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (ESTABLISHED_SPELLINGS; naming wave 20) 1e-4f, the tree spelling (naming wave 13). ChatGPT reads the Ego_r public here as ?DELTA_NOTIONAL_ZERO@@3MB: confirm with a PDB data dump
class C2DVector{public:float X,Y;float GetSquaredMagnitude()const{return X*X+Y*Y;}bool IsNonZero()const{return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}};
class C2DBoxF{public:float TLX,TLY,BRX,BRY;};
// Layout: original four-byte colour union is projected through its IntValue arm.
class CRGBColour{public:unsigned long IntValue;};
enum EEnginePrimitiveType{};enum EEngineInterpolationSet{};enum EEngine2DLayer{};enum EEnginePrimitiveBlendMode{};enum EEnginePrimitiveStatIndex{};
namespace std{template<class A,class B>struct pair;template<class T>struct less;template<class T>class allocator{};template<class K,class V,class C,class A>class map;template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int n)const{return this->_M_start[n];}};}
template<class T>class CArray:public std::vector<T>{public:const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
class CEnginePrimitive{protected:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;unsigned char ThemeChannelDesc[4];public:EEngineInterpolationSet GetInterpolationSet()const{return InterpolationSet;}};
// Layout: original offsets4d/55 are unaligned C2DVector objects. Byte storage
// preserves them without adding packing; typed references below recover the objects.
class CEnginePrimitive2DClockSprite:public CEnginePrimitive{public:C2DBoxF ClipBox;EEngine2DLayer Layer;unsigned long BankIndex,Frame,BankIndex2,Frame2;float AngleStart,AngleEnd;EEnginePrimitiveBlendMode BlendMode;CRGBColour Colour,Colour2;bool ClipBoxIsPos;unsigned long Flags;bool Filter;unsigned char UVOffsetsStart[8],UVOffsetsEnd[8];};
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBaseFields
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#define CEngineInternalPrimitive2DClockSprite CEngineInternalPrimitive2DClockSpriteFields
#include "engine/CEngineInternalPrimitive2DClockSprite.h"
#undef CEngineInternalPrimitive2DClockSprite
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase{private:long Type;public:virtual~CEnginePersistentPrimitiveBase();virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const=0;protected:virtual void NotifyReleased(const CEnginePrimitiveHandle&);virtual bool WasRendered()const;};
// Layout: shared base48 restored to original virtual prefix through Update(slot5).
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase{public:unsigned long EnginePrimitiveFlags;unsigned char unobservedFadeGridState[0x1c];EEngineInterpolationSet InterpolationSet;unsigned char unobservedRenderAndListState[0x18];virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const;virtual void Update(const CEnginePrimitive&,unsigned long);};
class CEngineInternalPrimitive2DClockSprite:public CEngineInternalPrimitiveBase{public:C2DBoxF ClipBox;EEngine2DLayer Layer;unsigned long BankIndex,Frame;CRGBColour Colour;unsigned long BankIndex2,Frame2;CRGBColour Colour2;float AngleStart,AngleEnd;bool Filter;unsigned char UVOffsetsStart[8],UVOffsetsEnd[8];bool WrapTexture;EEnginePrimitiveBlendMode BlendMode;virtual void Update(const CEnginePrimitive&,unsigned long);};
class CGraphicInfo;class CGraphicInfoFrame;
class CGraphicDataBank{public:class CGraphicHeader{public:unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;unsigned char PixelFormat[4];unsigned char TransparencyType,MipmapLevels;unsigned char IsFlatAnimatedGraphic:1;unsigned long FrameDataSize;unsigned long CompressedMipmapSizes[1];};enum ETextureType{};
private:unsigned char unobservedBankBases[0x1e0];CArray<CGraphicInfo>GraphicList;
public:const CGraphicInfo*PeekGraphic(unsigned long)const;const CGraphicInfoFrame*PeekGraphicInfoFrame(unsigned long,unsigned long)const;};
class CBaseClass{public:virtual~CBaseClass();};
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;};
class CGraphicInfoFrame:public CBaseClass{public:class CFrameData;private:unsigned short FrameNum;signed char SmallMipmapLevel;CIVCountedPointer<CFrameData>Data;CGraphicInfo*GraphicInfo;public:void BeginLoading(unsigned long,unsigned long)const;unsigned long GetWidth()const;unsigned long GetHeight()const;};
class CGraphicInfo{private:CGraphicDataBank::CGraphicHeader Header;CGraphicDataBank::ETextureType TextureType;CGraphicDataBank*Bank;long Index;CGraphicInfoFrame*Frames;public:const CGraphicInfoFrame*PeekInfoFrame(unsigned long n)const{return Frames+n;}unsigned long GetWidth()const{return Header.FrameWidth;}unsigned long GetHeight()const{return Header.FrameHeight;}};
FABLE_FORCEINLINE const CGraphicInfo*CGraphicDataBank::PeekGraphic(unsigned long n)const{return n<GraphicList.size()?&GraphicList[n]:0;}
FABLE_FORCEINLINE const CGraphicInfoFrame*CGraphicDataBank::PeekGraphicInfoFrame(unsigned long n,unsigned long frame)const{return PeekGraphic(n)?PeekGraphic(n)->PeekInfoFrame(frame):0;}
FABLE_FORCEINLINE unsigned long CGraphicInfoFrame::GetWidth()const{return GraphicInfo->GetWidth();}FABLE_FORCEINLINE unsigned long CGraphicInfoFrame::GetHeight()const{return GraphicInfo->GetHeight();}
#define CEngineResourceManager CEngineResourceManagerFields
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager
enum EGraphicBankNo{};
class CEngineResourceManager:public CEngineResourceManagerFields{public:const CGraphicDataBank*PeekGraphicBank(EGraphicBankNo n)const{return GraphicBanks[n];}};
namespace NEngineInternal{extern CEngineResourceManager*GPResourceManager;}
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase)==0x48);
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitive2DClockSprite)==0x94);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DClockSprite)==0x60);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==0x2c);
void CEngineInternalPrimitive2DClockSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    const CEnginePrimitive2DClockSprite& sprite = static_cast<const CEnginePrimitive2DClockSprite&>(primitive);
    ClipBox = sprite.ClipBox;
    Layer = sprite.Layer;
    BankIndex = sprite.BankIndex;
    Frame = sprite.Frame;
    BankIndex2 = sprite.BankIndex2;
    Frame2 = sprite.Frame2;
    AngleStart = sprite.AngleStart;
    AngleEnd = sprite.AngleEnd;
    BlendMode = sprite.BlendMode;
    Colour = sprite.Colour;
    Colour2 = sprite.Colour2;
    const EEngineInterpolationSet& interpolationSet = primitive.GetInterpolationSet();
    InterpolationSet = interpolationSet;
    Filter = (sprite.Flags >> 1) & 1;
    reinterpret_cast<C2DVector&>(UVOffsetsStart) = reinterpret_cast<const C2DVector&>(sprite.UVOffsetsStart);
    reinterpret_cast<C2DVector&>(UVOffsetsEnd) = reinterpret_cast<const C2DVector&>(sprite.UVOffsetsEnd);
    WrapTexture = reinterpret_cast<const C2DVector&>(UVOffsetsStart).IsNonZero() || reinterpret_cast<const C2DVector&>(UVOffsetsEnd).IsNonZero();
    if (flags & 0x80)
        EnginePrimitiveFlags |= 0x80;
    if (BankIndex || BankIndex2) {
        unsigned long width = 0;
        unsigned long height = 0;
        if (BankIndex) {
            const CGraphicInfoFrame* frame = NEngineInternal::GPResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->PeekGraphicInfoFrame(BankIndex, Frame);
            frame->BeginLoading(0, 0);
            height = frame->GetHeight();
            width = frame->GetWidth();
        }
        if (BankIndex2) {
            const CGraphicInfoFrame* frame = NEngineInternal::GPResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->PeekGraphicInfoFrame(BankIndex2, Frame2);
            frame->BeginLoading(0, 0);
            if (frame->GetWidth() > width)
                width = frame->GetWidth();
            if (frame->GetHeight() > height)
                height = frame->GetHeight();
        }
        if (sprite.ClipBoxIsPos) {
            if (sprite.Flags & 1) {
                unsigned long halfWidth = width >> 1;
                unsigned long halfHeight = height >> 1;
                ClipBox.TLX -= halfWidth;
                ClipBox.TLY -= halfHeight;
                ClipBox.BRX += halfWidth;
                ClipBox.BRY += halfHeight;
            } else {
                ClipBox.BRX += width;
                ClipBox.BRY += height;
            }
        }
    }
}