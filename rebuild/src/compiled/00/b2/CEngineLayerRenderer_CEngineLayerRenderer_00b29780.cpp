// CEngineLayerRenderer::~CEngineLayerRenderer() -- store the own vtable, free Components's element block (the STLport vector destructor inline: trivial elements, then `if (_M_start) free(_M_start)`), then the CBaseClassNonCopyable destructor as a tail jump
// Leaf: ??1CEngineLayerRenderer@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29780, 33 bytes (+ int3 padding to 48), thiscall, tail `jmp` to the base destructor.
// Identity (proven): CEngineLayerRenderer::~CEngineLayerRenderer() (Ego_r.pdb / FableWin.pdb ??1CEngineLayerRenderer@@UAE@XZ); evidence: it stores the CEngineLayerRenderer vtable 0x012A0F04 first (RTTI .?AVCEngineLayerRenderer@@; bases CBaseClassNonCopyable <- CBaseClass) and tail-jumps to the parent's destructor
// Label: the manifest's "OnDie" is propagated across byte-identical bodies (family J023: six destructors of a class
//   that adds one STLport vector of trivially destroyed elements; one retail address is one class's destructor, named
//   from the vtable it stores first).
// Callees: free 0x00BFEA14, CBaseClassNonCopyable::~CBaseClassNonCopyable() 0x0099A300 (catalog-bound ??1CBaseClassNonCopyable@@MAE@XZ, PDB-confirmed).
// Layout: CEngineLayerRenderer (Ego_r.pdb, no STL member before) Layer +0x04, Flags +0x08, SceneFilterFlags +0x0C, Components (CArray<CEngineComponent*>) +0x10.
//   Local model: the generated engine headers for these classes carry a data-member vptr (and Dinkumware vector sizes).
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                           // 0x0099A300
};
class CEngineComponent;
enum EEngineRenderLayer { EEngineRenderLayer_Force32 = 0x7fffffff };
enum ERenderRenderLayerFlags { ERenderRenderLayerFlags_Force32 = 0x7fffffff };
class CEngineLayerRenderer : public CBaseClassNonCopyable {                              // RTTI: CEngineLayerRenderer <- CBaseClassNonCopyable <- CBaseClass
public:
    virtual ~CEngineLayerRenderer();
    EEngineRenderLayer      Layer;                              // +0x04
    ERenderRenderLayerFlags Flags;                              // +0x08
    unsigned long           SceneFilterFlags;                   // +0x0C
    CArray<CEngineComponent*> Components;                       // +0x10
};

CEngineLayerRenderer::~CEngineLayerRenderer()
{
}