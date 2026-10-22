// Behaviour test for CPSCRenderMesh::WriteBinary (0x00AFEE00): records every stream write and checks the order, the
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
struct Model {
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;                   
    unsigned long BlendOp : 3;                          
    unsigned long TrailBlendOp : 3;                     
    unsigned long FadeInEndInteger : 10;                
    unsigned long FadeOutBeginInteger : 10;             
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;          
    unsigned long FlickerMinSizeInteger : 10;           
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;                
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;                     
    unsigned long AlphaFadeEnable : 1;                  
    unsigned long EndRenderSizeX : 11;                  
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;                  
    unsigned long SizeFadeEnable : 1;                   
    unsigned long FlickerEnable : 1;                    
    unsigned long TrailUseStartColour : 1;              
    unsigned long TrailUseMidColour : 1;                
    unsigned long TrailUseEndColour : 1;                
    unsigned long UseRenderSizeParam : 1;               
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;                    
    unsigned long AlphaFadeMinimum : 7;                 
    unsigned long UseStartColour : 1;                   
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;                     
    unsigned long TrailWidth : 10;                      
    unsigned long UseEndColour : 1;                     
    unsigned long RenderSizeParam;                      // +0x48
    CEnginePrimitiveHandle PrimHandle;                  // +0x4c
    long ChildPrimIndex;                                // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;             // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    void WriteBinary(Stream* out)
    {
        out->WriteLong(BankIndex);
        out->WriteLong(TrailBankIndex);
        out->WriteColour(TrailStartColour);
        out->WriteColour(TrailMidColour);
        out->WriteColour(TrailEndColour);
        out->WriteColour(StartColour);
        out->WriteColour(MidColour);
        out->WriteColour(EndColour);
        out->WriteULong(BlendMode);
        out->WriteULong(TrailBlendMode);
        out->WriteULong(BlendOp);
        out->WriteULong(TrailBlendOp);
        out->WriteULong(FadeInEndInteger);
        out->WriteULong(FadeOutBeginInteger);
        out->WriteULong(TrailLengthInteger);
        out->WriteULong(FlickerMinAlphaInteger);
        out->WriteULong(FlickerMinSizeInteger);
        out->WriteULong(StartRenderSizeX);
        out->WriteULong(StartRenderSizeY);
        out->WriteULong(StartRenderSizeZ);
        out->WriteEBOOL(CentredOnPos);
        out->WriteEBOOL(AlphaFadeEnable);
        out->WriteULong(EndRenderSizeX);
        out->WriteULong(EndRenderSizeY);
        out->WriteULong(EndRenderSizeZ);
        out->WriteEBOOL(SizeFadeEnable);
        out->WriteEBOOL(FlickerEnable);
        out->WriteEBOOL(TrailUseStartColour);
        out->WriteEBOOL(TrailUseMidColour);
        out->WriteEBOOL(TrailUseEndColour);
        out->WriteEBOOL(UseRenderSizeParam);
        out->WriteULong(FlickerBias);
        out->WriteULong(FlickerSpeed);
        out->WriteULong(AlphaFadeMinimum);
        out->WriteEBOOL(UseStartColour);
        out->WriteULong(SizeFadeMinimum);
        out->WriteEBOOL(UseMidColour);
        out->WriteULong(TrailWidth);
        out->WriteEBOOL(UseEndColour);
        out->WriteULong(RenderSizeParam);
    }
};
int main()
{
    Model o;
    o.BankIndex = 101;
    o.TrailBankIndex = 202;
    o.TrailStartColour.Colour = 21;
    o.TrailMidColour.Colour = 28;
    o.TrailEndColour.Colour = 35;
    o.StartColour.Colour = 42;
    o.MidColour.Colour = 49;
    o.EndColour.Colour = 56;
    o.BlendMode = 2;
    o.TrailBlendMode = 7;
    o.BlendOp = 4;
    o.TrailBlendOp = 1;
    o.FadeInEndInteger = 486;
    o.FadeOutBeginInteger = 523;
    o.TrailLengthInteger = 560;
    o.FlickerMinAlphaInteger = 597;
    o.FlickerMinSizeInteger = 634;
    o.StartRenderSizeX = 671;
    o.StartRenderSizeY = 708;
    o.StartRenderSizeZ = 745;
    o.CentredOnPos = 1;
    o.AlphaFadeEnable = 0;
    o.EndRenderSizeX = 856;
    o.EndRenderSizeY = 893;
    o.EndRenderSizeZ = 930;
    o.SizeFadeEnable = 0;
    o.FlickerEnable = 1;
    o.TrailUseStartColour = 0;
    o.TrailUseMidColour = 1;
    o.TrailUseEndColour = 0;
    o.UseRenderSizeParam = 1;
    o.FlickerBias = 165;
    o.FlickerSpeed = 1226;
    o.AlphaFadeMinimum = 111;
    o.UseStartColour = 1;
    o.SizeFadeMinimum = 57;
    o.UseMidColour = 1;
    o.TrailWidth = 387;
    o.UseEndColour = 1;
    o.RenderSizeParam = 4040;
    o.ChildPrimIndex = 4242;
    o.ChildTrailPrimIndex = 4444;
    Stream s;
    o.WriteBinary(&s);
    static const double want[40] = { 101.0, 202.0, 21.0, 28.0, 35.0, 42.0, 49.0, 56.0, 2.0, 7.0, 4.0, 1.0, 486.0, 523.0, 560.0, 597.0, 634.0, 671.0, 708.0, 745.0, 1.0, 0.0, 856.0, 893.0, 930.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 165.0, 1226.0, 111.0, 1.0, 57.0, 1.0, 387.0, 1.0, 4040.0 };
    static const char kinds[] = "llccccccuuuuuuuuuuuubbuuubbbbbbuuubububu";
    bool ok = s.n == 40;
    for (int i = 0; ok && i < 40; ++i) ok = s.kind[i] == kinds[i] && s.val[i] == want[i];
    if (!ok) { std::printf("00afee00_TEST FAIL\n"); return 1; }
    std::printf("00afee00_TEST PASS\n");
    return 0;
}