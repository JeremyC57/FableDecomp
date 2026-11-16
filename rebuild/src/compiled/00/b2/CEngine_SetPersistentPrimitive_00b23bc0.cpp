// Leaf: ?CEngine_SetPersistentPrimitive@@YGXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z
class CEnginePrimitiveHandle { public:};
class CEnginePrimitive { public:};

struct CGlobalTarget {
    void RealSet(CEnginePrimitiveHandle* h, CEnginePrimitive* p, unsigned long x, int flag);
};

class CEnginePrimitiveRenderer;
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The members read here are modelled by the local CGlobalTarget view of that object (name debt: the shared
// engine/CEnginePrimitiveRenderer.h retail branch).

void __stdcall CEngine_SetPersistentPrimitive(CEnginePrimitiveHandle& h,
                                      CEnginePrimitive const& p,
                                      unsigned long x)
{
    ((CGlobalTarget*)NEngineInternal::GPPrimitiveRenderer)->RealSet(&h, (CEnginePrimitive*)&p, x, 0);
}