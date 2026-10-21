#include <cstdio>
#include <cstring>
// Behaviour test for CPSCDecalRenderer::DatabaseCopy (0x00B1B810): every setting is copied, bit-fields keep their neighbours.
struct CRGBColour { unsigned int Colour; };
struct CPSCDecalRenderer {
    float         DecalLifeSecs;                        // +0x0c
    long          DecalBankIndex;                       // +0x10
    CRGBColour    StartColour;                          // +0x14
    CRGBColour    MidColour;                            // +0x18
    CRGBColour    EndColour;                            // +0x1c
    unsigned int BlendMode : 3;                        // +0x20
    unsigned int BlendOp : 3;
    unsigned int Flags : 10;
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;             // +0x24
    unsigned int StartRenderSize : 11;
    unsigned int AlphaFadeMinimum : 7;
    unsigned int EndRenderSize : 11;                   // +0x28
    unsigned int AnimationTimeSecs : 14;
    unsigned int SizeFadeMinimum : 7;
    unsigned int UseStartColour : 1;                   // +0x2c
    unsigned int UseMidColour : 1;
    unsigned int UseEndColour : 1;
    unsigned int FlickerBias : 8;
    unsigned int FlickerMinAlphaInteger : 10;
    unsigned int FlickerMinSizeInteger : 10;
    unsigned int FlickerSpeed : 12;                    // +0x30
    unsigned char AlphaFadeEnable : 1;                  // +0x34
    unsigned char SizeFadeEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1;
    unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;          // +0x35
    float         MaxPoolSize;                          // +0x38
    float         MaxPoolAlpha;                         // +0x3c
    float         MaxPoolLife;                          // +0x40
    float         PoolFrameIncreaseRate;                // +0x44
    float         StencilCubeWidth;                     // +0x48
    void DatabaseCopy(const CPSCDecalRenderer* other)
    {
    const CPSCDecalRenderer* source = other;
    DecalLifeSecs = source->DecalLifeSecs;
    DecalBankIndex = source->DecalBankIndex;
    ForceAnimationTime = source->ForceAnimationTime;
    AnimationTimeSecs = source->AnimationTimeSecs;
    StartRenderSize = source->StartRenderSize;
    EndRenderSize = source->EndRenderSize;
    BlendMode = source->BlendMode;
    BlendOp = source->BlendOp;
    FadeInEndInteger = source->FadeInEndInteger;
    FadeOutBeginInteger = source->FadeOutBeginInteger;
    AlphaFadeEnable = source->AlphaFadeEnable;
    AlphaFadeMinimum = source->AlphaFadeMinimum;
    SizeFadeEnable = source->SizeFadeEnable;
    SizeFadeMinimum = source->SizeFadeMinimum;
    UseStartColour = source->UseStartColour;
    UseMidColour = source->UseMidColour;
    UseEndColour = source->UseEndColour;
    StartColour = source->StartColour;
    MidColour = source->MidColour;
    EndColour = source->EndColour;
    Flags = source->Flags;
    FlickerEnable = source->FlickerEnable;
    FlickerSpeed = source->FlickerSpeed;
    FlickerBias = source->FlickerBias;
    FlickerMinAlphaInteger = source->FlickerMinAlphaInteger;
    FlickerMinSizeInteger = source->FlickerMinSizeInteger;
    PoolingEnable = source->PoolingEnable;
    PoolIncreaseAlphaEnable = source->PoolIncreaseAlphaEnable;
    PoolIncreaseSizeEnable = source->PoolIncreaseSizeEnable;
    PoolIncreaseLifeEnable = source->PoolIncreaseLifeEnable;
    MaxPoolSize = source->MaxPoolSize;
    MaxPoolAlpha = source->MaxPoolAlpha;
    MaxPoolLife = source->MaxPoolLife;
    PoolIncreaseFrameEnable = source->PoolIncreaseFrameEnable;
    PoolFrameIncreaseRate = source->PoolFrameIncreaseRate;
    StencilCubeWidth = source->StencilCubeWidth;
}
};
int main()
{
    CPSCDecalRenderer a, b;
    std::memset(&a, 0, sizeof a);
    std::memset(&b, 0, sizeof b);
    b.DecalLifeSecs = (float)3;
    b.DecalBankIndex = (long)4;
    b.StartColour.Colour = 102;
    b.MidColour.Colour = 103;
    b.EndColour.Colour = 104;
    b.BlendMode = 6;
    b.BlendOp = 3;
    b.Flags = 264;
    b.FadeInEndInteger = 301;
    b.FadeOutBeginInteger = 338;
    b.StartRenderSize = 375;
    b.AlphaFadeMinimum = 28;
    b.EndRenderSize = 449;
    b.AnimationTimeSecs = 486;
    b.SizeFadeMinimum = 11;
    b.UseStartColour = 1;
    b.UseMidColour = 1;
    b.UseEndColour = 1;
    b.FlickerBias = 159;
    b.FlickerMinAlphaInteger = 708;
    b.FlickerMinSizeInteger = 745;
    b.FlickerSpeed = 782;
    b.AlphaFadeEnable = 1;
    b.SizeFadeEnable = 1;
    b.ForceAnimationTime = 1;
    b.FlickerEnable = 1;
    b.PoolingEnable = 1;
    b.PoolIncreaseAlphaEnable = 1;
    b.PoolIncreaseSizeEnable = 1;
    b.PoolIncreaseLifeEnable = 1;
    b.PoolIncreaseFrameEnable = 1;
    b.MaxPoolSize = (float)34;
    b.MaxPoolAlpha = (float)35;
    b.MaxPoolLife = (float)36;
    b.PoolFrameIncreaseRate = (float)37;
    b.StencilCubeWidth = (float)38;
    a.DatabaseCopy(&b);
    int bad = 0;
    bad |= a.DecalLifeSecs != (float)3;
    bad |= a.DecalBankIndex != (long)4;
    bad |= a.StartColour.Colour != 102;
    bad |= a.MidColour.Colour != 103;
    bad |= a.EndColour.Colour != 104;
    bad |= a.BlendMode != 6;
    bad |= a.BlendOp != 3;
    bad |= a.Flags != 264;
    bad |= a.FadeInEndInteger != 301;
    bad |= a.FadeOutBeginInteger != 338;
    bad |= a.StartRenderSize != 375;
    bad |= a.AlphaFadeMinimum != 28;
    bad |= a.EndRenderSize != 449;
    bad |= a.AnimationTimeSecs != 486;
    bad |= a.SizeFadeMinimum != 11;
    bad |= a.UseStartColour != 1;
    bad |= a.UseMidColour != 1;
    bad |= a.UseEndColour != 1;
    bad |= a.FlickerBias != 159;
    bad |= a.FlickerMinAlphaInteger != 708;
    bad |= a.FlickerMinSizeInteger != 745;
    bad |= a.FlickerSpeed != 782;
    bad |= a.AlphaFadeEnable != 1;
    bad |= a.SizeFadeEnable != 1;
    bad |= a.ForceAnimationTime != 1;
    bad |= a.FlickerEnable != 1;
    bad |= a.PoolingEnable != 1;
    bad |= a.PoolIncreaseAlphaEnable != 1;
    bad |= a.PoolIncreaseSizeEnable != 1;
    bad |= a.PoolIncreaseLifeEnable != 1;
    bad |= a.PoolIncreaseFrameEnable != 1;
    bad |= a.MaxPoolSize != (float)34;
    bad |= a.MaxPoolAlpha != (float)35;
    bad |= a.MaxPoolLife != (float)36;
    bad |= a.PoolFrameIncreaseRate != (float)37;
    bad |= a.StencilCubeWidth != (float)38;
    if (bad) { std::printf("00b1b810_TEST FAIL\n"); return 1; }
    std::printf("00b1b810_TEST PASS\n");
    return 0;
}