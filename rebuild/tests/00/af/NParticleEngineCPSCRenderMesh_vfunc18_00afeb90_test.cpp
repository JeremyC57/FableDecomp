#include <cstdio>
// BEGIN exact source snapshot SHA256 dd4c0c231f362c24c2e2f2ee69edbe264397f3fd3a8027168573b090f360a161
// Original method recovered from Ego_r and FableWin PDBs; see advisor/NAMES.md.
// 00AFEB90 NParticleEngine::CPSCRenderMesh::DatabaseCopy, 620 retail bytes.
// Exact RTTI-vtable evidence identifies slot 18. The generated header is absent;
// this model follows PDB CPSCRenderMesh's 100-byte field layout and packed unsigned
// bit units. Assignment order was transcribed from the retail field loads.
namespace NParticleEngine {
class CParticleSystem;
class CPSCBase { public:
    virtual void v000() {}
    CParticleSystem* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
};
struct CRGBColour { unsigned int Colour; };
class CPSCRenderMesh : public CPSCBase { public:
    unsigned int BankIndex;
    unsigned int TrailBankIndex;
    CRGBColour TrailStartColour, TrailMidColour, TrailEndColour;
    CRGBColour StartColour, MidColour, EndColour;
    unsigned int BlendMode : 3;
    unsigned int TrailBlendMode : 3;
    unsigned int BlendOp : 3;
    unsigned int TrailBlendOp : 3;
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned int TrailLengthInteger : 12;
    unsigned int FlickerMinAlphaInteger : 10;
    unsigned int FlickerMinSizeInteger : 10;
    unsigned int StartRenderSizeX : 11;
    unsigned int StartRenderSizeY : 11;
    unsigned int StartRenderSizeZ : 11;
    unsigned int CentredOnPos : 1;
    unsigned int AlphaFadeEnable : 1;
    unsigned int EndRenderSizeX : 11;
    unsigned int EndRenderSizeY : 11;
    unsigned int EndRenderSizeZ : 11;
    unsigned int SizeFadeEnable : 1;
    unsigned int FlickerEnable : 1;
    unsigned int TrailUseStartColour : 1;
    unsigned int TrailUseMidColour : 1;
    unsigned int TrailUseEndColour : 1;
    unsigned int UseRenderSizeParam : 1;
    unsigned int FlickerBias : 8;
    unsigned int FlickerSpeed : 12;
    unsigned int AlphaFadeMinimum : 7;
    unsigned int UseStartColour : 1;
    unsigned int SizeFadeMinimum : 7;
    unsigned int UseMidColour : 1;
    unsigned int TrailWidth : 10;
    unsigned int UseEndColour : 1;
    unsigned int RenderSizeParam;
    unsigned int PrimHandleData;
    unsigned int PrimHandleInfo;
    long ChildPrimIndex;
    unsigned int TrailPrimHandleData;
    unsigned int TrailPrimHandleInfo;
    long ChildTrailPrimIndex;
    void DatabaseCopy(const CPSCBase& other);
};
void CPSCRenderMesh::DatabaseCopy(const CPSCBase& other)
{
    const CPSCRenderMesh* source = (const CPSCRenderMesh*)&other;
    BankIndex = source->BankIndex;
    StartRenderSizeX = source->StartRenderSizeX;
    StartRenderSizeY = source->StartRenderSizeY;
    StartRenderSizeZ = source->StartRenderSizeZ;
    EndRenderSizeX = source->EndRenderSizeX;
    EndRenderSizeY = source->EndRenderSizeY;
    EndRenderSizeZ = source->EndRenderSizeZ;
    UseRenderSizeParam = source->UseRenderSizeParam;
    RenderSizeParam = source->RenderSizeParam;
    UseStartColour = source->UseStartColour;
    UseMidColour = source->UseMidColour;
    UseEndColour = source->UseEndColour;
    StartColour = source->StartColour;
    MidColour = source->MidColour;
    EndColour = source->EndColour;
    CentredOnPos = source->CentredOnPos;
    FadeInEndInteger = source->FadeInEndInteger;
    FadeOutBeginInteger = source->FadeOutBeginInteger;
    AlphaFadeEnable = source->AlphaFadeEnable;
    AlphaFadeMinimum = source->AlphaFadeMinimum;
    SizeFadeEnable = source->SizeFadeEnable;
    SizeFadeMinimum = source->SizeFadeMinimum;
    FlickerEnable = source->FlickerEnable;
    FlickerSpeed = source->FlickerSpeed;
    FlickerBias = source->FlickerBias;
    FlickerMinAlphaInteger = source->FlickerMinAlphaInteger;
    FlickerMinSizeInteger = source->FlickerMinSizeInteger;
    TrailBankIndex = source->TrailBankIndex;
    TrailLengthInteger = source->TrailLengthInteger;
    TrailWidth = source->TrailWidth;
    TrailUseStartColour = source->TrailUseStartColour;
    TrailUseMidColour = source->TrailUseMidColour;
    TrailUseEndColour = source->TrailUseEndColour;
    TrailStartColour = source->TrailStartColour;
    TrailMidColour = source->TrailMidColour;
    TrailEndColour = source->TrailEndColour;
    BlendMode = source->BlendMode;
    BlendOp = source->BlendOp;
    TrailBlendMode = source->TrailBlendMode;
    TrailBlendOp = source->TrailBlendOp;
}
}

// END exact source snapshot
int main()
{
    static NParticleEngine::CPSCRenderMesh src;
    static NParticleEngine::CPSCRenderMesh dst;
    src.BankIndex = 19; src.TrailBankIndex = 21; src.RenderSizeParam = 33;
    src.StartRenderSizeX = 101; src.StartRenderSizeY = 202; src.StartRenderSizeZ = 303;
    src.EndRenderSizeX = 404; src.EndRenderSizeY = 505; src.EndRenderSizeZ = 606;
    src.UseRenderSizeParam = 1; src.UseStartColour = 1; src.UseMidColour = 0; src.UseEndColour = 1;
    src.StartColour.Colour = 0x11223344; src.MidColour.Colour = 0x55667788; src.EndColour.Colour = 0x99aabbcc;
    src.CentredOnPos = 1; src.FadeInEndInteger = 201; src.FadeOutBeginInteger = 302;
    src.AlphaFadeEnable = 1; src.AlphaFadeMinimum = 55; src.SizeFadeEnable = 1; src.SizeFadeMinimum = 66;
    src.FlickerEnable = 1; src.FlickerSpeed = 77; src.FlickerBias = 88;
    src.FlickerMinAlphaInteger = 99; src.FlickerMinSizeInteger = 111; src.TrailLengthInteger = 122;
    src.TrailWidth = 133; src.TrailUseStartColour = 1; src.TrailUseMidColour = 0; src.TrailUseEndColour = 1;
    src.TrailStartColour.Colour = 0xaabbccdd; src.TrailMidColour.Colour = 0x10203040; src.TrailEndColour.Colour = 0x50607080;
    src.BlendMode = 1; src.BlendOp = 2; src.TrailBlendMode = 3; src.TrailBlendOp = 4;
    dst.PrimHandleData = 199; dst.PrimHandleInfo = 198; dst.ChildPrimIndex = 201;
    dst.TrailPrimHandleData = 197; dst.TrailPrimHandleInfo = 196; dst.ChildTrailPrimIndex = 195;
    dst.DatabaseCopy(src);
    if (dst.BankIndex != 19 || dst.TrailBankIndex != 21 || dst.RenderSizeParam != 33 ||
        dst.StartRenderSizeX != 101 || dst.StartRenderSizeY != 202 || dst.StartRenderSizeZ != 303 ||
        dst.EndRenderSizeX != 404 || dst.EndRenderSizeY != 505 || dst.EndRenderSizeZ != 606 ||
        !dst.UseRenderSizeParam || !dst.UseStartColour || dst.UseMidColour || !dst.UseEndColour ||
        dst.StartColour.Colour != 0x11223344 || dst.MidColour.Colour != 0x55667788 || dst.EndColour.Colour != 0x99aabbcc ||
        !dst.CentredOnPos || dst.FadeInEndInteger != 201 || dst.FadeOutBeginInteger != 302 || !dst.AlphaFadeEnable ||
        dst.AlphaFadeMinimum != 55 || !dst.SizeFadeEnable || dst.SizeFadeMinimum != 66 || !dst.FlickerEnable ||
        dst.FlickerSpeed != 77 || dst.FlickerBias != 88 || dst.FlickerMinAlphaInteger != 99 || dst.FlickerMinSizeInteger != 111 ||
        dst.TrailLengthInteger != 122 || dst.TrailWidth != 133 || !dst.TrailUseStartColour || dst.TrailUseMidColour ||
        !dst.TrailUseEndColour || dst.TrailStartColour.Colour != 0xaabbccdd || dst.TrailMidColour.Colour != 0x10203040 ||
        dst.TrailEndColour.Colour != 0x50607080 || dst.BlendMode != 1 || dst.BlendOp != 2 || dst.TrailBlendMode != 3 ||
        dst.TrailBlendOp != 4 || dst.PrimHandleData != 199 || dst.PrimHandleInfo != 198 ||
        dst.ChildPrimIndex != 201 || dst.TrailPrimHandleData != 197 || dst.TrailPrimHandleInfo != 196 ||
        dst.ChildTrailPrimIndex != 195) {
        std::printf("00afeb90_TEST FAIL\n"); return 1;
    }
    std::printf("00afeb90_TEST PASS\n"); return 0;
}