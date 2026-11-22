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
#endif
