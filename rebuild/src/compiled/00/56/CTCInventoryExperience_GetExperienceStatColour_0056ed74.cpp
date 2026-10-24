// CTCInventoryExperience::GetExperienceStatColour(EHeroTrainableStatType stat) -- the spend-screen colour of a trainable stat's band: red for 0..2, yellow for 3..5, blue otherwise
// Leaf: ?GetExperienceStatColour@CTCInventoryExperience@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056ED74, 94 bytes, thiscall `ret 8` (hidden CRGBFloatColour result + stat;
//   `this` unused). Name from FableWin.pdb (`?GetExperienceStatColour@CTCInventoryExperience@@QAE?AVCRGBFloatColour
//   @@W4EHeroTrainableStatType@@@Z`); its only callers are AddStatToList (0x005729D8) and AddStatToListPC
//   (0x00573792), which take an EHeroTrainableStatType. It sits right before GetExperienceAbilityColour
//   (landed, 0x0056EDD2). Label correction: the catalog "SetColorRGBA" is a guess.
// Colours: stats 0..2 (1, 0.3, 0.3, 1), 3..5 (1, 1, 0.3, 1), otherwise (0.3, 0.3, 1, 1); 0.3f is the pool
//   constant 0x01238170 (and the immediate 0x3E99999A in the middle arm). Each return is an inline four-float
//   constructor into the hidden result, tail-merged across the arms (the `fstp [eax+8] ; fld1 ; fstp [eax+0Ch]`
//   tails are shared). Signed range tests (`jl`/`jg`): the enum. Size TU (`fld1 ; fstp` over 7-byte immediates):
//   `optimize("s", on)`; no frame (unlike the sibling, which keeps `optimize("y", off)` for its local tables).
#pragma optimize("s", on)

struct CRGBFloatColour {
    float R;                                       // +0x00
    float G;                                       // +0x04
    float B;                                       // +0x08
    float A;                                       // +0x0c
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };

struct CTCInventoryExperience {
    CRGBFloatColour GetExperienceStatColour(EHeroTrainableStatType stat);
};

CRGBFloatColour CTCInventoryExperience::GetExperienceStatColour(EHeroTrainableStatType stat)
{
    if (stat >= 0 && stat <= 2)
        return CRGBFloatColour(1.0f, 0.3f, 0.3f, 1.0f);
    if (stat >= 3 && stat <= 5)
        return CRGBFloatColour(1.0f, 1.0f, 0.3f, 1.0f);
    return CRGBFloatColour(0.3f, 0.3f, 1.0f, 1.0f);
}