// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::~CModeIceTrollGroundSmashIcicleAssault() -- implicit ice-troll icicle-assault creature-mode destructor: destroy ViablePoints, Icicles, PDef, Filter and PTarget, then ~CModeStandBase
// Leaf: ??1CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743FE0, int3 padding before and after, thiscall, tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): (1) its sole caller is the scalar deleting destructor 0x00743FC0, slot 0 of vtable 0x01268774
//   (RTTI .?AVCModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@ : CModeStandBase : CModeBase); (2) Ego_r.pdb 0x00756A20 /
//   FableWin.pdb 0x024BAA60 ??1CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAE@XZ. No vtable store: compiler-generated.
//   The catalog label "CNewFrontendGameComponent::~CNewFrontendGameComponent" is wrong.
// Callees: free 0x00BFEA14 (ViablePoints storage), ~vector<CIcicleData> out of line (VC7.1 does not inline it, as retail; this TU's
//   COMDAT also matches 0x00743A90), CAnimFilterBase::~CAnimFilterBase inline (vptr store, CCharString Group dtor 0x0099EAE0),
//   ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (PTarget), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail; RTTI .?AVCModeStandBase@@, global, and both PDBs spell it so).
// Shape: members in reverse order; the PDB ctor (body not graded) is defined so the vtable, ??_G (RELOCATION_MATCH vs 0x00743FC0)
//   and this ??1 are emitted.
// Layout (NF247, q12682 header port): Ego_r.pdb members PTarget +0x68 .. ViablePoints +0x90 (STLport vectors 12 bytes), retail sizeof 0x9C,
//   on CTCCreatureModeManager.h (FABLE_RETAIL_CMODEBASE_VIRTUALS; it brings stlport/fable_containers.h for std::vector), CModeStandBase.h
//   (FABLE_RETAIL_CMODESTANDBASE + PASS22_VTABLE), CAnimFilterBase.h / CAnimFilterOfGroup.h (INLINE_CTOR: the implicit, inline
//   ~CAnimFilterOfGroup), CIntelligentPointer.h, CDefPointer.h, C3DVector.h. NCreatureMode::CModeIceTrollGroundSmashIcicleAssault has no header
//   anywhere under rebuild/include: it stays this row's local class (HEAD token) on the shared CModeStandBase base.
// Vtable 0x01268774 (NF247, q12682): slots 3 / 4 / 5 / 6 now bind the class's own overrides (retail 0x008DE7A0 / 0x00743CA0 / 0x008D7630 /
//   0x008D9650; Ego_r.pdb / FableWin.pdb / Ego_d.pdb), not the CModeBase names.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE                  // NF242 (q12677): CModeStandBase's slot 2 / 7 / 9 overrides (0x00862EC0 / 0x00861900 / 0x0073DA00)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING                            // GetAsCharArray / GetLength for the inline CAnimFilterOfGroup ctor
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"

class CThing;
class CSnowTrollDef;
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    class CIcicleData {
    public:
        CIcicleData();
        CIntelligentPointer<CThing> PEffect;                // +0x00
        CIntelligentPointer<CThing> PTrap;                  // +0x08
        long                        EffectTimer;            // +0x10
        long                        TrapTimer;              // +0x14
        C3DVector                   Position;               // +0x18
    };
    CModeIceTrollGroundSmashIcicleAssault(CThing& thing);
    virtual unsigned long GetType() const;
    // NF242 (q12677) slots 2 / 11 (retail 0x008D6850 / 0x00743CB0) and NF247 (q12682) slots 3-6 (retail 0x008DE7A0 / 0x00743CA0 / 0x008D7630 /
    // 0x008D9650): the class's own overrides of vtable 0x01268774, each spelled ?<name>@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@ in
    // Ego_r.pdb / FableWin.pdb / Ego_d.pdb. Declarations only.
    virtual void AnimUpdate(CAnimateThing& animateThing);
    virtual void FrameUpdate();
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual bool IsRemoveOnDeactivate() const;
    CIntelligentPointer<CThing>        PTarget;             // +0x68
    CAnimFilterOfGroup                 Filter;              // +0x70
    long                               Timer;               // +0x7c
    CDefPointer<const CSnowTrollDef>   PDef;                // +0x80
    std::vector<CIcicleData>           Icicles;             // +0x84
    std::vector<C3DVector>             ViablePoints;        // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CModeIceTrollGroundSmashIcicleAssault) == 0x9c);

CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())
{
}
}
