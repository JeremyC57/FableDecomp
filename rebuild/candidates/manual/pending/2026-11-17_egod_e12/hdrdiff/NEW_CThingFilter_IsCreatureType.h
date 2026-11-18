#ifndef FABLE_ENGINE_CTHINGFILTER_ISCREATURETYPE_H
#define FABLE_ENGINE_CTHINGFILTER_ISCREATURETYPE_H
// New header (egod e12, row 0x00985440 CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate): the Ego_d.pdb class record
// (types.txt 0x60252: public std::unary_function<CThing,bool>; public ctor (ECreatureType type) and bool operator()(const CThingCreatureBase&
// test_thing) const -- Ego_d.pdb ??RCThingFilter_IsCreatureType@@QBE_NABVCThingCreatureBase@@@Z, arglist 0x22FFD, parameter names from its
// S_BPREL32 records; the landed GetPFirstThing row 0x00986E30 cites the same Ego_r spelling); private ECreatureType Type +0; sizeof 4 -- Ego_r.pdb struct_layouts_egor.tsv agrees). The ctor is a header inline (Ego_d emits it in
// the user's object, ai_state_group_opinion_reaction_root.obj, 24 bytes /Od); retail stores the type straight into the filter object
// (0x009854C0 `mov dword ptr [esp+20h],1`). operator() is a header inline too (Ego_d emits it in the same object, 33 bytes /Od;
// retail expands it in the GetPFirstThing<CFilter_Bind3<IsNot, IsCreatureType, In2DRange>, const CThingAICreature> COMDAT 0x00986E30:
// `call 0x00662000 (CThingCreatureBase::GetCreatureType) ; cmp eax,[edi]`). Needs engine/CThingCreatureBase.h first.
#include "stlport/fable_containers.h"
#include "engine/ECreatureType.h"
#include "engine/CThingCreatureBase.h"
class CThingFilter_IsCreatureType : public std::unary_function<CThing,bool> {
public:
 CThingFilter_IsCreatureType(ECreatureType type):Type(type){}
 bool operator()(const CThingCreatureBase& test_thing) const { return test_thing.GetCreatureType() == Type; }
private:
 ECreatureType Type;
};
#endif
