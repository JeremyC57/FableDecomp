// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- store this class's vtable, free the four vertex arrays' blocks through the landscape memory manager in reverse member order, then the CBase destructor (tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC410, length 115 bytes.
// Leaf: ??1CPatchTesselationEdgeStrip
// Callees: CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90 (engine global 0x01436EA8 -> +0x6b0), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300.
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, vtable 0x012A8318; same model as the landed Reset 0x00BFC3B0).
// Shape: the four `if (Data) FreeVirtual(Data)` blocks are the CEdgeStipVertexArray members' own inline destructors,
// run by the compiler in reverse declaration order (+0x20, +0x18, +0x10, +8); the body of the strip's destructor is empty.
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8 (descriptive)

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};

template <class T>
class CEdgeStipVertexArray { public:
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    ~CEdgeStipVertexArray()
    {
        if (Data)
            g_pLandscapeRenderer->MemoryManager->FreeVirtual(Data);
    }
};

class CPatchTesselationEdgeStrip : public CBaseClassNonCopyable { public:        // vtable 0x012A8318, 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
}