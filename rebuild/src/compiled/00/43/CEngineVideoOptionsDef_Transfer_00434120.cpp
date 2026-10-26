// CEngineVideoOptionsDef::Transfer(CPersistContext&) -- transfers video options in persistence order.
// Retail 00434120, 244 bytes. Donor _quarantine/CEngineVideoOptionsDef.h
// has a 0x40-byte base; native first member is +0x3c (all members -4).
// Honest retail view uses the donor names; no native virtual calls occur here.
#include <stddef.h>
class CPersistContext { public:
 template <class T> void Transfer(T& value); // 00431102
};
class CEngineVideoOptionsDef { public:
 unsigned char DefinitionBase[0x3c];
 int HiresTextureMemory;
 float LODErrorTolerance, CharacterLODErrorTolerance, DrawDistanceMultiplier;
 float DrawDistanceMinimum, DrawDistanceMaximum, RepeatedMeshDrawDistanceFactor;
 float MinimumZSpriteAsMeshDistance, MaximumZSpriteAsMeshDistance, ZSpriteDrawDistanceMultiplier;
 int ShadowBufferSize;
 float ShadowDistanceScale;
 bool Enable2DDisplacement, Enable3DDisplacement, EnableGlow, EnableRadialBlur;
 bool EnableWaterReflection, EnableWeatherEffects, EnableColourFilter, EnableRepeatedMeshes;
 float WeatherDensity;
 void Transfer(CPersistContext& c);
};
typedef char NativeVideoSize[(sizeof(CEngineVideoOptionsDef)==0x78)?1:-1];
typedef char NativeVideoOffset[(offsetof(CEngineVideoOptionsDef,HiresTextureMemory)==0x3c)?1:-1];
void CEngineVideoOptionsDef::Transfer(CPersistContext& c) {
 c.Transfer(*(long*)&(HiresTextureMemory));
 c.Transfer(LODErrorTolerance);
 c.Transfer(CharacterLODErrorTolerance);
 c.Transfer(DrawDistanceMultiplier);
 c.Transfer(DrawDistanceMinimum);
 c.Transfer(DrawDistanceMaximum);
 c.Transfer(RepeatedMeshDrawDistanceFactor);
 c.Transfer(MinimumZSpriteAsMeshDistance);
 c.Transfer(MaximumZSpriteAsMeshDistance);
 c.Transfer(ZSpriteDrawDistanceMultiplier);
 c.Transfer(*(long*)&(ShadowBufferSize));
 c.Transfer(ShadowDistanceScale);
 c.Transfer(Enable2DDisplacement);
 c.Transfer(Enable3DDisplacement);
 c.Transfer(EnableGlow);
 c.Transfer(EnableRadialBlur);
 c.Transfer(EnableWaterReflection);
 c.Transfer(EnableWeatherEffects);
 c.Transfer(EnableColourFilter);
 c.Transfer(WeatherDensity);
 c.Transfer(EnableRepeatedMeshes);
}