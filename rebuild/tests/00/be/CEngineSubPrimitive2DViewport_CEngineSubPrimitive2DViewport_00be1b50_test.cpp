// behaviour test for 0x00BE1B50 CEngineSubPrimitive2DViewport::CEngineSubPrimitive2DViewport(CEngineInternalPrimitive2DViewport*) (model copy of the member initialisation)
#include <cstdio>
#include <cstring>
#include <new>
class CEngineInternalPrimitiveBase { public: int id; };
class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase { };
class CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; void* Manager;
};
class CEngineSubPrimitive2DViewport : public CEngineSubPrimitiveBase { public: CEngineSubPrimitive2DViewport(CEngineInternalPrimitive2DViewport* parent) : CEngineSubPrimitiveBase(parent) { } };
int main()
{
    CEngineInternalPrimitive2DViewport prim;
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CEngineSubPrimitive2DViewport* s = new (raw) CEngineSubPrimitive2DViewport(&prim);
    int ok = s->Parent == &prim && s->NextSubPrimitive == 0;
    if (!ok) { std::printf("00be1b50_TEST FAIL\n"); return 1; }
    std::printf("00be1b50_TEST PASS\n");
    return 0;
}