// CCombatAction_GenericStrikeResponse::CCombatAction_GenericStrikeResponse(CThingCreatureBase& creature, const CThing* striker, const C3DVector& strikePos, const C3DVector& strikeDir) -- construct the CCombatAction_GenericStrikeResponseBase base with the same four arguments, then install this class's vtable
// Leaf: ??0CCombatAction_GenericStrikeResponse@@QAE@AAVCThingCreatureBase@@PBVCThing@@ABVC3DVector@@2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867990, 42 bytes, thiscall, returns this, `ret 10h`.
// Identity (proven): it stores vtable 0x012771FC (RTTI .?AVCCombatAction_GenericStrikeResponse@@, direct base CCombatAction_GenericStrikeResponseBase) after calling the base constructor 0x008678B0 with its four arguments; Ego_r.pdb ??0CCombatAction_GenericStrikeResponse@@QAE@AAVCThingCreatureBase@@PBVCThing@@ABVC3DVector@@2@Z; caller CThingCreatureBase::ReactToGenericStrike 0x00667450.
// Callee: CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase 0x008678B0.
// Shape (family L037): the four stack arguments re-pushed in reverse, base ctor call, `mov [esi], vtable`.
class CThingCreatureBase;
class CThing;
class C3DVector;
class CCombatAction_GenericStrikeResponseBase {                                              // (members not touched here)
public:
    CCombatAction_GenericStrikeResponseBase(CThingCreatureBase& creature, const CThing* striker, const C3DVector& strikePos, const C3DVector& strikeDir);   // 0x008678B0
    virtual ~CCombatAction_GenericStrikeResponseBase();
};
class CCombatAction_GenericStrikeResponse : public CCombatAction_GenericStrikeResponseBase {
public:
    CCombatAction_GenericStrikeResponse(CThingCreatureBase& creature, const CThing* striker, const C3DVector& strikePos, const C3DVector& strikeDir);
};
CCombatAction_GenericStrikeResponse::CCombatAction_GenericStrikeResponse(CThingCreatureBase& creature, const CThing* striker, const C3DVector& strikePos, const C3DVector& strikeDir)
    : CCombatAction_GenericStrikeResponseBase(creature, striker, strikePos, strikeDir)
{
}