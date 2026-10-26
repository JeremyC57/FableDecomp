// Names are from Ego_r.pdb CEnginePrimitive2DSprite and CEnginePrimitiveThemeChannelDesc,
// corroborated by landed006558CE. TextureFrame groups the adjacent BankIndex/Frame words;
// texture overloads clear them. The position overload represents ClipBox as two equal points.
// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite -- initializes sprite wrapper metadata, box, blend parameters, and UV vectors; retail 0x0043fa20 length 0xa7. The shared internal sprite header is a distinct renderer object (0x8c); local wrapper fields follow constructor stores.
class C2DBoxF { public: float left, top, right, bottom; };
class C2DVector { public: float x, y; };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad; CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) {} };
struct CSpriteTextureFrame { unsigned long BankIndex, Frame; void Reset() { BankIndex=0; Frame=0; } };
struct CEnginePrimitive2DSprite {
    unsigned long Type;
    unsigned long InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    C2DVector ClipBoxStart, ClipBoxEnd;
    unsigned long Layer;
    CSpriteTextureFrame TextureFrame;
    float StartAngle, EndAngle;
    unsigned long BlendMode;
    unsigned char Colour[4];
    unsigned char ClipBoxIsPos;
    unsigned long Flags;
    void* Texture;
    C2DVector UVOffsetsStart, UVOffsetsEnd;
    CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DVector* basis, unsigned long layer,
        void* texture, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd);
};
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DVector* basis, unsigned long layer,
        void* texture, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd) : Type(0x22), InterpolationSet(interpolation) {
    (void)unused;

    ClipBoxStart=*basis; ClipBoxEnd=*basis;
    Layer=layer; TextureFrame.Reset();
    StartAngle=angleStart; EndAngle=angleEnd; BlendMode=blend;
    Colour[2]=c1; Colour[1]=c2; Colour[0]=c3; Colour[3]=c0;
    ClipBoxIsPos=1; Flags=param; Texture=texture;
    UVOffsetsStart=*uvStart; UVOffsetsEnd=*uvEnd;
}