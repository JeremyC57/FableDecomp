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
#ifdef FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_FRAMEUPDATE
#include "engine/CEnginePrimitiveHandle.h"
#endif
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
#ifdef FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_FRAMEUPDATE
    // Retail opt-in (wf7, row 0x007FD650 FrameUpdate): the three debug line handles typed (Ego_r.pdb line1 / line2 / line3,
    // CEnginePrimitiveHandle, 8 bytes each: same 0x18 bytes), the slot 9 override ?FrameUpdate@CTCDPhysicalObstruction@@UAEXXZ and
    // the private ?UpdateAdditionalImpulse@CTCDPhysicalObstruction@@AAEXXZ (0x007FD580). Declarations only, no layout change.
    CEnginePrimitiveHandle line1;                        // +0x38
    CEnginePrimitiveHandle line2;                        // +0x40
    CEnginePrimitiveHandle line3;                        // +0x48
    virtual void FrameUpdate();                          // slot 9 0x007FD650
private:
    void UpdateAdditionalImpulse();                      // 0x007FD580
public:
#else
    unsigned char Lines[0x18];                           // +0x38 line1, line2, line3 (CEnginePrimitiveHandle)
#endif
#ifdef FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004DF1C9
public:
#endif
#ifdef FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_DRIVERSLOTS
// agentFAM13 (2026-11-12): CTCDriverBase virtuals this driver overrides but this declaration lacks (TLC vtable slot 28 IsDriverType,
// slot 29 CreateAdditionalTCs, as engine/CTCDriverBase.h); spellings from the Ego_r / FableWin name sets. Declaration only.
public:
    virtual bool IsDriverType(long type) const;
#endif // FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_DRIVERSLOTS
};
FABLE_STATIC_ASSERT(sizeof(CTCDPhysicalObstruction) == 0x50);
#endif // FABLE_ENGINE_CTCDPHYSICALOBSTRUCTION_H
