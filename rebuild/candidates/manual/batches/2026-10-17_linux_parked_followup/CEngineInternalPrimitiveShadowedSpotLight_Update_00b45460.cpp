// CEngineInternalPrimitiveShadowedSpotLight::Update(ShadowedLightData*,void*) -- copy light parameters, colour bytes and vectors into the primitive
// Grade: DIFFER(164v164) -- after four parity attempts, the generated instruction body is one evaluation-order permutation away from retail; latest source uses an inline four-float Set to reproduce VC7.1 FPU scheduling.
// Retail 0x00B45460, 176 bytes. The first argument supplies light data; the second is retained at +0x0c.
extern float g_ByteToFloat; // 0x01231724, 1/255
struct CVector3 { float x,y,z; };
struct CFloatColour { float c0,c1,c2,c3; void Set(float a,float b,float c,float d){c0=a;c1=b;c2=c;c3=d;} };
struct ShadowedLightData {
 unsigned char pad00[0x0c]; CVector3 direction; CVector3 position;
 unsigned char r,g,b,alpha; long field28; long field2c; long field30;
};
struct CEngineInternalPrimitiveShadowedSpotLight {
 unsigned char pad00[0x0c]; void* source; unsigned char pad10[0x38];
 CVector3 direction; CVector3 position;
 CFloatColour colour; long field70; long field74;
 long field78;
 void Update(ShadowedLightData* light, void* retained);
};
void CEngineInternalPrimitiveShadowedSpotLight::Update(ShadowedLightData* light, void* retained)
{
 source=retained;
 direction=light->direction;
 field70=light->field28;
 colour.Set(light->b*g_ByteToFloat,light->g*g_ByteToFloat,light->r*g_ByteToFloat,light->alpha*g_ByteToFloat);
 field74=light->field2c;
 position=light->position;
 field78=light->field30;
}
