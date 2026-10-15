// Grade: DIFFER(ours v retail) -- 152v152, x87 operand order only: retail squares the components Y, Z, then X (`fld st(1) ; fmul st(2) ; fld st(1) ; fmul st(2)`), the lane squares Z, Y, X (`fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3)`) for every source order tried: x+y+z, x+(y+z), y+z+x, and a by-value Magnitude(x, y, z) helper (the same source shape matches the sibling 0x00BBA340, whose retail order is Z, Y, X)
// CEnginePrimitiveManagerWeatherMask::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when enabled, queues the weather mask as a pool-allocated sub-primitive at the distance of its bounding-sphere centre from the camera (alpha 0xff, render flags 1); returns 2 (rendered), else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB6AB0, length 150 bytes (+ int3 padding).
// Callees: 0x00BEB540 CEngineSubPrimitiveBase::operator new (`__fastcall`, size 0x18 in ecx), 0x0099A2F0 CBase::CBase(),
// 0x00B846E0 CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase*, float, unsigned char,
// unsigned long, CEnginePrimitiveManager*) on the global renderer @ 0x01436E74; the camera global @ 0x01436EA0
// (world position at +0x54). Vtable 0x012A5704 = CEngineSubPrimitiveWeatherMask.
// Layout: CEnginePrimitiveManager::Enable at +8 (PDB); CEngineInternalPrimitiveWeatherMask::BSphere at +0x48 (PDB);
// CEngineSubPrimitiveBase (PDB, 0x18): Parent +0xc, NextSubPrimitive +0x10.
// Shape: the distance is computed BEFORE the allocation (a named float local, parked in the dead `primitive`
// argument slot across the operator new call), the sub-primitive's constructor chain is inlined as in 0x00BBA340.
#include <math.h>

struct C3DVector { float X, Y, Z; };

class CBase {
public:
    virtual ~CBase();
    CBase();                                       // 0x0099A2F0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class __declspec(novtable) CEngineSubPrimitiveBase : public CBase {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (sub-primitive pool)
};

struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54 (CameraDesc +0x14, its WorldPosition +0x40)
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

struct C3DBoundingSphere { C3DVector Center; float Radius; };
class CEngineInternalPrimitiveWeatherMask {         // CEngineInternalPrimitiveBase (0x48) + mask data (PDB)
public:
    unsigned char     _base[0x48];
    C3DBoundingSphere BSphere;                      // +0x48
};
class CEngineSubPrimitiveWeatherMask : public CEngineSubPrimitiveBase {   // vtable 0x012A5704
public:
    CEngineSubPrimitiveWeatherMask(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveWeatherMask();
};
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha,
                         unsigned long renderFlags, CEnginePrimitiveManager* manager);   // 0x00B846E0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerWeatherMask {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerWeatherMask::RenderPrimitive(CEngineInternalPrimitiveBase& primitive,
                                                              EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveWeatherMask& mask = (const CEngineInternalPrimitiveWeatherMask&)primitive;
    float dx = mask.BSphere.Center.X - g_pEngineCamera->WorldPosition.X;
    float dy = mask.BSphere.Center.Y - g_pEngineCamera->WorldPosition.Y;
    float dz = mask.BSphere.Center.Z - g_pEngineCamera->WorldPosition.Z;
    float distance = (float)sqrt(dx * dx + dy * dy + dz * dz);
    CEngineSubPrimitiveWeatherMask* subPrimitive = new CEngineSubPrimitiveWeatherMask(primitive);
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(subPrimitive, distance, 0xff, 1, (CEnginePrimitiveManager*)this);
    return RENDERED_SUB_PRIMITIVES;
}
