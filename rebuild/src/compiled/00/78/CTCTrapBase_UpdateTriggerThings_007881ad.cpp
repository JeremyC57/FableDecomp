// CTCTrapBase::UpdateTriggerThings() -- refresh the trap's trigger list: every creature able to be engaged in combat inside the box of the trap def's trigger radius around the trap
// Leaf: ?UpdateTriggerThings@CTCTrapBase@@IAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007881AD, 124 bytes, __fastcall, `leave ; ret` (frame-pointer size TU:
//   `optimize("y", off)` + `optimize("s", on)`, as the landed UpdateTriggerOnProximity 0x007887BE of the same class).
// Identity: Ego_r.pdb `?UpdateTriggerThings@CTCTrapBase@@IAEXXZ` (protected; donor 0x0078F26D); the landed FrameUpdate
//   0x007888B4 calls it. The catalog's "CTCCombatBase_UpdateEngageable" is a propagated label.
// Callees: STLport vector<CIntelligentPointer<CThing> >::erase(first, last) 0x004AC0F0 (inline clear()), CThing::GetPos()
//   const 0x004C73D0 (Ego_r.pdb), __ftol2 0x00BFEA70 (x3), C2DBoxI::C2DBoxI(const C2DCoordI&, long) 0x00434740
//   (FableWin.pdb), CTCBase::PeekThingSearchTools() const 0x00686D90 (Ego_r.pdb IBE;
//   retail keeps the PDB pair order: GetThingSearchTools 0x00686D80 (landed 0x007EE5A0 binds it), Peek 0x00686D90),
//   CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CIntelligentPointer<CThing> >,
//   CFilter_Bind2<CThingFilter_None, CThingFilter_IsThingAbleToBeEngagedInCombat> > 0x0078954D (Ego_r.pdb donor 0x0079051B).
// Shape: the centre is a `C2DCoordI((long)pos.X, (long)pos.Y)` TEMPORARY written inside the call expression. VC7.1 builds
//   it before the outer argument pushes (frame [ebp-18h]), X first, and defers the X store past the `fld [ebx+4]` of Y --
//   the shape of the landed NCreatureHiding::IsThemeHidable 0x00749D20. A named C2DCoordI local (any ctor form) stores X
//   straight after its `__ftol2` (or truncates Y first through the (long, long) ctor). The filter pair is declared before
//   the def pointer copy (that order sets the [ebp-10h]/[ebp-0Ch]/[ebp-8] slots); the radius is truncated inside the call.
// Layout: local model -- engine/CTCTrapBase.h carries Ego_r (Dinkumware 16-byte vector) offsets; retail uses 12-byte
//   STLport vectors (TriggerThings +0x2c, PTrapDef +0x54, as the landed 0x007887BE / ctor 0x0078918D); retail CTrapDef
//   TriggerRadius +0x3c.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/C3DVector.h"

class CThing {
public:
    const C3DVector& GetPos() const;                     // 0x004C73D0
};
class CThingCreatureBase : public CThing { };

class C2DBoxI {
public:
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI(const C2DCoordI& centre, long radius);       // 0x00434740
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                           // STLport 4.6: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* erase(_Tp* __first, _Tp* __last);               // 0x004AC0F0 for CIntelligentPointer<CThing>
    void clear() { erase(_M_start, _M_finish); }
};
}

class CThingFilter_None { };
class CThingFilter_IsThingAbleToBeEngagedInCombat { };
template <class F1, class F2> class CFilter_Bind2 {
public:
    const F1* F1_;
    const F2* F2_;
    CFilter_Bind2(const F1& a, const F2& b) : F1_(&a), F2_(&b) { }
};

class CThingSearchTools {
public:
    template <class T, class TVector, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& things) const;   // 0x0078954D
};

class CTrapDef {
public:
    unsigned char _pad[0x3c];
    float         TriggerRadius;                         // +0x3c
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90
};

class CTCTrapBase : public CTCBase {
public:
    unsigned char _pad_0x0c[0x20];
    std::vector<CIntelligentPointer<CThing> > TriggerThings;   // +0x2c
    unsigned char       TrapOpener[0x1c];                // +0x38 CFadeInFadeOutBase
    const CTrapDef*     PTrapDef;                        // +0x54 CDefPointer<const CTrapDef>
protected:
    void UpdateTriggerThings();
};

void CTCTrapBase::UpdateTriggerThings()
{
    TriggerThings.clear();
    CThingFilter_None none;
    CThingFilter_IsThingAbleToBeEngagedInCombat able;
    CFilter_Bind2<CThingFilter_None, CThingFilter_IsThingAbleToBeEngagedInCombat> filter(none, able);
    const CTrapDef* def = PTrapDef;
    const C3DVector& pos = ParentThing->GetPos();
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
        C2DBoxI(C2DCoordI((long)pos.X, (long)pos.Y), (long)def->TriggerRadius), filter, TriggerThings);
}