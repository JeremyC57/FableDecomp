// behaviour test for 0x00BB1780 CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite*) (model copy of the member initialisation)
#include <cstdio>
#include <cstring>
#include <new>
class CEngineInternalPrimitiveBase { public: int id; };
class CEngineInternalPrimitive2DClockSprite : public CEngineInternalPrimitiveBase { };
class CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager;
};
class CEngineSubPrimitive2DClockSprite : public CEngineSubPrimitiveBase { public: CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent) : CEngineSubPrimitiveBase(parent) { } };
int main()
{
    CEngineInternalPrimitive2DClockSprite prim;
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CEngineSubPrimitive2DClockSprite* s = new (raw) CEngineSubPrimitive2DClockSprite(&prim);
    int ok = s->Parent == &prim && s->NextSubPrimitive == 0;
    if (!ok) { std::printf("00bb1780_TEST FAIL\n"); return 1; }
    std::printf("00bb1780_TEST PASS\n");
    return 0;
}