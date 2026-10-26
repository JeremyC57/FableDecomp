// NEnvironmentInternal::CBlendedEnvironmentTheme::BuildFromSource(const CEnvironmentDef& environment, const CEnvironmentLookupTexture& lookup, const CEnvironmentThemeDaySetDef& daySet, const CEnvironmentThemeDef& theme, long column) -- fill a blended theme from one theme def: the 19 colours from the lookup texture (column, the environment def's lookup rows), the 30 scalers from the theme and day-set defs, and the sky / upper-cloud / lower-cloud texture sets (two textures, blends 1 - b and b)
// Leaf: BuildFromSource
// Retail 0x008865C0, 1962 bytes, `ret 0x14`.
// Callees: CEnvironmentLookupTexture::GetColour(column, row) 0x008860D0 (returns CRGBFloatColour); STLport
//   vector<long>::_M_insert_overflow 0x00440990 / vector<float>::_M_insert_overflow 0x004408A0 (push_back's out-of-line
//   half). Float pool: 0x0122DED8 = 1.0f; 0x01237F00 / 0x0122ED70 = 1.0 / 0.0 (double).
// Layout: Ego_r.pdb CEnvironmentDef / CEnvironmentThemeDef less 4 (retail CDefBase) and CEnvironmentThemeDaySetDef less 8
//   (retail 12-byte CArray); CBlendedEnvironmentTheme as the landed Blend 0x00887B70 (texture sets at +0x1ac, 0x18 each).
void StandInSink(void* p, unsigned long n);
class CRGBFloatColour { public:
    float R, G, B, A;
};
struct __true_type {};
template <class T> class CArray { public:             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    template <class Tag> __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned long fill_len, bool atend) { StandInSink((void*)position, (unsigned long)fill_len + *(const unsigned char*)&x); }   // 0x00440990 / 0x004408A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1UL, true);
        }
    }
};
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                    // +0x00
    CArray<float> Blends;                      // +0x0c
};
struct CEnvironmentLookupTexture {
    long           Width;
    long           Height;
    long           Pitch;
    unsigned char* Data;
    CRGBFloatColour GetColour(long column, long row) const;   // 0x008860D0
};
class CEnvironmentDef { public:
    unsigned char _pad00[0x40];
    long DiffuseLookupRow;                     // +0x40
    long AmbientLookupRow;
    long BacklightLookupRow;
    long ReflectionLookupRow;
    long MistEffectColourLookupRow;            // +0x50
    long FogColourLookupRow;
    long FogAlphaLookupRow;
    long SunColourLookupRow;
    long CloudColourLookupRow;                 // +0x60
    long MoonColourLookupRow;
    long StarsColourLookupRow;
    long SunFlareColourLookupRow;
    long LensFlareColourLookupRow;             // +0x70
    long SkyGradientTopLookupRow;
    long SkyGradientTopAlphaLookupRow;
    long SkyGradientBottomLookupRow;
    long SkyGradientBottomAlphaLookupRow;      // +0x80
    long WaterColourLookupRow;
    long SeaColourLookupRow;
    long GlowThresholdColourLookupRow;
    long GlowBloomColourLookupRow;             // +0x90
    long SunlightAttenuatorColourLookupRow;
};
class CEnvironmentThemeDaySetDef { public:
    unsigned char _pad00[0x48];
    float SunTilt;                             // +0x48
    float SunRotate;
    float SunHeight;
    float MoonTilt;
    float MoonRotate;
    float MoonHeight;
};
class CEnvironmentThemeDef { public:
    unsigned char _pad00[0x3c];
    float TimeOfDay;                           // +0x3c
    bool  MoonLit;                             // +0x40
    float FogStartZ;                           // +0x44
    float FogEndZ;
    long  SkyTexture0;                         // +0x4c
    long  SkyTexture1;
    float SkyTexture1Blend;
    long  CloudLowerLayerTexture0;             // +0x58
    long  CloudLowerLayerTexture1;
    float CloudLowerLayerTexture1Blend;
    float CloudLowerLayerSpeedMultiplier;      // +0x64
    long  CloudUpperLayerTexture0;
    long  CloudUpperLayerTexture1;
    float CloudUpperLayerTexture1Blend;        // +0x70
    float CloudUpperLayerSpeedMultiplier;
    float WaterColourToReflectionBlend;
    float WaterAlphaFactor;
    float WaterSpecularHightlightFactor;       // +0x80
    float RainStrength;
    float SnowStrength;
    float MistMaxAlpha;
    float LightningFrequency;                  // +0x90
    float GlowThresholdScale;
    float GlowBloomScale;
    float GlowMotionBlur;
    float ShadowFactor;                        // +0xa0
    float FadedShadowFactor;
    float WaterRefractionBlendStart;
    float WaterRefractionBlendEnd;
    float WaterFlowSpeedFactor;                // +0xb0
    float WaterOscilationSpeed;
    float WaterSwellFactor;
    float WaterShoreSwellFactor;
    float WaterReflectionOffset;               // +0xc0
};
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    float                       TimeOfDay;     // +0x000
    CRGBFloatColour             Colours[19];   // +0x004
    float                       Scalers[30];   // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];   // +0x1ac
    void BuildFromSource(const CEnvironmentDef& environment, const CEnvironmentLookupTexture& lookup,
                         const CEnvironmentThemeDaySetDef& daySet, const CEnvironmentThemeDef& theme, long column);
};
}

void NEnvironmentInternal::CBlendedEnvironmentTheme::BuildFromSource(const CEnvironmentDef& environment,
                                                                      const CEnvironmentLookupTexture& lookup,
                                                                      const CEnvironmentThemeDaySetDef& daySet,
                                                                      const CEnvironmentThemeDef& theme, long column)
{
    TimeOfDay = theme.TimeOfDay;
    Colours[0] = lookup.GetColour(column, environment.DiffuseLookupRow);
    Colours[1] = lookup.GetColour(column, environment.AmbientLookupRow);
    Colours[2] = lookup.GetColour(column, environment.BacklightLookupRow);
    Colours[3] = lookup.GetColour(column, environment.ReflectionLookupRow);
    Colours[4] = lookup.GetColour(column, environment.MistEffectColourLookupRow);
    Colours[5] = lookup.GetColour(column, environment.FogColourLookupRow);
    Colours[6] = lookup.GetColour(column, environment.SunColourLookupRow);
    Colours[7] = lookup.GetColour(column, environment.CloudColourLookupRow);
    Colours[10] = lookup.GetColour(column, environment.SunFlareColourLookupRow);
    Colours[11] = lookup.GetColour(column, environment.LensFlareColourLookupRow);
    Colours[8] = lookup.GetColour(column, environment.MoonColourLookupRow);
    Colours[9] = lookup.GetColour(column, environment.StarsColourLookupRow);
    Colours[12] = lookup.GetColour(column, environment.SkyGradientTopLookupRow);
    Colours[13] = lookup.GetColour(column, environment.SkyGradientBottomLookupRow);
    Colours[14] = lookup.GetColour(column, environment.WaterColourLookupRow);
    Colours[15] = lookup.GetColour(column, environment.SeaColourLookupRow);
    Colours[5].A = lookup.GetColour(column, environment.FogAlphaLookupRow).R;
    Colours[12].A = lookup.GetColour(column, environment.SkyGradientTopAlphaLookupRow).R;
    Colours[13].A = lookup.GetColour(column, environment.SkyGradientBottomAlphaLookupRow).R;
    Colours[16] = lookup.GetColour(column, environment.GlowThresholdColourLookupRow);
    Colours[17] = lookup.GetColour(column, environment.GlowBloomColourLookupRow);
    Colours[18] = lookup.GetColour(column, environment.SunlightAttenuatorColourLookupRow);
    Scalers[2] = theme.WaterColourToReflectionBlend;
    Scalers[3] = theme.WaterAlphaFactor;
    Scalers[4] = theme.WaterSpecularHightlightFactor;
    Scalers[0] = theme.CloudLowerLayerSpeedMultiplier;
    Scalers[1] = theme.CloudUpperLayerSpeedMultiplier;
    Scalers[5] = (float)(theme.MoonLit ? 1.0 : 0.0);
    Scalers[6] = theme.FogStartZ;
    Scalers[7] = theme.FogEndZ;
    Scalers[15] = theme.RainStrength;
    Scalers[17] = theme.LightningFrequency;
    Scalers[14] = theme.SnowStrength;
    Scalers[16] = theme.MistMaxAlpha;
    Scalers[18] = theme.GlowThresholdScale;
    Scalers[19] = theme.GlowBloomScale;
    Scalers[20] = theme.GlowMotionBlur;
    Scalers[8] = daySet.SunTilt;
    Scalers[9] = daySet.SunRotate;
    Scalers[10] = daySet.SunHeight;
    Scalers[11] = daySet.MoonTilt;
    Scalers[12] = daySet.MoonRotate;
    Scalers[13] = daySet.MoonHeight;
    Scalers[21] = theme.ShadowFactor;
    Scalers[22] = theme.FadedShadowFactor;
    Scalers[23] = theme.WaterRefractionBlendStart;
    Scalers[24] = theme.WaterRefractionBlendEnd;
    Scalers[25] = theme.WaterFlowSpeedFactor;
    Scalers[26] = theme.WaterOscilationSpeed;
    Scalers[27] = theme.WaterSwellFactor;
    Scalers[28] = theme.WaterShoreSwellFactor;
    Scalers[29] = theme.WaterReflectionOffset;
    TextureSets[0].Textures.push_back(theme.SkyTexture0);
    TextureSets[0].Textures.push_back(theme.SkyTexture1);
    TextureSets[0].Blends.push_back(1.0f - theme.SkyTexture1Blend);
    TextureSets[0].Blends.push_back(theme.SkyTexture1Blend);
    TextureSets[2].Textures.push_back(theme.CloudLowerLayerTexture0);
    TextureSets[2].Textures.push_back(theme.CloudLowerLayerTexture1);
    TextureSets[2].Blends.push_back(1.0f - theme.CloudLowerLayerTexture1Blend);
    TextureSets[2].Blends.push_back(theme.CloudLowerLayerTexture1Blend);
    TextureSets[1].Textures.push_back(theme.CloudUpperLayerTexture0);
    TextureSets[1].Textures.push_back(theme.CloudUpperLayerTexture0);
    TextureSets[1].Blends.push_back(1.0f - theme.CloudUpperLayerTexture1Blend);
    TextureSets[1].Blends.push_back(theme.CloudUpperLayerTexture1Blend);
}