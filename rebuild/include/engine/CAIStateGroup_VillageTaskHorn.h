#ifndef FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKHORN_H
#define FABLE_ENGINE_CAISTATEGROUP_VILLAGETASKHORN_H
// New header (chunk Q11, row 0x00940960): CAIStateGroup_VillageTaskHorn, Ego_r.pdb (struct_layouts_egor.tsv, 32 bytes):
// CAIStateGroupBase 0x14, PTCVillageMember +0x14, PTask CIntelligentPointer<CVillageTaskHorn> +0x18. Members (Ego_r.pdb):
// ctor 0x0091CEB3, dtor 0x0091CED6, Alloc 0x0091E775, IsRunnable / Cleanup / Update / Initialise (the Update row is retail
// 0x00940960, vtable slot 12). Spelled as the sibling engine/CAIStateGroup_VillageTaskTorch.h.
#include "engine/CAIStateGroupBase.h"
class CTCVillageMember; class CVillageTaskHorn;
#ifdef FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
class CCharString; class CAIStateGroupBase;
#endif
class CAIStateGroup_VillageTaskHorn:public CAIStateGroupBase {
#ifdef FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
public: static CCharString FABLE_FASTCALL GetName(); private:
#endif
public:
 virtual void Initialise(CAIBrain & p0, CThingAICreature & p1, long p2, EScriptAIPriority p3, const CCharString & p4);
 virtual void Update();
 virtual bool IsRunnable();
 virtual void Cleanup();
 static CAIStateGroupBase * FABLE_FASTCALL Alloc();
private:
 CTCVillageMember * PTCVillageMember;
 CIntelligentPointer<CVillageTaskHorn> PTask;
public:
 CAIStateGroup_VillageTaskHorn();
 virtual ~CAIStateGroup_VillageTaskHorn();
};
FABLE_STATIC_ASSERT_AT(23, sizeof(CAIStateGroup_VillageTaskHorn)==32);
#endif
