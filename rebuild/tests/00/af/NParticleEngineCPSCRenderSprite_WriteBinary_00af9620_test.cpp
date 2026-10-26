// Behaviour test for CPSCRenderSprite::WriteBinary (0x00AF9620): records every stream write and checks the order, the
// stream method used and the (bit-field) member values.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
class CRGBColour { public: union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
class CEnginePrimitiveHandle { public: long Index; long Generation; };
struct Stream {
    char kind[96]; double val[96]; int n;
    Stream() : n(0) {}
    void put(char k, double v) { kind[n] = k; val[n] = v; n++; }
    void WriteEBOOL(bool v) { put('b', v ? 1 : 0); }
    void WriteLong(long v) { put('l', (double)v); }
    void WriteULong(unsigned long v) { put('u', (double)v); }
    void WriteFloat(float v) { put('f', v); }
    void WriteColour(const CRGBColour& c) { put('c', (double)c.Colour); }
    void Write3DVector(const C3DVector& v) { put('v', v.X + v.Y + v.Z); }
    void WriteNullTerminatedString(const CCharString& s) { put('s', (double)(unsigned long)s.PStringData); }
};
struct Model {
    long SpriteBankIndex;                               // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour StartColour;                             // +0x14
    CRGBColour MidColour;                               // +0x18
    CRGBColour EndColour;                               // +0x1c
    unsigned long BlendMode : 3;                        // +0x20
    unsigned long TrailBlendMode : 3;                   
    unsigned long BlendOp : 3;                          
    unsigned long TrailBlendOp : 3;                     
    unsigned long Flags : 10;                           
    unsigned long FadeInEndInteger : 10;                
    unsigned long FadeOutBeginInteger : 10;             // +0x24
    unsigned long TrailLengthInteger : 12;              
    unsigned long FlickerMinAlphaInteger : 10;          
    unsigned long FlickerMinSizeInteger : 10;           // +0x28
    unsigned long NoCrossedSprites : 5;                 
    unsigned long StartRenderSize : 11;                 
    unsigned long AlphaFadeMinimum : 7;                 // +0x2c
    unsigned long EndRenderSize : 11;                   
    unsigned long FlickerBias : 8;                      
    unsigned long AnimationTimeSecs : 14;               // +0x30
    unsigned long SizeFadeMinimum : 7;                  
    unsigned long TrailWidth : 10;                      
    unsigned long FlickerSpeed : 12;                    // +0x34
    unsigned long UseStartColour : 1;                   
    unsigned long UseMidColour : 1;                     
    unsigned long UseEndColour : 1;                     
    unsigned char AlphaFadeEnable : 1;                  // +0x38
    unsigned char SizeFadeEnable : 1;                   
    unsigned char FlickerEnable : 1;                    
    unsigned char ForceAnimationTime : 1;               
    CEnginePrimitiveHandle PrimHandle;                  // +0x3c
    long ChildPrimIndex;                                // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;             // +0x48
    long ChildTrailPrimIndex;                           // +0x50
    void WriteBinary(Stream* out)
    {
        out->WriteLong(SpriteBankIndex);
        out->WriteLong(TrailBankIndex);
        out->WriteColour(StartColour);
        out->WriteColour(MidColour);
        out->WriteColour(EndColour);
        out->WriteULong(BlendMode);
        out->WriteULong(TrailBlendMode);
        out->WriteULong(BlendOp);
        out->WriteULong(TrailBlendOp);
        out->WriteULong(Flags);
        out->WriteULong(FadeInEndInteger);
        out->WriteULong(FadeOutBeginInteger);
        out->WriteULong(TrailLengthInteger);
        out->WriteULong(FlickerMinAlphaInteger);
        out->WriteULong(FlickerMinSizeInteger);
        out->WriteULong(NoCrossedSprites);
        out->WriteULong(StartRenderSize);
        out->WriteULong(AlphaFadeMinimum);
        out->WriteULong(EndRenderSize);
        out->WriteULong(FlickerBias);
        out->WriteULong(AnimationTimeSecs);
        out->WriteULong(SizeFadeMinimum);
        out->WriteULong(TrailWidth);
        out->WriteULong(FlickerSpeed);
        out->WriteEBOOL(UseStartColour);
        out->WriteEBOOL(UseMidColour);
        out->WriteEBOOL(UseEndColour);
        out->WriteEBOOL(AlphaFadeEnable);
        out->WriteEBOOL(SizeFadeEnable);
        out->WriteEBOOL(FlickerEnable);
        out->WriteEBOOL(ForceAnimationTime);
    }
};
int main()
{
    Model o;
    o.SpriteBankIndex = 101;
    o.TrailBankIndex = 202;
    o.StartColour.Colour = 21;
    o.MidColour.Colour = 28;
    o.EndColour.Colour = 35;
    o.BlendMode = 3;
    o.TrailBlendMode = 0;
    o.BlendOp = 5;
    o.TrailBlendOp = 2;
    o.Flags = 375;
    o.FadeInEndInteger = 412;
    o.FadeOutBeginInteger = 449;
    o.TrailLengthInteger = 486;
    o.FlickerMinAlphaInteger = 523;
    o.FlickerMinSizeInteger = 560;
    o.NoCrossedSprites = 21;
    o.StartRenderSize = 634;
    o.AlphaFadeMinimum = 31;
    o.EndRenderSize = 708;
    o.FlickerBias = 233;
    o.AnimationTimeSecs = 782;
    o.SizeFadeMinimum = 51;
    o.TrailWidth = 856;
    o.FlickerSpeed = 893;
    o.UseStartColour = 1;
    o.UseMidColour = 0;
    o.UseEndColour = 1;
    o.AlphaFadeEnable = 0;
    o.SizeFadeEnable = 1;
    o.FlickerEnable = 0;
    o.ForceAnimationTime = 1;
    o.ChildPrimIndex = 3333;
    o.ChildTrailPrimIndex = 3535;
    Stream s;
    o.WriteBinary(&s);
    static const double want[31] = { 101.0, 202.0, 21.0, 28.0, 35.0, 3.0, 0.0, 5.0, 2.0, 375.0, 412.0, 449.0, 486.0, 523.0, 560.0, 21.0, 634.0, 31.0, 708.0, 233.0, 782.0, 51.0, 856.0, 893.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 };
    static const char kinds[] = "llcccuuuuuuuuuuuuuuuuuuubbbbbbb";
    bool ok = s.n == 31;
    for (int i = 0; ok && i < 31; ++i) ok = s.kind[i] == kinds[i] && s.val[i] == want[i];
    if (!ok) { std::printf("00af9620_TEST FAIL\n"); return 1; }
    std::printf("00af9620_TEST PASS\n");
    return 0;
}