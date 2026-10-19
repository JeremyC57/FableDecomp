#ifndef _MSC_VER
#define __int64 long long
#endif
// Names are from Ego_r.pdb CEnginePrimitive2DSprite and CEnginePrimitiveThemeChannelDesc,
// corroborated by landed006558CE. TextureFrame groups the adjacent BankIndex/Frame words;
// texture overloads clear them. The position overload represents ClipBox as two equal points.
// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite -- initializes sprite wrapper metadata, box, blend parameters, and UV vectors; retail 0x0041bf60 length 0xa7. The shared internal sprite header is a distinct renderer object (0x8c); local wrapper fields follow constructor stores.
struct C2DBoxF { float left, top, right, bottom; };
struct C2DVector { float x, y; };
struct CEnginePrimitiveThemeChannelDesc { unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad; CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) {} };
struct CSpriteTextureFrame { unsigned long BankIndex, Frame; void Reset() { BankIndex=0; Frame=0; } };
struct CEnginePrimitive2DSprite {
    unsigned long Type;
    unsigned long InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    C2DBoxF ClipBox;
    unsigned long Layer;
    CSpriteTextureFrame TextureFrame;
    float StartAngle, EndAngle;
    unsigned long BlendMode;
    unsigned char Colour[4];
    unsigned char ClipBoxIsPos;
    unsigned long Flags;
    void* Texture;
    C2DVector UVOffsetsStart, UVOffsetsEnd;
    CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DBoxF* box, unsigned long layer,
        void* texture, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd);
};
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DBoxF* box, unsigned long layer,
        void* texture, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd) : Type(0x22), InterpolationSet(interpolation) {
    (void)unused;

    ClipBox.left=box->left; ClipBox.top=box->top; ClipBox.right=box->right; ClipBox.bottom=box->bottom;
    Layer=layer;
    StartAngle=angleStart; EndAngle=angleEnd; BlendMode=blend;

    TextureFrame.Reset();
    Colour[2]=c1; Colour[1]=c2; Colour[0]=c3; Colour[3]=c0;
    ClipBoxIsPos=0; Flags=param; Texture=texture;
    UVOffsetsStart=*uvStart; UVOffsetsEnd=*uvEnd;
}

#include <cstdio>
#include <cstddef>
static int check(bool ok) { return ok ? 0 : 1; }
int main()
{
    int failures=0;
    C2DVector uv0={-0.25f,1.5f}, uv1={3.75f,-4.5f};
    C2DBoxF box={-2.0f,0.5f,7.0f,9.25f};
    C2DVector basis={-2.0f,7.0f};
    for (unsigned int i=0; i<2; ++i) {
        const unsigned long layer=i?0xffffffffUL:0;
        const unsigned long interpolation=i?0x76543210UL:0;
        const unsigned long blend=i?0x12345678UL:0;
        const unsigned long param=i?0x89abcdefUL:0;
        void* texture=i?static_cast<void*>(&box):0;
        const float a=i?-3.5f:0.0f, b=i?9.75f:0.0f;
        unsigned char c0=i?0xa1:0, c1=i?0xb2:0, c2=i?0xc3:0, c3=i?0xd4:0;
        CEnginePrimitive2DSprite sprite(~static_cast<unsigned __int64>(0), &box, layer,
            texture, blend, interpolation, a,b,c0,c1,c2,c3,param,&uv0,&uv1);
        failures+=check(sprite.Type==0x22 && sprite.InterpolationSet==interpolation);
        failures+=check(sprite.ThemeChannelDesc.ChannelIndex0==0 && sprite.ThemeChannelDesc.ChannelIndex1==0 && sprite.ThemeChannelDesc.ChannelBlendFactor==0);
        failures+=check(sprite.ClipBox.left==box.left && sprite.ClipBox.top==box.top && sprite.ClipBox.right==box.right && sprite.ClipBox.bottom==box.bottom);
        failures+=check(sprite.Layer==layer && sprite.TextureFrame.BankIndex==0 && sprite.TextureFrame.Frame==0);
        failures+=check(sprite.StartAngle==a && sprite.EndAngle==b && sprite.BlendMode==blend);
        failures+=check(sprite.Colour[0]==c3 && sprite.Colour[1]==c2 && sprite.Colour[2]==c1 && sprite.Colour[3]==c0);
        failures+=check(sprite.ClipBoxIsPos==0 && sprite.Flags==param && sprite.Texture==texture);
        failures+=check(sprite.UVOffsetsStart.x==uv0.x && sprite.UVOffsetsStart.y==uv0.y && sprite.UVOffsetsEnd.x==uv1.x && sprite.UVOffsetsEnd.y==uv1.y);
        const float oldX=uv0.x; uv0.x+=1.0f;
        failures+=check(sprite.UVOffsetsStart.x==oldX);
    }
#ifdef _MSC_VER
    failures+=check(sizeof(CEnginePrimitive2DSprite)==0x54 && offsetof(CEnginePrimitive2DSprite,Colour)==0x34 && offsetof(CEnginePrimitive2DSprite,UVOffsetsStart)==0x44);
#endif
    if(failures) { std::printf("0041bf60_TEST FAIL %d\n", failures); return 1; }
    std::printf("0041bf60_TEST PASS\n"); return 0;
}