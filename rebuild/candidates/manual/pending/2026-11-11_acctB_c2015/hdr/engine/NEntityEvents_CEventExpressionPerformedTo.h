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
    virtual ~CEventBase();                                               // slot 0
    virtual EEventType GetType() const;                                  // slot 1
    virtual void OnAdd(const CTCEntityEvents& events);                   // slot 2
    virtual long GetLifeTime() const;                                    // slot 3
    virtual bool IsReadyToDie() const;                                   // slot 4
    long WorldFrame;                                                     // +0x04
};
#endif
class CEventExpressionPerformedTo : public CEventBase {
public:
    static EEventType __fastcall StatGetType() { return (EEventType)3; }
    CDefPointer<const CExpressionDef> GetPExpressionDef() const { return PExpressionDef; }
private:
    CIntelligentPointer<CThingCreatureBase> PerformedBy;                 // +0x08
    CDefPointer<const CExpressionDef> PExpressionDef;                    // +0x10
    long DurationInFrames;                                               // +0x14
    long ExtraData1;                                                     // +0x18
    long ExtraData2;                                                     // +0x1c
};
} // namespace NEntityEvents
#endif
