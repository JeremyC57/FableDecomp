// behaviour test for 0x00BBA260 CEngineSubPrimitiveDecal::CEngineSubPrimitiveDecal(CEngineInternalPrimitiveDecal*) (model copy of the member initialisation)
#include <cstdio>
#include <cstring>
#include <new>
class CEngineInternalPrimitiveBase { public: int id; };
class CEngineInternalPrimitiveDecal : public CEngineInternalPrimitiveBase { };
class CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager;
};
class CEngineSubPrimitiveDecal : public CEngineSubPrimitiveBase { public: CEngineSubPrimitiveDecal(CEngineInternalPrimitiveDecal* parent) : CEngineSubPrimitiveBase(parent) { } };
int main()
{
    CEngineInternalPrimitiveDecal prim;
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CEngineSubPrimitiveDecal* s = new (raw) CEngineSubPrimitiveDecal(&prim);
    int ok = s->Parent == &prim && s->NextSubPrimitive == 0;
    if (!ok) { std::printf("00bba260_TEST FAIL\n"); return 1; }
    std::printf("00bba260_TEST PASS\n");
    return 0;
}