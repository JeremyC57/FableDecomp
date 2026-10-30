// NScript::CPredicate_And<CIsThingAlive, CIsVariableFalse>::`scalar deleting destructor'(unsigned int flags) -- the And's vtable, Right (the bool watcher, inline: back to the predicate base's vtable) then Left destroyed, the predicate base's vtable, then free this when bit 0 of flags is set
// Leaf: ??_G?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7DFE0, 52 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): slot 2 of the RTTI-less predicate vtable 0x012EA430 (operator() 0x00E7DE30, Clone 0x00E7E060), which
//   the landed CPredicate_And<CIsThingAlive, CIsVariableFalse> constructor 0x00E7E020 stores (hand501: Ego_r.pdb Clone
//   ?Clone@?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@UBEPAVCPredicateBase@2@XZ). One
//   `test [esp+8],1` = the scalar deleting destructor (??_G). Label correction: the manifest row
//   "CLandscapeBackgroundSubNodeSplitBranch::`scalar_deleting_destructor'" is propagated.
// Callees: ~CIsThingAlive 0x00E241B0 (Left), operator delete 0x00BFE9BC.
// Layout (Ego_r.pdb): CPredicate_And<P1, P2> {vptr; Left +4 (CIsThingAlive, 0x10 bytes); Right +0x14 (CIsVariableFalse
//   {vptr 0x012EA43C; const bool* Variable})}.
// Shape: the landed CPredicate_And<CIsThingAlive, CIsThingAlive> ??_G 0x00E245A0 model; CIsVariableFalse's destructor is
//   a user-written inline one (its out-of-line copy is 0x00E7DE10), so Right's destruction folds to CPredicateBase's
//   vtable store 0x012C2FF4 at +0x14 and the And's own vtable store is scheduled first; with an implicit destructor VC7.1
//   computes `lea ecx,[esi+4]` before the vtable store.
void __cdecl operator delete(void* block);              // 0x00BFE9BC
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
namespace NScript {
class CIsVariableFalse : public CPredicateBase {        // 8 bytes (Ego_r), vtable 0x012EA43C
public:
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual ~CIsVariableFalse() { }                     // inline here (out-of-line copy 0x00E7DE10)
    virtual bool operator()() const;                    // 0x00E7DDD0
    virtual CPredicateBase* Clone() const;              // 0x00E7DDE0
    const bool* Variable;                               // +0x04
};
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    CPredicate_And(const L& left, const R& right) : Left(left), Right(right) { }
    virtual ~CPredicate_And() { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    L Left;                                             // +0x04
    R Right;                                            // +0x14
};
template class CPredicate_And<CIsThingAlive, CIsVariableFalse>;
}