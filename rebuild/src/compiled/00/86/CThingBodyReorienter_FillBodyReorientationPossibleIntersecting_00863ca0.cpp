// CThingBodyReorienter::FillBodyReorientationPossibleIntersecting() -- start a reorientation check: gather every thing with a physics mesh in the square round the body (half-side = the body's physics diameter, at least 1) as the possible intersecting things; with none found the reorientation is done at once
// Leaf: ?FillBodyReorientationPossibleIntersecting@CThingBodyReorienter@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00863CA0, length 358 bytes, thiscall, plain `ret`. Name from both PDBs
//   (Ego_r.pdb 0x00832BD0, FableWin.pdb 0x02834200: private: void __fastcall ...(void)); only caller 0x00863E1F in the landed
//   CThingBodyReorienter::UpdateBodyReorienation 0x00863E10.
// Callees: CThing::GetPos() const 0x004C73D0; __ftol2 0x00BFEA70 (x3: pos.X, pos.Y, size);
//   CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CThingFilter_HasPhysicsMesh> 0x00863E70 (landed; the
//   instantiation retail compiled right after this TU's functions); CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90,
//   GetPItem 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10 (CIntelligentPointer<CThing>, vtable 0x01238C6C);
//   vector<CIntelligentPointer<CThing> >::_M_insert_overflow 0x004AC940 (the inline push_back's grow path); free 0x00BFEA14
//   (the local vector's storage).
// Data: 1.0f 0x0122DED8; CMainGameComponent::PMainGameComponent 0x013B86A0 (+0x24 world, +0x20 thing search tools).
// Layout: CThingBodyReorienter has no engine header; Ego_r.pdb members with the 12-byte STLport vector, as the landed siblings
//   0x00863280 / 0x00863790 / 0x00863E10 (StillCheckingForNewBodyReorientationAngle +0x08, HasPossibleIntersectingThings +0x14,
//   PossibleIntersectingThings +0x18, Body +0x24). CThingFilter_HasPhysicsMesh has no engine header either: an empty class
//   (Ego_r.pdb size 1) with FableWin.pdb's user constructor ??0CThingFilter_HasPhysicsMesh@@QAE@XZ and
//   ??RCThingFilter_HasPhysicsMesh@@QBE_NABVCThing@@@Z 0x006A6F50.
// Shape (why the bytes are what they are):
//   - the frame is 0x30 with `and esp,-8`: box (16) / things (12) / filter (its own word, because it is a NAMED local with a user
//     constructor: nothing is stored to it) / the 8-byte slot shared by the size float and the loop's CIntelligentPointer
//     temporary / push_back's __false_type tag byte;
//   - the box is a named local built with the radius constructor from `pos.ToC2DCoordI()` (FableWin.pdb keeps
//     ?ToC2DCoordI@C3DVector@@ and ??0C2DBoxI@@QAE@ABVC2DCoordI@@J@Z as neighbouring COMDATs): the by-value C2DCoordI keeps all
//     four corners in registers (edx / esi / ecx / edi) before the first store; C2DCoordI((long)pos.X, (long)pos.Y) spelled in
//     place stores each corner as it is computed (`lea ecx,[eax+edi]`), which was the last residue;
//   - the search template's body is visible (engine/CThingSearchTools.h FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY, the landed
//     0x00863E70 body kept out of line): VC7.1 then loads the local vector's start straight into the iterator register (ebp)
//     and re-materialises it for the destructor; declaration-only it goes through eax;
//   - GetThingSearchTools() is the private const inline accessor FableWin.pdb names
//     (?GetThingSearchTools@CThingBodyReorienter@@ABEAAVCThingSearchTools@@XZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "stlport/fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"

class CThingFilter_HasPhysicsMesh {
public:
    CThingFilter_HasPhysicsMesh() { }
    bool operator()(const CThing& thing) const;             // 0x006A6F50
};

class CThingBodyReorienter {
public:
    bool BodyReorientationDone;                             // +0x00
    bool ReorientingBody;                                   // +0x01
    long ReorientationCount;                                // +0x04
    bool StillCheckingForNewBodyReorientationAngle;         // +0x08
    bool HaveFinalReorientationAngle;                       // +0x09
    float FinalReorientationAngleXY;                        // +0x0C
    long RotationDirection;                                 // +0x10 ERotationDirection
    bool HasPossibleIntersectingThings;                     // +0x14
    std::vector<CIntelligentPointer<CThing> > PossibleIntersectingThings;   // +0x18
    CThing* Body;                                           // +0x24
private:
    CThingSearchTools& GetThingSearchTools() const { return CMainGameComponent::Get()->GetWorld().GetThingSearchTools(); }
    void FillBodyReorientationPossibleIntersecting();       // 0x00863CA0
};

void CThingBodyReorienter::FillBodyReorientationPossibleIntersecting()
{
    HasPossibleIntersectingThings = true;
    StillCheckingForNewBodyReorientationAngle = true;
    float size = Body->GetPhysicsTC()->GetDiameter();
    if (size < 1.0f)
        size = 1.0f;
    const C3DVector& pos = Body->GetPos();
    C2DBoxI box(pos.ToC2DCoordI(), (long)size);
    std::vector<CThing*> things;
    CThingFilter_HasPhysicsMesh filter;
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(box, filter, things);
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it)
        PossibleIntersectingThings.push_back(CIntelligentPointer<CThing>(*it));
    if (PossibleIntersectingThings.empty()) {
        BodyReorientationDone = true;
        StillCheckingForNewBodyReorientationAngle = false;
        HasPossibleIntersectingThings = false;
    }
}