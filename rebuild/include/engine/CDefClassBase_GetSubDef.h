#ifndef FABLE_CDEFCLASSBASE_GETSUBDEF_H
#define FABLE_CDEFCLASSBASE_GETSUBDEF_H
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefPointer.h"
#if defined(FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE) && defined(_MSC_VER)
// Per-TU opt-in (chunk 1021, row 0x0066BC10 CTCSoundPlayer::GetPotentialCriteriaStrings): the same body kept out of line, as that
// TU calls the GetSubDef<CEntitySoundDef> COMDAT 0x0065F604. The visible body tells VC7.1 the callee only writes through the out
// pointer, so the caller keeps the def it reads back in a register (retail `mov edi,[esp+0Ch]` after the call, 0x0066BC38, then
// SoundIdentifier and the release through edi with no reload).
template<class T>__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* def)const {
#else
template<class T>bool CDefClassBase::GetSubDef(CDefPointer<const T>* def)const {
#endif
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(T::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(FABLE_RETAIL_CDEFCLASSBASE_EXPRESSION_SPEED_INSTANCE_PASS26) && defined(_MSC_VER)
#include "engine/CExpressionSubDef.h"
// Fidelity: scaffolding, not retail. The 005DA240 owner uses default /O2 /Oy /W3.
// VC7.1 defers the ordinary instantiation to the end of this size-optimized TU.
// This explicit specialization uses the same lookup and ends with optimize("", on).
#pragma optimize("t", on)
template<>
inline __declspec(noinline) bool CDefClassBase::GetSubDef<CExpressionSubDef>(CDefPointer<const CExpressionSubDef>* def) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CExpressionSubDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> base(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        def->SetDynamicCast(base);
        return true;
    }
    return false;
}
#pragma optimize("", on)
#endif // FABLE_RETAIL_CDEFCLASSBASE_EXPRESSION_SPEED_INSTANCE_PASS26
#ifdef FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
// Fidelity scaffolding: specialize the genuine template body in its size/frame-pointer owner scope.
#include "engine/CCreatureDef.h"
#pragma optimize("y",off)
#pragma optimize("s",on)
template<> inline __declspec(noinline)
bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CCreatureDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#pragma optimize("",on)
#endif
#ifdef FABLE_RETAIL_WAREHOUSE_MOVE_CRATES_PASS29
#include "engine/CCrateStackDef.h"
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CCrateStackDef>(CDefPointer<const CCrateStackDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CCrateStackDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif

#ifdef FABLE_RETAIL_SHOT_ONHIT_PASS29
// Inline-budget: native GetSubDef is called at this site. Its separately linked 162-byte body uses frame-pointer size optimization.
#include "engine/CThingMultiArrowShotDef.h"
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CThingMultiArrowShotDef>(CDefPointer<const CThingMultiArrowShotDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CThingMultiArrowShotDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif

#endif
#ifdef FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY
// Opt-in (NF357 2026-11-24; T3 nf340 0x0042B1A6). Fidelity: scaffolding, not retail: it is for link determinism and makes no claim about
// retail's text. The per-type size-body device of engine/CDefinitionManager.h (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_<T>_SIZE_BODY,
// NF349's GETDEF_<T>_SIZE_BODY) for a TU that takes the template body above; the same macro selects the CDefinitionManager.h twin in a
// FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY TU (a TU takes one body or the other, never both). A /O2 TU emits its own pick-any copy of
// ??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z (182 B), while retail's linker kept the copy of the owner
// row 0x0042B1A6 (162 B), which was built in a frame-pointer size TU and sets these same two pragmas itself
// (rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp: optimize("y", off) / optimize("s", on); no CompilerFlags entry,
// so /O2 /Oy /W3). VC7.1 takes a template instance's optimize mode at the end of the TU; an `inline` explicit specialization takes it
// here. The body is the template body above with T = CWeaponDef, token for token. The pair ends with optimize("", on), which restores the
// command-line default (VC7.1 has no push / pop for optimize), so this is valid only in a TU with no optimize pragma above this include.
// Like any explicit specialization it must precede the TU's first use of GetSubDef<T> (VC7.1 C2908).
#include "engine/CWeaponDef.h"
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CWeaponDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif
#ifdef FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY
// Opt-in (NF357 2026-11-24; T3 nf340 0x0042B0A2): the CAppearanceDef twin of the block above, same device, same conditions and same ordering
// rule. Fidelity: scaffolding, not retail. Owner row 0x0042B0A2 (162 B; rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCAppearanceDef_0042b0a2.cpp:
// optimize("y", off) / optimize("s", on); no CompilerFlags entry). The body is the template body above with T = CAppearanceDef, token for token.
#include "engine/CAppearanceDef.h"
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CAppearanceDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif
#ifdef FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY
// Opt-in (NF357 2026-11-24; T3 nf340 0x0065F604): the CEntitySoundDef twin of the block above, same device, same conditions and same ordering
// rule. Fidelity: scaffolding, not retail. Owner row 0x0065F604 (162 B; rebuild/src/compiled/00/65/CDefClassBase_GetSubDefCEntitySoundDef_0065f604.cpp:
// optimize("y", off) / optimize("s", on); no CompilerFlags entry). The body is the template body above with T = CEntitySoundDef, token for token.
#include "engine/CEntitySoundDef.h"
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(CEntitySoundDef::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif
#endif
