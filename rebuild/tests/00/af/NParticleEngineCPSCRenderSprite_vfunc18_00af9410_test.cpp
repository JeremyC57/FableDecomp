#include <cstdio>
// BEGIN exact source snapshot SHA256 f20b707c3b65962135c5d7a66a158adad212292e7d7d7dedf8d4aad470cc21a7
// Original method recovered from Ego_r and FableWin PDBs; see advisor/NAMES.md.
// 00AF9410 NParticleEngine::CPSCRenderSprite::DatabaseCopy, 522 retail bytes.
// Direct vtable evidence identifies slot 18. No generated header is present; the
// local layout follows PDB CPSCRenderSprite and the source-level bitfield order is
// checked against the field offsets in struct_layouts_egor.tsv.
namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase {
    virtual void v000() {}
    CParticleSystem* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
};
struct CRGBColour { unsigned int Colour; };
struct CPSCRenderSprite : CPSCBase {
    unsigned int SpriteBankIndex;
    unsigned int TrailBankIndex;
    CRGBColour StartColour, MidColour, EndColour;
    unsigned int BlendMode : 3;
    unsigned int TrailBlendMode : 3;
    unsigned int BlendOp : 3;
    unsigned int TrailBlendOp : 3;
    unsigned int Flags : 10;
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned int TrailLengthInteger : 12;
    unsigned int FlickerMinAlphaInteger : 10;
    unsigned int FlickerMinSizeInteger : 10;
    unsigned int NoCrossedSprites : 5;
    unsigned int StartRenderSize : 11;
    unsigned int AlphaFadeMinimum : 7;
    unsigned int EndRenderSize : 11;
    unsigned int FlickerBias : 8;
    unsigned int AnimationTimeSecs : 14;
    unsigned int SizeFadeMinimum : 7;
    unsigned int TrailWidth : 10;
    unsigned int FlickerSpeed : 12;
    unsigned int UseStartColour : 1;
    unsigned int UseMidColour : 1;
    unsigned int UseEndColour : 1;
    unsigned char AlphaFadeEnable : 1;
    unsigned char SizeFadeEnable : 1;
    unsigned char FlickerEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned int PrimHandleData;
    unsigned int PrimHandleInfo;
    long ChildPrimIndex;
    unsigned int TrailPrimHandleData;
    unsigned int TrailPrimHandleInfo;
    long ChildTrailPrimIndex;
    void DatabaseCopy(const CPSCBase& other);
};
void CPSCRenderSprite::DatabaseCopy(const CPSCBase& other)
{
    const CPSCRenderSprite* source = (const CPSCRenderSprite*)&other;
    SpriteBankIndex = source->SpriteBankIndex;
    ForceAnimationTime = source->ForceAnimationTime;
    AnimationTimeSecs = source->AnimationTimeSecs;
    StartRenderSize = source->StartRenderSize;
    EndRenderSize = source->EndRenderSize;
    BlendMode = source->BlendMode;
    BlendOp = source->BlendOp;
    TrailBlendMode = source->TrailBlendMode;
    TrailBlendOp = source->TrailBlendOp;
    NoCrossedSprites = source->NoCrossedSprites;
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
    UseStartColour = source->UseStartColour;
    UseMidColour = source->UseMidColour;
    UseEndColour = source->UseEndColour;
    StartColour = source->StartColour;
    MidColour = source->MidColour;
    EndColour = source->EndColour;
    TrailBankIndex = source->TrailBankIndex;
    TrailLengthInteger = source->TrailLengthInteger;
    TrailWidth = source->TrailWidth;
    Flags = source->Flags;
}
}

// END exact source snapshot
int main()
{
    static NParticleEngine::CPSCRenderSprite src;
    static NParticleEngine::CPSCRenderSprite dst;
    src.SpriteBankIndex = 17; src.TrailBankIndex = 23;
    src.StartColour.Colour = 0x11223344; src.MidColour.Colour = 0x55667788; src.EndColour.Colour = 0x99aabbcc;
    src.BlendMode = 3; src.TrailBlendMode = 4; src.BlendOp = 5; src.TrailBlendOp = 6;
    src.Flags = 0x155; src.FadeInEndInteger = 0x201; src.FadeOutBeginInteger = 0x102;
    src.TrailLengthInteger = 0xabc; src.FlickerMinAlphaInteger = 0x155; src.FlickerMinSizeInteger = 0x2aa;
    src.NoCrossedSprites = 17; src.StartRenderSize = 0x345; src.EndRenderSize = 0x456;
    src.AlphaFadeMinimum = 0x55; src.SizeFadeMinimum = 0x33; src.FlickerBias = 0xa5;
    src.AnimationTimeSecs = 0x2345; src.TrailWidth = 0x301; src.FlickerSpeed = 0x789;
    src.UseStartColour = 1; src.UseMidColour = 0; src.UseEndColour = 1;
    src.AlphaFadeEnable = 1; src.SizeFadeEnable = 1; src.FlickerEnable = 1; src.ForceAnimationTime = 1;
    dst.PrimHandleData = 99; dst.PrimHandleInfo = 98; dst.ChildPrimIndex = 101;
    dst.TrailPrimHandleData = 97; dst.TrailPrimHandleInfo = 96; dst.ChildTrailPrimIndex = 95;
    dst.DatabaseCopy(src);
    if (dst.SpriteBankIndex != 17 || dst.TrailBankIndex != 23 || dst.StartColour.Colour != 0x11223344 ||
        dst.MidColour.Colour != 0x55667788 || dst.EndColour.Colour != 0x99aabbcc || dst.BlendMode != 3 ||
        dst.TrailBlendMode != 4 || dst.BlendOp != 5 || dst.TrailBlendOp != 6 || dst.Flags != 0x155 ||
        dst.FadeInEndInteger != 0x201 || dst.FadeOutBeginInteger != 0x102 || dst.TrailLengthInteger != 0xabc ||
        dst.FlickerMinAlphaInteger != 0x155 || dst.FlickerMinSizeInteger != 0x2aa || dst.NoCrossedSprites != 17 ||
        dst.StartRenderSize != 0x345 || dst.EndRenderSize != 0x456 || dst.AlphaFadeMinimum != 0x55 ||
        dst.SizeFadeMinimum != 0x33 || dst.FlickerBias != 0xa5 || dst.AnimationTimeSecs != 0x2345 ||
        dst.TrailWidth != 0x301 || dst.FlickerSpeed != 0x789 || !dst.UseStartColour || dst.UseMidColour ||
        !dst.UseEndColour || !dst.AlphaFadeEnable || !dst.SizeFadeEnable || !dst.FlickerEnable ||
        !dst.ForceAnimationTime || dst.PrimHandleData != 99 || dst.PrimHandleInfo != 98 ||
        dst.ChildPrimIndex != 101 || dst.TrailPrimHandleData != 97 || dst.TrailPrimHandleInfo != 96 ||
        dst.ChildTrailPrimIndex != 95) {
        std::printf("00af9410_TEST FAIL\n"); return 1;
    }
    std::printf("00af9410_TEST PASS\n"); return 0;
}