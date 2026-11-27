#ifndef FABLE_ENGINE_CANIMFILTEROFGROUP_H
#define FABLE_ENGINE_CANIMFILTEROFGROUP_H
#include "engine/CAnimFilterBase.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "engine/CCRC.h"
#endif
class CAnimFilterOfGroup:public CAnimFilterBase {
public:
 virtual bool operator()(const CAnimationEntry&)const;
 virtual CAnimFilterBase* Clone()const;
#if defined(FABLE_RETAIL_CANIMFILTEROFGROUP_CTOR) && !defined(FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR)
 // NF147 opt-in (row 0x008A1A10 CGameScriptInterface::AddCreatureScriptedMode): the (const CCharString&) constructor called out of line,
 // FableWin.pdb 0x01DD33C0 ??0CAnimFilterOfGroup@@QAE@ABVCCharString@@@Z (retail 0x0073E8F0, its landed self row; landed 0x0073E9A0 binds it).
 // Declaration only; no layout change.
 CAnimFilterOfGroup(const CCharString& group);                         // 0x0073E8F0
#endif
#ifdef FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
 // Retail opt-in (wf7, row 0x00847DF0 CCreatureAction_UseLamp::Initialise): the (const CCharString&) constructor (out-of-line copy
 // 0x0073E8F0) and the destructor as retail expands them in place for a stack filter: vptr 0x01267568, Group default-constructed then
 // assigned, GroupCRC = the SOURCE string's CRC (CCharString::GetCRC, inline under FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE);
 // the destructor resets the vptr to CAnimFilterBase 0x0122FFCC after ~Group.
 CAnimFilterOfGroup(const CCharString& group)
 {
  Group = group;
#ifdef FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
  GroupCRC = group.GetCRC();
#else
  GroupCRC = CCRC::Calc(0, group.GetAsCharArray(), group.GetLength());
#endif
 }
 // no user-declared destructor: retail destroys a stack filter without re-storing this class's vptr (implicit destructor).
#else
 virtual ~CAnimFilterOfGroup();
#endif
#ifdef FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE
 // NF128 opt-in (row 0x008E9A30 NCreatureMode::CModeStrafe::OnActivate): FableWin.pdb ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z
 // (0x01DD3450; no Ego_r copy), inline: retail expands it in place (0x008E9A4A..0x008E9A80) (CCharString::operator= 0x0099EFB0, then the CRC of the
 // argument: length / NullChar select / CCRC::Calc 0x004014A0); its out-of-line copy is 0x0073DFF0 (landed with this leaf). Needs
 // FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE (GetCRC). No layout change.
 void SetCurrentGroupName(const CCharString& name) { Group = name; GroupCRC = name.GetCRC(); }
#endif
#ifdef FABLE_RETAIL_DRAGONBASE_DONOR_PROTECTED_PASS29
private:
#endif
 CCharString Group;
 unsigned long GroupCRC;
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CAnimFilterOfGroup)==12);
#endif
