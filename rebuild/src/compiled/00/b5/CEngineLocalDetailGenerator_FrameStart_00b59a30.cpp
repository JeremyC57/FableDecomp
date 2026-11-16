// CEngineLocalDetailGenerator::FrameStart(const CEngineComponentParams&) -- when local detail is enabled: on off-world-tick frames update every loaded map's local-detail cache against the time budget (now + LocalDetailMaxUpdateTime), then render every cache from the engine camera, and remember the camera position
// Leaf: FrameStart
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B59A30, 205 bytes, `ret 4` (frame pointer, 8-byte aligned stack for the double).
//   Mangled ?FrameStart@CEngineLocalDetailGenerator@@UAEXABVCEngineComponentParams@@@Z (vtable 0x012A2588 slot 1).
// Callees: GFGetFastSubFrameTimer 0x009E1C70 (`__fastcall`, double), CLocalDetailCacheMap::Update(double) 0x00BDED90,
//   CLocalDetailCacheMap::Render(CEngineCamera*, float) 0x00BDDD70. Globals: the engine camera 0x01436EA0
//   (CameraDesc.WorldPosition +0x54, Ego_r.pdb agrees), the engine state 0x01436E8C (loaded-map list at +0x2c).
// Layout (retail, as the landed ctor 0x00B59B60 / LoadNewScene 0x00B599C0): OldCameraPos +0x18, LocalDetailMaxUpdateTime
//   +0x30, LocalDetailEnabled +0x34 (the PDB members sit 0x38 higher); CEngineComponentParams::OffWorldTickProcessFlag +0x7c.
// Shape: the landed LoadNewScene list walks (a pointer local to the list, the head re-read after each call, each node's map
//   +4 cache null-tested); the time budget is a named double local; the camera position a C3DVector local built by the
//   memberwise user copy ctor (direct offsets) and assigned to OldCameraPos at the end (implicit assignment: `add ebx,18h`);
//   the render loop reads the camera global into a local first thing in the body (retail loads it at the rotated latch,
//   before the cache test).
#include "engine/C3DVector.h"

class CEngineCamera { public:
    unsigned char _pad[0x54];
    C3DVector     WorldPosition;                       // +0x54 (CameraDesc.WorldPosition)
};
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                 // 0x01436EA0

double __fastcall GFGetFastSubFrameTimer();            // 0x009E1C70

class CLocalDetailCacheMap { public:
    bool Update(double time);                          // 0x00BDED90
    void Render(CEngineCamera* camera, float alpha);   // 0x00BDDD70
};

class CEngineMap { public:
    void*                 LandscapeMap;                // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;         // +0x04
};

// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A) and its +0x2C list the Ego_r.pdb member VisibleMapList
// (std::list<CEngineMap*>, engine/CEngineWorldMap.h overlay opt-in _VISIBLEMAPLIST); were the SEngineState / g_pEngineState,
// CMapList and SMapListNode stand-ins.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
#include "engine/CEngineWorldMap.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

class CEngineComponentParams { public:
    unsigned char _pad[0x7c];
    bool          OffWorldTickProcessFlag;             // +0x7c
};

class CEngineLocalDetailGenerator { public:
    virtual void FrameStart(const CEngineComponentParams& params);
    unsigned char _pad04[0x18 - 4];
    C3DVector     OldCameraPos;                        // +0x18
    unsigned char _pad24[0x30 - 0x24];
    float         LocalDetailMaxUpdateTime;            // +0x30
    bool          LocalDetailEnabled;                  // +0x34
    bool          DynamicUpdateEnabled;                // +0x35
};

void CEngineLocalDetailGenerator::FrameStart(const CEngineComponentParams& params)
{
    if (!LocalDetailEnabled)
        return;
    double endTime = GFGetFastSubFrameTimer() + LocalDetailMaxUpdateTime;
    C3DVector cameraPos = NEngineInternal::GPCamera->WorldPosition;
    std::list<CEngineMap*>* maps = &NEngineInternal::GPEngineWorldMap->VisibleMapList;
    std::list<CEngineMap*>::iterator it;
    if (params.OffWorldTickProcessFlag) {
        for (it = maps->begin(); it != maps->end(); ++it) {
            CLocalDetailCacheMap* cache = (*it)->LocalDetailCacheMap;
            if (cache)
                cache->Update(endTime);
        }
    }
    for (it = maps->begin(); it != maps->end(); ++it) {
        CEngineCamera* camera = NEngineInternal::GPCamera;
        CLocalDetailCacheMap* cache = (*it)->LocalDetailCacheMap;
        if (cache)
            cache->Render(camera, 0.0f);
    }
    OldCameraPos = cameraPos;
}