#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONGUARDREACTTOBODYFOUNDMESSAGE_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONGUARDREACTTOBODYFOUNDMESSAGE_H
// New header (egod e07, row 0x00911300 Update): Ego_r.pdb / Ego_d.pdb class with no own members (struct_layouts_egor.tsv sizeof 0x14 =
// CAIStateGroupBase). Ego_r.pdb members: ?Alloc@..@@SIPAVCAIStateGroupBase@@XZ, ?FindBodyFoundEvent@..@@AAEPBVCMessageEvent@@XZ
// (private, retail 0x009112A0), ?IsRunnable@..@@UAE_NXZ (0x009112F0), ?Update@..@@UAEXXZ (0x00911300). Declarations only. Needs
// engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS).
#include "rebuild_abi.h"
#include "engine/CAIStateGroupBase.h"
class CMessageEvent;
class CAIStateGroup_MinionGuardReactToBodyFoundMessage : public CAIStateGroupBase {
public:
    static CAIStateGroupBase* FABLE_FASTCALL Alloc();
    virtual bool IsRunnable();                                           // 0x009112F0
    virtual void Update();                                               // 0x00911300
private:
    const CMessageEvent* FindBodyFoundEvent();                           // 0x009112A0
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionGuardReactToBodyFoundMessage) == 0x14);
#endif
