// behaviour test for 0x00B98B10 CEngineSubPrimitiveBoundingBox::CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase*) (model copy of the member initialisation)
#include <cstdio>
#include <cstring>
#include <new>
class CEngineInternalPrimitiveBase { public: int id; };
class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase { };
class CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager;
};
class CEngineSubPrimitiveBoundingBox : public CEngineSubPrimitiveBase { public: CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase* parent) : CEngineSubPrimitiveBase(parent) { } };
int main()
{
    CEngineInternalPrimitiveOcclusionBase prim;
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CEngineSubPrimitiveBoundingBox* s = new (raw) CEngineSubPrimitiveBoundingBox(&prim);
    int ok = s->Parent == &prim && s->NextSubPrimitive == 0;
    if (!ok) { std::printf("00b98b10_TEST FAIL\n"); return 1; }
    std::printf("00b98b10_TEST PASS\n");
    return 0;
}