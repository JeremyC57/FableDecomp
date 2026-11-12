#ifndef FABLE_ENGINE_CTCSHAPEMANAGER_H
#define FABLE_ENGINE_CTCSHAPEMANAGER_H
#include "engine/CTCBase.h"
#include "engine/C2DBoxF.h"
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES
// Retail opt-in (wf3, row 0x00761980 CTCShapeManager::DrawShapes): the typed shape containers and the private DrawShapes declaration.
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace NShape { class CShape; }
class CEnginePrimitivePrepareInfo;
#endif
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_TRACK
namespace NShape { class CTrack; }
#endif
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_GETINSTANTIATIONFROMENUM
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace NShape { class CShape; enum EShape; }
#endif
#endif
// Ego_r TPI0x2cd5b; original CTCBase and vector reduction put complete native extent at30.
class CTCShapeManager : public CTCBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    bool IsPosInAnyShape(const C3DVector&) const;
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_GETPRINCIPALVECTOR
    // Retail opt-in (wf3, row 0x00761980 evidence probe): ?GetPrincipalVector@CTCShapeManager@@QBE_NAAVC3DVector@@@Z 0x00760910
    // (Ego_r.pdb 0x00772130 + FableWin.pdb). Declaration only: no layout change.
    bool GetPrincipalVector(C3DVector&) const;   // 0x00760910
#endif
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER
    // Retail opt-in (chunk 728, row 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator): the inline static type query every
    // CTC defines (StatGetInterfaceType, no out-of-line copy for this class in either PDB); retail GetInterfaceType returns 0x44 and
    // GetTC<CTCShapeManager> tests UsingComponents bit 0x44 (`test byte ptr [eax+28h],10h`) and compares the found key with 44h.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHAPE_MANAGER; }
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_TRACK
    // Retail opt-in (chunk 754, row 0x0087B840 NCameraModes::CTrack::OnInitLeaf): Ego_r.pdb
    // ?GetBestTrack@CTCShapeManager@@QBE_NPAPBVCTrack@NShape@@@Z, out of line at 0x00761B50. Declaration only: no layout change.
    bool GetBestTrack(const NShape::CTrack** track) const;   // 0x00761B50
#endif
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_GETINSTANTIATIONFROMENUM
    // Nested opt-in (agentNF27, 2026-11-12, row 0x007624E0): Ego_r.pdb (0x00773560, right after the CTrack / CBasicShape ctors
    // 0x00772F80 / 0x00773480 = retail 0x00761F40 / 0x00762440) + FableWin.pdb
    // ?GetInstantiationFromEnum@CTCShapeManager@@QAE?AV?$CCountedPointer@VCShape@NShape@@@@W4EShape@NShape@@AAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
    // (by-value counted pointer through the hidden result, `ret 0Ch`). Declaration only: no layout change.
    CCountedPointer<NShape::CShape> GetInstantiationFromEnum(NShape::EShape type, std::vector<C3DVector>& points);   // 0x007624E0
#endif
#endif
private:
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES
    // Ego_r.pdb members PersistantShapes (vector<CCountedPointer<NShape::CShape> >, retail +0x0c) and SelectedShape (its iterator,
    // retail +0x18), typed; and ?DrawShapes@CTCShapeManager@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@@Z 0x00761980
    // (Ego_r.pdb 0x00772C30 + FableWin.pdb). Same layout as the opaque default (12 + 4 bytes).
    std::vector<CCountedPointer<NShape::CShape> > PersistantShapes;
    std::vector<CCountedPointer<NShape::CShape> >::iterator SelectedShape;
    void DrawShapes(const CEnginePrimitivePrepareInfo&, const C3DVector&);   // 0x00761980
#else
    unsigned char PersistantShapes[12]; // vector<CCountedPointer<NShape::CShape> >
    unsigned char SelectedShape[4]; // Original STLport vector iterator
#endif
    bool Activated,NewShapeActivated,LeavingShape;
    C2DBoxF ClosedShapeBounds;
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00765650
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x00765640
public:
    virtual void EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &);  // slot 28, 0x0075FFE0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCShapeManager)==0x30);
#endif
