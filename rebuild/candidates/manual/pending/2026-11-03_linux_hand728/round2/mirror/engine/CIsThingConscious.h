#ifndef FABLE_ENGINE_CISTHINGCONSCIOUS_H
#define FABLE_ENGINE_CISTHINGCONSCIOUS_H
// Retail model (hand-written 2026-10-30, wave 5 class-model migration): NScript::CIsThingConscious : CPredicateBase (Ego_r.pdb
// 0x10: Thing +0x04; the CPredicate_And<CIsThingConscious,...> layouts put it at 0x10 bytes too). No RTTI; vtable 0x012C2FE8
// (the ctor stores it: Clone 0x00CDDAE0 `mov [esi],0x012C2FE8` before the CScriptThing copy 0x004ABE90 into +4): 0 operator()
// 0x00CDDAB0 (Thing slot 75 IsAlive() `call [eax+12Ch]`, then slot 61 IsUnconscious() `call [edx+0F4h]`, true when alive
// and not unconscious), 1 Clone 0x00CDDAE0 (operator new 0x10 + inline copy), 2 scalar deleting dtor 0x00CDDB10;
// ~CIsThingConscious 0x00EF7AB0 (vtable store, the CScriptThing dtor expanded inline, then the CPredicateBase store).
// Names: Ego_r.pdb + FableWin.pdb ??RCIsThingConscious@NScript@@UBE_NXZ, ?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ,
// ??1CIsThingConscious@NScript@@UAE@XZ; the ctor ??0CIsThingConscious@NScript@@QAE@ABVCScriptThing@@@Z and the copy ctor
// ??0..@QAE@ABV01@@Z are FableWin-only (retail expands them inline: a row that needs the inline body defines it out of class
// after the include). Same shape as engine/CIsThingAlive.h (vtable 0x012C32F0).
// Contract: engine/CScriptThing.h needs CCountedPointer<T> defined before the include.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CIsThingConscious.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CPredicateBase.h"
#include "engine/CScriptThing.h"

namespace NScript {
class CIsThingConscious : public CPredicateBase {
public:
    CIsThingConscious(const CScriptThing&);
    virtual bool operator()() const;                                     // slot 0  0x00CDDAB0
    virtual CPredicateBase* Clone() const;                               // slot 1  0x00CDDAE0
    virtual ~CIsThingConscious();                                        // slot 2  0x00CDDB10 (scalar deleting); 0x00EF7AB0
    CScriptThing Thing;                                                  // +0x04
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CIsThingConscious) == 0x10);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CISTHINGCONSCIOUS_H
