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