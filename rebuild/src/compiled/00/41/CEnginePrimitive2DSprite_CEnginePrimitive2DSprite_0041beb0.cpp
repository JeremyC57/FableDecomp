// Names are from Ego_r.pdb CEnginePrimitive2DSprite and CEnginePrimitiveThemeChannelDesc,
// corroborated by landed006558CE. TextureFrame groups the adjacent BankIndex/Frame words;
// texture overloads clear them. The position overload represents ClipBox as two equal points.
// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite -- initializes sprite wrapper metadata, box, blend parameters, and UV vectors; retail 0x0041beb0 length 0xab. The shared internal sprite header is a distinct renderer object (0x8c); local wrapper fields follow constructor stores.
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
    unsigned long BankIndex, Frame;
    float StartAngle, EndAngle;
    unsigned long BlendMode;
    unsigned char Colour[4];
    unsigned char ClipBoxIsPos;
    unsigned long Flags;
    void* Texture;
    C2DVector UVOffsetsStart, UVOffsetsEnd;
    CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DBoxF* box, unsigned long layer,
        unsigned long bank, unsigned long frame, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd);
};
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DBoxF* box, unsigned long layer,
        unsigned long bank, unsigned long frame, unsigned long blend, unsigned long interpolation,
        float angleStart, float angleEnd, unsigned char c0, unsigned char c1,
        unsigned char c2, unsigned char c3, unsigned long param,
        const C2DVector* uvStart, const C2DVector* uvEnd) : Type(0x22), InterpolationSet(interpolation) {
    (void)unused;

    ClipBox.left=box->left; ClipBox.top=box->top; ClipBox.right=box->right; ClipBox.bottom=box->bottom;
    Layer=layer; BankIndex=bank; Frame=frame;
    StartAngle=angleStart; EndAngle=angleEnd; BlendMode=blend;

    Colour[2]=c1; Colour[1]=c2; Colour[0]=c3; Colour[3]=c0;
    ClipBoxIsPos=0; Flags=param; Texture=0;
    UVOffsetsStart=*uvStart; UVOffsetsEnd=*uvEnd;
}