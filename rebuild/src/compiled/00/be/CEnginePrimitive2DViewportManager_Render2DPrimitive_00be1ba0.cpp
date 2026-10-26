// CEnginePrimitive2DViewportManager::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, ESubPrimitiveType type, CEngineSubPrimitiveChain& chain) -- wraps the 2D viewport primitive in a new sub-primitive and queues it on the renderer at the depth of the primitive's 2D layer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE1BA0, length 98 bytes (112 with padding).
// Callees: the sub-primitive class `operator new` @ 0x00BEB540 (`__fastcall`, size 0x18 in ecx), CBase::CBase @ 0x0099A2F0,
// CEnginePrimitiveRenderer2D::Layer2DToDistance(EEngine2DLayer) @ 0x00B4A6A0 (float, on the 2D renderer @ 0x01436E7C),
// the renderer's sub-primitive queue call @ 0x00B84720 (on the primitive renderer @ 0x01436E74; manifest
// "Engine_AddClockVertex" is a gap-scan name). Vtable 0x012A7ED0 = CEngineSubPrimitive2DViewport (rtti-vtables.tsv).
// Shape: `new CEngineSubPrimitive2DViewport(primitive)` with the null-tested inline constructor (CBase call, primitive at
// +0xC, next link +0x10 = 0, then the vtable: the two stores before the derived vtable belong to an inlined
// `__declspec(novtable)` intermediate base -- as init-list members or body assignments of the derived class they sink
// below the vtable store); the queue call's arguments evaluate right to left: `this` (pushed before
// the layer call), the float distance (`push ecx ; fstp [esp]`), the sub-primitive, the chain -- the THIRD argument (the
// manifest lists the chain second; the read of [esp+20h] puts it last). `type` is unused (`ret 0Ch`).
struct CBase {
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
};

enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };
enum ESubPrimitiveType { SUB_PRIMITIVE_DEFAULT = 0 };

class CEngineInternalPrimitiveBase { public:
    unsigned char  _pad00[0x90];
    EEngine2DLayer m_layer;                        // +0x90
};

class CEngineSubPrimitiveChain;
class CEnginePrimitive2DViewportManager;

struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {   // intermediate base: owns the primitive link
    unsigned char                 _pad04[0x8];
    CEngineInternalPrimitiveBase* m_pPrimitive;    // +0x0C
    CEngineSubPrimitiveBase*      m_pNext;         // +0x10
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& primitive) : m_pPrimitive(&primitive), m_pNext(0) { }
};

struct CEngineSubPrimitive2DViewport : CEngineSubPrimitiveBase {   // vtable 0x012A7ED0, 0x18 bytes
    unsigned long m_unknown14;                     // +0x14
    virtual void VSlot0();
    CEngineSubPrimitive2DViewport(CEngineInternalPrimitiveBase& primitive) : CEngineSubPrimitiveBase(primitive) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540
};

class CEnginePrimitiveRenderer2D { public:
    float Layer2DToDistance(EEngine2DLayer layer); // 0x00B4A6A0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D_01436e7c;   // 0x01436E7C

class CEnginePrimitiveRenderer { public:
    void AddSubPrimitive(CEngineSubPrimitiveChain& chain, CEngineSubPrimitive2DViewport* subPrimitive, float distance,
                         CEnginePrimitive2DViewportManager* manager);   // 0x00B84720
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e74;       // 0x01436E74

class CEnginePrimitive2DViewportManager { public:
    void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, ESubPrimitiveType type, CEngineSubPrimitiveChain& chain);
};

void CEnginePrimitive2DViewportManager::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, ESubPrimitiveType type,
                                                          CEngineSubPrimitiveChain& chain)
{
    CEngineSubPrimitive2DViewport* subPrimitive = new CEngineSubPrimitive2DViewport(primitive);
    g_pPrimitiveRenderer_01436e74->AddSubPrimitive(
        chain, subPrimitive, g_pPrimitiveRenderer2D_01436e7c->Layer2DToDistance(primitive.m_layer), this);
}