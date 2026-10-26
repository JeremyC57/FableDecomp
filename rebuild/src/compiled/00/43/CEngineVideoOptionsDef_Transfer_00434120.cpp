// CEngineVideoOptionsDef::Transfer(CPersistContext&) -- transfers video options in persistence order.
// Retail 00434120, 244 bytes. Donor _quarantine/CEngineVideoOptionsDef.h
// has a 0x40-byte base; native first member is +0x3c (all members -4).
// Honest retail view uses the donor names; no native virtual calls occur here.
#include <stddef.h>
class CPersistContext { public:
 void TransferInt(int&); // 00431102
 void TransferFloat(float&); // 00431061
 void TransferBool(bool&); // 0043314a
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
 c.TransferInt(HiresTextureMemory);
 c.TransferFloat(LODErrorTolerance);
 c.TransferFloat(CharacterLODErrorTolerance);
 c.TransferFloat(DrawDistanceMultiplier);
 c.TransferFloat(DrawDistanceMinimum);
 c.TransferFloat(DrawDistanceMaximum);
 c.TransferFloat(RepeatedMeshDrawDistanceFactor);
 c.TransferFloat(MinimumZSpriteAsMeshDistance);
 c.TransferFloat(MaximumZSpriteAsMeshDistance);
 c.TransferFloat(ZSpriteDrawDistanceMultiplier);
 c.TransferInt(ShadowBufferSize);
 c.TransferFloat(ShadowDistanceScale);
 c.TransferBool(Enable2DDisplacement);
 c.TransferBool(Enable3DDisplacement);
 c.TransferBool(EnableGlow);
 c.TransferBool(EnableRadialBlur);
 c.TransferBool(EnableWaterReflection);
 c.TransferBool(EnableWeatherEffects);
 c.TransferBool(EnableColourFilter);
 c.TransferFloat(WeatherDensity);
 c.TransferBool(EnableRepeatedMeshes);
}