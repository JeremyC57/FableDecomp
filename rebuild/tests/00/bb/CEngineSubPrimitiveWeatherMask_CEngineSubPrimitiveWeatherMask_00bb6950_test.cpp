// behaviour test for 0x00BB6950 CEngineSubPrimitiveWeatherMask::CEngineSubPrimitiveWeatherMask(CEngineInternalPrimitiveWeatherMask*) (model copy of the member initialisation)
#include <cstdio>
#include <cstring>
#include <new>
class CEngineInternalPrimitiveBase { public: int id; };
class CEngineInternalPrimitiveWeatherMask : public CEngineInternalPrimitiveBase { };
class CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager;
};
class CEngineSubPrimitiveWeatherMask : public CEngineSubPrimitiveBase { public: CEngineSubPrimitiveWeatherMask(CEngineInternalPrimitiveWeatherMask* parent) : CEngineSubPrimitiveBase(parent) { } };
int main()
{
    CEngineInternalPrimitiveWeatherMask prim;
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CEngineSubPrimitiveWeatherMask* s = new (raw) CEngineSubPrimitiveWeatherMask(&prim);
    int ok = s->Parent == &prim && s->NextSubPrimitive == 0;
    if (!ok) { std::printf("00bb6950_TEST FAIL\n"); return 1; }
    std::printf("00bb6950_TEST PASS\n");
    return 0;
}