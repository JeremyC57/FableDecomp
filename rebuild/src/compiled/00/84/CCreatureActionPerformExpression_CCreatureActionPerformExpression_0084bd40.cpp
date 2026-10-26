// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression&) -- member-wise
//   copy: base action copy, the counted expression-def pointer, the intelligent target pointer and the expression scalars
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084BD40, 0xB4 bytes. Callees: CCreatureActionBase::CCreatureActionBase(const
//   CCreatureActionBase&) 0x0062EBC0 (manifest), CBaseIntelligentPointer ctor/GetPItem/SetPItem 0x00A01B10/0x00A01B50/
//   0x00A01B90. Vtable stores: 0x012743DC = CCreatureAction_PerformExpression, 0x01238C6C = CIntelligentPointer.
// Layout: PDB (Ego_r) names/types; the TLC CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so each retail offset is the
//   PDB offset - 0x0c. CDefPointer's pointee keeps its count at +0x04. Local model: no engine/ header exists for these.
class CBaseObject;
struct CBaseObjectPointer;
class CThing;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual ~CIntelligentPointer();
};
class CDefPointeeBase { public: virtual void VFunc00(); long RefCount; };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++((CDefPointeeBase*)Object)->RefCount; }
};
class CExpressionDef;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};
class CCreatureAction_PerformExpression : public CCreatureActionBase { public:
    CDefPointer<const CExpressionDef> PExpressionDef;   // +0xa8
    CIntelligentPointer<CThing> PPerformToThing;        // +0xac
    long ExpressionItemDefIndex;                        // +0xb4
    bool EmoteIconAdded;                                // +0xb8
    bool ReplacedAction;                                // +0xb9
    bool Extended;                                      // +0xba
    bool ButtonHeld;                                    // +0xbb
    unsigned long SoundEffectID;                        // +0xbc
    long ExtraData1;                                    // +0xc0
    long ExtraData2;                                    // +0xc4
    virtual ~CCreatureAction_PerformExpression();
    CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& other);
};
CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& o)
    : CCreatureActionBase(o), PExpressionDef(o.PExpressionDef), PPerformToThing(o.PPerformToThing),
      ExpressionItemDefIndex(o.ExpressionItemDefIndex), EmoteIconAdded(o.EmoteIconAdded), ReplacedAction(o.ReplacedAction),
      Extended(o.Extended), ButtonHeld(o.ButtonHeld), SoundEffectID(o.SoundEffectID), ExtraData1(o.ExtraData1),
      ExtraData2(o.ExtraData2)
{
}