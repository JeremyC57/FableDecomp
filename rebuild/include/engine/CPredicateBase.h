#ifndef FABLE_ENGINE_CPREDICATEBASE_H
#define FABLE_ENGINE_CPREDICATEBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): NScript::CPredicateBase, the abstract script
// predicate (Ego_r.pdb 4 bytes, no members). No RTTI; vtable 0x012C2FF4: 0 _purecall, 1 _purecall, 2 scalar deleting dtor
// 0x00CDDB40 (the dtor is inline: that body is the vtable store + conditional delete). The pure slots are named from every
// predicate's overrides: slot 0 `bool operator()() const` (Ego_r + FableWin ??RCIsThingAlive@NScript@@UBE_NXZ; CIsThingAlive
// slot 0 0x00CDF120 tail-jumps to CScriptThing::IsAlive), slot 1 `CPredicateBase* Clone() const` (?Clone@..@@UBEPAVCPredicateBase@2@XZ;
// CIsThingAlive slot 1 0x00CDF130 news 0x10 bytes). Declaration order = vtable order (dtor last).
// Ctor / copy ctor / dtor are inline: retail stores this vtable inline at every derived construction / destruction
// (0x00E24760, 0x00CDF160) and has no out-of-line instance (FableWin.pdb has ??0 / ??1 only as debug-build copies).
// FableWin.pdb also has the static ?InitialiseGameInterface@CPredicateBase@NScript@@SIXABVCGameScriptInterfaceBase@@@Z.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CPredicateBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#include "stlport/fable_containers.h"
#endif
class CGameScriptInterfaceBase;

namespace NScript {
// Opt-in FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (chunk H40, inline-budget device; same lever as the predicate filter rows in docs/journal/2026-10/2026-10-26_one_address_one_name_pass.md): the abstract base gets
// MSVC's `__declspec(novtable)`, so its constructor is empty and a derived predicate ctor compiled under a row-local
// `#pragma inline_depth(0)` region (nested CScriptThing copy 0x004ABE90 CALLED, as retail does at every predicate construction
// site) has no `call CPredicateBase::CPredicateBase` left (first witness 0x00E5B3B0). It also drops the base vtable store
// 0x012C2FF4 from expanded destructors: use it only in rows whose retail body has no such store. Other compilers see the plain class.
#if defined(FABLE_RETAIL_CPREDICATEBASE_NOVTABLE) && defined(_MSC_VER)
class __declspec(novtable) CPredicateBase
#else
class CPredicateBase
#endif
#ifdef FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
    : public std::unary_function<void,bool>
#endif
{
public:
    CPredicateBase() { }
    virtual bool operator()() const = 0;                                 // slot 0
    virtual CPredicateBase* Clone() const = 0;                           // slot 1
    virtual ~CPredicateBase() { }                                        // slot 2  0x00CDDB40 (scalar deleting)
    static void FABLE_FASTCALL InitialiseGameInterface(const CGameScriptInterfaceBase&);
#ifdef FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
protected:
    static const CGameScriptInterfaceBase* PGame; // Both complete original PDBs: protected static const pointee.
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CPredicateBase) == 0x4);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CPREDICATEBASE_H
