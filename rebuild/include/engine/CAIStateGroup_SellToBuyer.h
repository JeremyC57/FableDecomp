#ifndef FABLE_ENGINE_CAISTATEGROUP_SELLTOBUYER_H
#define FABLE_ENGINE_CAISTATEGROUP_SELLTOBUYER_H
// CAIStateGroup_SellToBuyer (new, chunk 2023): RTTI .?AVCAIStateGroup_SellToBuyer@@ : CAIStateGroup_WorkBase<CAIStateGroupBase> :
// CAIStateGroupBase (vtable 0x0128BF84: slot 9 Cleanup 0x0094BF50, 11 IsRunnable 0x0094C010, 12 Update 0x0094C050). Ego_r.pdb member
// PBuyer (CIntelligentPointer<CThingCreatureBase>) +0x14, sizeof 0x1C (struct_layouts_egor.tsv). Ego_r.pdb / FableWin.pdb spellings:
// ?Cleanup@CAIStateGroup_SellToBuyer@@UAEXXZ, ?IsRunnable@..@@UAE_NXZ, ?Update@..@@UAEXXZ (public virtual overrides),
// ?RefuseCustom@..@@AAEXXZ (0x0094BF60) and ?GetBuyersOrder@..@@AAEPBVCMessageEvent@@XZ (0x0094BFC0) private.
// Needs the retail CAIStateGroupBase (FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS) and the retail CIntelligentPointer.
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
class CMessageEvent;
#ifdef FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
class CCharString; class CAIStateGroupBase;
#endif
class CAIStateGroup_SellToBuyer : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
#ifdef FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
public: static CCharString FABLE_FASTCALL GetName(); static CAIStateGroupBase* FABLE_FASTCALL Alloc(); private:
#endif
public:
    virtual void Cleanup();                                              // slot 9  0x0094BF50
    virtual bool IsRunnable();                                           // slot 11 0x0094C010
    virtual void Update();                                               // slot 12 0x0094C050
private:
    void RefuseCustom();                                                 // 0x0094BF60
    const CMessageEvent* GetBuyersOrder();                               // 0x0094BFC0
    CIntelligentPointer<CThingCreatureBase> PBuyer;                      // +0x14
};
#endif
