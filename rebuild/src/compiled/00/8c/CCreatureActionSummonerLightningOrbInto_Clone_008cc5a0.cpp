// CCreatureAction_SummonerLightningOrbInto::Clone() const -- `return new CCreatureAction_SummonerLightningOrbInto(*this);` with the implicit copy constructor inline: the CCombatActionBase copy, the own vtable store, then the POrb member's out-of-line copy constructor; null when the allocation fails
// Leaf: ?Clone@CCreatureAction_SummonerLightningOrbInto@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CC5A0, 65 bytes (+ int3 padding to 80), thiscall, plain `ret`, returns the copy.
// Identity (proven): CCreatureAction_SummonerLightningOrbInto::Clone() const (Ego_r.pdb ?Clone@CCreatureAction_SummonerLightningOrbInto@@UBEPAVCCreatureActionBase@@XZ); evidence: it is
//   slot 11 of the CCreatureAction_SummonerLightningOrbInto vtable 0x012830EC (RTTI .?AVCCreatureAction_SummonerLightningOrbInto@@), the same vtable it stores into the 0x11C-byte copy.
// Label: the manifest's "CCreatureAction_SummonerLightningOrbInto::Clone" is propagated across byte-identical bodies (family J017: six creature-action Clone
//   bodies with one out-of-line member copy; retail has no identical-code folding, so each address is one class's Clone).
// Callees: operator new(unsigned int) (import thunk 0x00BFEA1A), CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0
//   (??0CCombatActionBase@@QAE@ABV0@@Z, catalog-bound and in Ego_r.pdb), CIntelligentPointer<CThing>::CIntelligentPointer(const CIntelligentPointer&) 0x004A9730 (??0?$CIntelligentPointer@VCThing@@@@QAE@ABV0@@Z, Ego_r.pdb spelling; the catalog also binds the const-CThing and stand-in spellings here, but the vtable it stores has RTTI .?AV?$CIntelligentPointer@VCThing@@@@ (agent952 H044)).
// Layout: retail CCombatActionBase is 0x114 bytes (12-byte STLport containers; Ego_r.pdb 0x12C), so POrb (Ego_r.pdb's only own
//   member, CIntelligentPointer<CThing>) sits at +0x114 and the class is 0x11C bytes. Local model: the generated engine headers for these
//   classes carry the Ego_r (Dinkumware) layout and a data-member vptr, which cannot give the retail offsets or a vtable store.
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                 // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other);   // out of line
};
class CCreatureActionBase {                         // 0xA8 bytes in retail (vptr first)
public:
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
    virtual ~CCreatureActionBase();
    unsigned char _body[0xA8 - 4];
};
class CCombatActionBase : public CCreatureActionBase {   // 0x114 bytes in retail
public:
    CCombatActionBase(const CCombatActionBase& other);       // 0x0062F3B0
    unsigned char _body[0x114 - 0xA8];
};
class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase {
public:
    virtual ~CCreatureAction_SummonerLightningOrbInto();
    virtual CCreatureActionBase* Clone() const;     // slot 11
    CIntelligentPointer<CThing> POrb;   // +0x114
};

CCreatureActionBase* CCreatureAction_SummonerLightningOrbInto::Clone() const
{
    return new CCreatureAction_SummonerLightningOrbInto(*this);
}