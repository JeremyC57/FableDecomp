#ifndef FABLE_ENGINE_CTCDPHYSICALOBSTRUCTION_H
#define FABLE_ENGINE_CTCDPHYSICALOBSTRUCTION_H
// CTCDPhysicalObstruction (new, chunk 767): RTTI .?AVCTCDPhysicalObstruction@@ : CTCDriverBase : CTCBase (vtable 0x01241C0C,
// slot 6 OnInitialActivate 0x007FD3C0). Ego_r.pdb struct_layouts_egor.tsv (84 bytes): StartPos +0x0C, EndPos +0x18 (C3DVector),
// LifeTimer +0x24 (long), Radius +0x28, ThingsToIgnoreObstruction +0x2C (vector, retail 12 bytes), line1..3 (CEnginePrimitiveHandle)
// +0x38 / +0x40 / +0x48 in retail. Name set (Ego_r.pdb donor order = retail order): OnInitialActivate 0x007FD3C0, SetLifeTimeSeconds
// 0x007FD3D0, SetParams 0x007FD3F0, Construct 0x007FD430, Create 0x007FD4B0, UpdateAdditionalImpulse 0x007FD580, FrameUpdate
// 0x007FD650. Only what rows use is modelled.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/C3DVector.h"
class CTCDPhysicalObstruction : public CTCDriverBase {
public:
    virtual void OnInitialActivate();                    // slot 6 0x007FD3C0
#ifdef FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
    // Retail opt-in (chunk 796, row 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover): Ego_r.pdb
    // ?StatGetInterfaceType@CTCDPhysicalObstruction@@SI?AW4ETCInterfaceType@@XZ (inline: TCI_PHYSICAL_OBSTRUCTION 237, the
    // UsingComponents bit the caller tests before reading PTCDriver), ?Create@CTCDPhysicalObstruction@@SIPAVCThing@@ABVC3DVector@@0M@Z
    // (0x007FD4B0), ?SetLifeTimeSeconds@CTCDPhysicalObstruction@@QAEXM@Z (0x007FD3D0) and
    // ?SetThingToIgnoreObstruction@CTCDPhysicalObstruction@@QAEXPAVCThing@@@Z (0x007898A3). Declarations only.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PHYSICAL_OBSTRUCTION; }
    static CThing* __fastcall Create(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0
    void SetLifeTimeSeconds(float seconds);              // 0x007FD3D0
    void SetThingToIgnoreObstruction(CThing* thing);     // 0x007898A3
#endif
    C3DVector     StartPos;                              // +0x0c
    C3DVector     EndPos;                                // +0x18
    long          LifeTimer;                             // +0x24
    float         Radius;                                // +0x28
    unsigned char ThingsToIgnoreObstruction[0xc];        // +0x2c vector<CIntelligentPointer<CThing> >
    unsigned char Lines[0x18];                           // +0x38 line1, line2, line3 (CEnginePrimitiveHandle)
};
FABLE_STATIC_ASSERT(sizeof(CTCDPhysicalObstruction) == 0x50);
#endif // FABLE_ENGINE_CTCDPHYSICALOBSTRUCTION_H
