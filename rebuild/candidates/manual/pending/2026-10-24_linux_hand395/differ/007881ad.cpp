// CTCTrapBase::UpdateTriggerThings() -- refresh the trap's trigger list: every creature able to be engaged in combat inside the square of the trap def's trigger radius around the trap
// Grade: DIFFER(124v124) -- one scheduling slot: retail stores the X `__ftol2` result to the centre after issuing the
//   `fld [ebx+4]` for Y, ours stores it straight after the call; every other byte (frame, registers, filter/def slots) matches.
// Leaf: UpdateTriggerThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007881AD, 124 bytes, __thiscall, `leave ; ret` (frame-pointer size TU:
//   `optimize("y", off)` + `optimize("s", on)`, as the landed UpdateTriggerOnProximity 0x007887BE).
// Identity: Ego_r.pdb `?UpdateTriggerThings@CTCTrapBase@@IAEXXZ`; the landed FrameUpdate 0x007888B4 calls it. The catalog's
//   "CTCCombatBase_UpdateEngageable" is propagated.
// Callees: STLport vector<CIntelligentPointer<CThing> >::erase(first, last) 0x004AC0F0 (inline clear()), CThing::GetPosition
//   0x004C73D0, __ftol2 0x00BFEA70 (x3), C2DBoxI::C2DBoxI(const C2DCoordI&, long) 0x00434740, CTCBase::GetThingSearchTools
//   0x00686D90, CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CIntelligentPointer<CThing> >,
//   CFilter_Bind2<CThingFilter_None, CThingFilter_IsThingAbleToBeEngagedInCombat> > 0x0078954D (PDB donor 0x0079051B).
// Tried: temporaries for the filters (value-initialised: stosb zero stores), C2DCoordI(long, long) (Y first), out-of-line
//   C3DVector conversion, def/filter declaration orders (filter first fixes the slot order), two long locals (worse).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector { float X, Y, Z; };

class CThing {
public:
    const C3DVector& GetPosition() const;                // 0x004C73D0
};
class CThingCreatureBase : public CThing { };

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    __forceinline C2DCoordI(const C3DVector& v) : X((long)v.X), Y((long)v.Y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& centre, long radius);       // 0x00434740
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };

struct CTriggerThingVector {                             // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointer<CThing>* _M_start;
    CIntelligentPointer<CThing>* _M_finish;
    CIntelligentPointer<CThing>* _M_end_of_storage;
    CIntelligentPointer<CThing>* erase(CIntelligentPointer<CThing>* first,
                                       CIntelligentPointer<CThing>* last);   // 0x004AC0F0
    void clear() { erase(_M_start, _M_finish); }
};

class CThingFilter_None { };
class CThingFilter_IsThingAbleToBeEngagedInCombat { };
template <class F1, class F2> struct CFilter_Bind2 {
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
    CThingSearchTools* GetThingSearchTools();            // 0x00686D90
};

class CTCTrapBase : public CTCBase {
public:
    unsigned char       _pad_0x0c[0x20];
    CTriggerThingVector TriggerThings;                   // +0x2c
    unsigned char       TrapOpener[0x1c];                // +0x38 CFadeInFadeOutBase
    const CTrapDef*     PTrapDef;                        // +0x54 CDefPointer<const CTrapDef>
    void UpdateTriggerThings();
};

void CTCTrapBase::UpdateTriggerThings()
{
    TriggerThings.clear();
    CThingFilter_None none;
    CThingFilter_IsThingAbleToBeEngagedInCombat able;
    CFilter_Bind2<CThingFilter_None, CThingFilter_IsThingAbleToBeEngagedInCombat> filter(none, able);
    const CTrapDef* def = PTrapDef;
    C2DCoordI centre(ParentThing->GetPosition());
    GetThingSearchTools()->GetAllThingsInRoughArea<CThingCreatureBase>(C2DBoxI(centre, (long)def->TriggerRadius), filter,
                                                                        TriggerThings);
}
