// CEnginePrimitiveRenderer2D::~CEnginePrimitiveRenderer2D() -- clear primitive chains, release counted managers, then the manager array and CEngineComponent state; retail 0x00b4aaa0 is 272 bytes.
// Leaf: ??1CEnginePrimitiveRenderer2D
// Retail calls: ClearPrimitiveList @ 0x00B8FEE0; CCPPointerInfo::DeleteFunc and operator delete @ 0x00BFE9BC; _Destroy_range @ 0x00B38930; free @ 0x00BFEA14; CEngineComponent::~CEngineComponent @ 0x00B59730.
// The real generated donor headers are included as *_PDB names. Their CArray footprint is 0x10 bytes, but retail stores its three native pointers at +0x0c..+0x14 (0x0c bytes), moving manager pairs/list heads four bytes earlier. The class below is this explicit retail projection. The renderer and component use genuine virtual destructors; VC7.1 supplies the destructor-phase vptr stores.
#include <stddef.h>

#define CEnginePrimitiveRenderer2D CEnginePrimitiveRenderer2D_PDB
#include "engine/CEnginePrimitiveRenderer2D.h"
#undef CEnginePrimitiveRenderer2D

#define CEngineComponent CEngineComponent_PDB
#include "engine/CEngineComponent.h"
#undef CEngineComponent

#include "engine/CEngineInternalPrimitiveBase.h"

class CEnginePrimitiveManager;
class CEngineComponent;
class CEnginePrimitiveManager2DStaticMeshes;
class CEnginePrimitiveManager2DAnimatedMeshes;
class CEnginePrimitiveManager2DSprites;
class CEnginePrimitive2DViewportManager;
class CEnginePrimitiveManager2DParticleGroup;

class CCPPointerInfo { public:
    unsigned int RefCount;
    void (FABLE_FASTCALL *DeleteFunc)(void*);
    void* Data;
};

extern void FABLE_CDECL FableOperatorDelete(void* pointer); // 0x00BFE9BC
extern void FABLE_CDECL FableFree(void* pointer);           // 0x00BFEA14

template<class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;

    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                FableOperatorDelete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

extern void FABLE_FASTCALL ClearPrimitiveList(CEngineInternalPrimitiveBase* first); // 0x00B8FEE0

struct EmptyRangeTag {};
extern void FABLE_FASTCALL DestroyManagerRange(
    CCountedPointer<CEnginePrimitiveManager>* first,
    CCountedPointer<CEnginePrimitiveManager>* last,
    EmptyRangeTag* tag); // 0x00B38930

struct CManagerPointerArray {
    CCountedPointer<CEnginePrimitiveManager>* Start;
    CCountedPointer<CEnginePrimitiveManager>* Finish;
    CCountedPointer<CEnginePrimitiveManager>* EndOfStorage;

    ~CManagerPointerArray()
    {
        EmptyRangeTag tag;
        DestroyManagerRange(Start, Finish, &tag);
        if (Start != 0)
            FableFree(Start);
    }
};

class CEngineComponent { public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    unsigned char Padding[2];
};

class CEnginePrimitiveRenderer2D : public CEngineComponent { public:
    CManagerPointerArray PrimitiveManagers;                     // native +0x0c, extent 0x0c
    CCountedPointer<CEnginePrimitiveManager2DStaticMeshes> StaticMesh2DManager; // +0x18
    CCountedPointer<CEnginePrimitiveManager2DAnimatedMeshes> AnimatedMesh2DManager; // +0x20
    CCountedPointer<CEnginePrimitiveManager2DSprites> Sprite2DManager; // +0x28
    CCountedPointer<CEnginePrimitive2DViewportManager> Viewport2DManager; // +0x30
    CCountedPointer<CEnginePrimitiveManager2DParticleGroup> Particle2DManager; // +0x38
    CEngineInternalPrimitiveBase* First2DPrim;                  // native +0x40
    CEngineInternalPrimitiveBase* First2DDisplacementPrim;       // native +0x44

    virtual ~CEnginePrimitiveRenderer2D();
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CEngineComponent) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveRenderer2D, PrimitiveManagers) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveRenderer2D, StaticMesh2DManager) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveRenderer2D, Particle2DManager) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveRenderer2D, First2DPrim) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveRenderer2D, First2DDisplacementPrim) == 0x44);
#endif

CEnginePrimitiveRenderer2D::~CEnginePrimitiveRenderer2D()
{
    ClearPrimitiveList(First2DPrim);
    ClearPrimitiveList(First2DDisplacementPrim);
}