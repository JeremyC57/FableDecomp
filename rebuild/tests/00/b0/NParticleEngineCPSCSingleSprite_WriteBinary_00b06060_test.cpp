// Behaviour test for CPSCSingleSprite::WriteBinary (0x00B06060): records every stream write and checks the order, the
// stream method used and the (bit-field) member values.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CCharString { void* PStringData; };
struct CRGBColour { union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
struct CEnginePrimitiveHandle { long Index; long Generation; };
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
static int g_baseCalls = 0;
struct SplineBase { void WriteBinary(Stream* out) { out->put('B', 0); ++g_baseCalls; } };
struct Model : SplineBase {
    long SpriteBankIndex;                               // +0x34
    CRGBColour StartColour;                             // +0x38
    CRGBColour MidColour;                               // +0x3c
    CRGBColour EndColour;                               // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;                              
    float EndRenderSize;                                
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;                              
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;                           
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;                              
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;                                       
    long TrailBlendMode;                                
    long TrailBlendOp;                                  
    long TrailBankIndex;                                
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;                                  
    bool UseEndColour;                                  
    bool SelfIlluminating : 1;                          // +0x93
    bool AlphaFadeEnable : 1;                           
    bool RotateAroundCentre : 1;                        
    bool FaceMe2D : 1;                                  
    bool FaceMe3D : 1;                                  
    bool CrossedSprites : 1;                            
    bool SizeFadeEnable : 1;                            
    bool ForceAnimationTime : 1;                        
    bool StayWithEmitter : 1;                           // +0x94
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
    void WriteBinary(Stream* out)
    {
        out->WriteLong(SpriteBankIndex);
        out->WriteColour(StartColour);
        out->WriteColour(MidColour);
        out->WriteColour(EndColour);
        out->WriteFloat(AnimationTimeSecs);
        out->WriteFloat(StartRenderSize);
        out->WriteFloat(EndRenderSize);
        out->WriteLong(SpriteAlignment);
        out->WriteLong(NoCrossedSprites);
        out->WriteFloat(InitialAngle);
        out->WriteLong(FadeInEndInteger);
        out->WriteLong(FadeOutBeginInteger);
        out->WriteFloat(AlphaFadeMinimum);
        out->WriteFloat(SizeFadeMinimum);
        out->WriteULong(PositionParam);
        out->WriteLong(BlendMode);
        out->WriteLong(BlendOp);
        out->WriteLong(TrailBlendMode);
        out->WriteLong(TrailBlendOp);
        out->WriteLong(TrailBankIndex);
        out->WriteFloat(TrailWidth);
        out->WriteLong(MaxTrailLength);
        out->WriteLong(StayWithEmitterIntFactor);
        out->WriteEBOOL(UseStartColour);
        out->WriteEBOOL(UseMidColour);
        out->WriteEBOOL(UseEndColour);
        out->WriteEBOOL(SelfIlluminating);
        out->WriteEBOOL(AlphaFadeEnable);
        out->WriteEBOOL(RotateAroundCentre);
        out->WriteEBOOL(FaceMe2D);
        out->WriteEBOOL(FaceMe3D);
        out->WriteEBOOL(CrossedSprites);
        out->WriteEBOOL(SizeFadeEnable);
        out->WriteEBOOL(ForceAnimationTime);
        out->WriteEBOOL(StayWithEmitter);
        out->WriteEBOOL(UsePosition);
        out->WriteEBOOL(UseSplinePoints);
        SplineBase::WriteBinary(out);
    }
};
int main()
{
    Model o;
    o.SpriteBankIndex = 101;
    o.StartColour.Colour = 14;
    o.MidColour.Colour = 21;
    o.EndColour.Colour = 28;
    o.AnimationTimeSecs = 5.5f;
    o.StartRenderSize = 6.5f;
    o.EndRenderSize = 7.5f;
    o.SpriteAlignment = 808;
    o.NoCrossedSprites = 909;
    o.InitialAngle = 10.5f;
    o.FadeInEndInteger = 1111;
    o.FadeOutBeginInteger = 1212;
    o.AlphaFadeMinimum = 13.5f;
    o.SizeFadeMinimum = 14.5f;
    o.PositionParam = 1515;
    o.BlendMode = 1616;
    o.BlendOp = 1717;
    o.TrailBlendMode = 1818;
    o.TrailBlendOp = 1919;
    o.TrailBankIndex = 2020;
    o.TrailWidth = 21.5f;
    o.MaxTrailLength = 2222;
    o.StayWithEmitterIntFactor = 2323;
    o.UseStartColour = false;
    o.UseMidColour = true;
    o.UseEndColour = false;
    o.SelfIlluminating = 1;
    o.AlphaFadeEnable = 0;
    o.RotateAroundCentre = 1;
    o.FaceMe2D = 0;
    o.FaceMe3D = 1;
    o.CrossedSprites = 0;
    o.SizeFadeEnable = 1;
    o.ForceAnimationTime = 0;
    o.StayWithEmitter = 1;
    o.UsePosition = false;
    o.UseSplinePoints = true;
    o.EnteringTimelineEnd = false;
    Stream s;
    o.WriteBinary(&s);
    static const double want[37] = { 101.0, 14.0, 21.0, 28.0, 5.5, 6.5, 7.5, 808.0, 909.0, 10.5, 1111.0, 1212.0, 13.5, 14.5, 1515.0, 1616.0, 1717.0, 1818.0, 1919.0, 2020.0, 21.5, 2222.0, 2323.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 };
    static const char kinds[] = "lcccfffllfllffulllllfllbbbbbbbbbbbbbb";
    bool ok = s.n == 38 && s.kind[37] == 'B' && g_baseCalls == 1;
    for (int i = 0; ok && i < 37; ++i) ok = s.kind[i] == kinds[i] && s.val[i] == want[i];
    if (!ok) { std::printf("00b06060_TEST FAIL\n"); return 1; }
    std::printf("00b06060_TEST PASS\n");
    return 0;
}