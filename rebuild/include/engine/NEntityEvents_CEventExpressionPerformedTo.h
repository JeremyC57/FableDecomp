#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTEXPRESSIONPERFORMEDTO_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTEXPRESSIONPERFORMEDTO_H
// NEntityEvents::CEventExpressionPerformedTo (new, chunk 2015, row 0x0097FEA0). Layout: Ego_r.pdb struct_layouts_egor.tsv (32 bytes):
// CEventBase { vptr, WorldFrame }, PerformedBy (CIntelligentPointer<CThingCreatureBase>) +0x08, PExpressionDef
// (CDefPointer<const CExpressionDef>) +0x10, DurationInFrames +0x14, ExtraData1 +0x18, ExtraData2 +0x1C (no container member:
// retail offsets = PDB; retail reads PExpressionDef at +0x10, 0x009801BF). Names (FableWin.pdb): the inline accessor
// ?GetPExpressionDef@CEventExpressionPerformedTo@NEntityEvents@@QBE?AV?$CDefPointer@$$CBVCExpressionDef@@@@XZ (by value: the caller
// add-refs the copy, 0x009801C6) and ?StatGetType@CEventExpressionPerformedTo@NEntityEvents@@SI?AW4EEventType@2@XZ (inline: the
// `cmp eax,3` after the slot-1 GetType call, 0x009801BA). EEventType is a forward enum; 3 is this class's type number (read
// from the retail compare). Needs FABLE_RETAIL_CINTELLIGENTPOINTER / FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CThingCreatureBase;
class CExpressionDef;
class CTCEntityEvents;
namespace NEntityEvents {
enum EEventType;                                                         // MS forward enum
#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTBASE_DEFINED
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTBASE_DEFINED
// NEntityEvents::CEventBase (Ego_r.pdb: vptr +0, WorldFrame +4); slots 0..4 as the landed GetEvent rows (0x007AFB90) declare them.
class CEventBase {
public:
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
    CEventBase() : WorldFrame(0) {}
    virtual ~CEventBase() {}
    virtual EEventType GetType() const = 0;
#else
    virtual ~CEventBase();                                               // slot 0
    virtual EEventType GetType() const;                                  // slot 1
#endif
    virtual void OnAdd(const CTCEntityEvents& events);                   // slot 2
    virtual long GetLifeTime() const;                                    // slot 3
    virtual bool IsReadyToDie() const;                                   // slot 4
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
protected:
#endif
    long WorldFrame;                                                     // +0x04
};
#endif
class CEventExpressionPerformedTo : public CEventBase {
public:
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_EXPRESSION_PERFORMED_TO; }
#else
    static EEventType __fastcall StatGetType() { return (EEventType)3; }
#endif
    CDefPointer<const CExpressionDef> GetPExpressionDef() const { return PExpressionDef; }
#ifdef FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS
    // NF67 opt-in (round 7, rows 0x00921440 / 0x00966290): FableWin.pdb ?GetPPerformedBy@CEventExpressionPerformedTo@NEntityEvents@@QBEPAVCThingCreatureBase@@XZ,
    // ?GetExtraData1@..@@QBEJXZ, ?GetExtraData2@..@@QBEJXZ, ?GetDurationInFrames@..@@QBEJXZ, inline in retail (0x00921571 `lea ecx,[ebp+8]` +
    // GetPItem 0x00A01B50; 0x0092159E `mov eax,[ebp+18h]`; 0x009215B1 `mov eax,[ebp+1Ch]`). Bodies only (needs CThingCreatureBase complete
    // for the Get() cast); no layout change.
    CThingCreatureBase* GetPPerformedBy() const { return PerformedBy.Get(); }
    long GetDurationInFrames() const { return DurationInFrames; }
    long GetExtraData1() const { return ExtraData1; }
    long GetExtraData2() const { return ExtraData2; }
#endif
private:
    CIntelligentPointer<CThingCreatureBase> PerformedBy;                 // +0x08
    CDefPointer<const CExpressionDef> PExpressionDef;                    // +0x10
    long DurationInFrames;                                               // +0x14
    long ExtraData1;                                                     // +0x18
    long ExtraData2;                                                     // +0x1c
};
} // namespace NEntityEvents
#endif
