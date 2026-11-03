#ifndef FABLE_ENGINE_CACTIVEENTITYSCRIPTBASE_H
#define FABLE_ENGINE_CACTIVEENTITYSCRIPTBASE_H
// Retail model (hand-written 2026-10-30, wave 8 class-model migration). No RTTI (vt[-1] == 0); vtable 0x012C3594, stored by
// the ctor 0x00CE1110 (`mov [esi],0x012C3594` after the inlined CSpawnedFuncBase ctor and its vtable 0x012C2F9C), 7 slots:
// 0 scalar deleting dtor 0x00CE1200, 1-3 the microthread's (RunSuspendableProcess 0x00A44880, SuspendSuspendableProcess
// 0x00A44840, Restart 0x00A447D0), 4 CASuspendableProcess::SuspendableProcess 0x00A44600 (not overridden here),
// 5 OnPredicateFail 0x00CE1090 (`mov ecx,[ecx+34h]; test; je; mov eax,[ecx]; jmp [eax+14h]`: forwards to the entity script's
// slot 5), 6 OnInterrupted 0x00CE10A0. Names: Ego_r.pdb ?OnPredicateFail@CActiveEntityScriptBase@@UAEXXZ 0x00CC45A0,
// ?OnInterrupted@..@@UAEXXZ 0x00CC45B0 (the same 5-before-6 order as CSpawnedFuncBase.h), ??1CActiveEntityScriptBase@@UAE@XZ
// 0x00CC45C0, ctor ??0CActiveEntityScriptBase@@QAE@ABVCScriptThing@@_KK@Z (Ego_r 0x00CC44C0; retail 0x00CE1110, bound in
// rebuild/oracles/reloc_bindings.tsv); FableWin.pdb adds GetFlags / GetEntity / GetPEntityScript.
// Layout (Ego_r.pdb, 0x58; the retail ctor writes the same offsets): CSpawnedFuncBase 0x34, PEntityScript +0x34
// (CCountedPointer<CEntityScriptBase>, zeroed), Flags +0x3C (4th argument), EntityUID +0x40 (unsigned __int64, 2nd/3rd
// argument dwords), Entity +0x48 (the CScriptThing copy, inlined in the ctor: vtable 0x01238C8C, PImp addref).
// The dtor is implicit: 0x00CE1200 destroys Entity (0x004AA840), releases PEntityScript (0x00CE1000), then tail-calls
// ~CSpawnedFuncBase 0x00CDD4C0 with no CActiveEntityScriptBase vtable store first (wave 5 rule: a user-written dtor keeps it).
// Contract: FABLE_RETAIL_NSCRIPT and FABLE_RETAIL_SUSPENDABLE_PROCESS; CScriptThing comes from engine/CScriptThing.h, so the
// includer defines CCountedPointer<T> first.
#if !defined(FABLE_RETAIL_NSCRIPT) || !defined(FABLE_RETAIL_SUSPENDABLE_PROCESS)
#error engine/CActiveEntityScriptBase.h is the retail model: define FABLE_RETAIL_NSCRIPT and FABLE_RETAIL_SUSPENDABLE_PROCESS first
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
class CEntityScriptBase;

class CActiveEntityScriptBase : public CSpawnedFuncBase {
public:
    CActiveEntityScriptBase(const CScriptThing& entity, FableNScriptUInt64 uid, unsigned long flags);   // 0x00CE1110
    virtual void OnPredicateFail();                                      // slot 5  0x00CE1090
    virtual void OnInterrupted();                                        // slot 6  0x00CE10A0
    unsigned long GetFlags() const;                                      // FableWin.pdb
    CScriptThing GetEntity() const;                                      // Ego_r.pdb 0x00C9AE40, FableWin.pdb
    CEntityScriptBase* GetPEntityScript();                               // FableWin.pdb
    CCountedPointer<CEntityScriptBase> PEntityScript;                    // +0x34
    unsigned long                      Flags;                            // +0x3c
    FableNScriptUInt64                 EntityUID;                        // +0x40
    CScriptThing                       Entity;                           // +0x48
};
#if defined(_MSC_VER)   // VC7.1 aligns the unsigned __int64 member to 8 (tail pad 0x54 -> 0x58); the i386 SysV ABI aligns it to 4
FABLE_STATIC_ASSERT(sizeof(CActiveEntityScriptBase) == 0x58);
#endif
FABLE_STATIC_ASSERT(offsetof(CActiveEntityScriptBase, Entity) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT && FABLE_RETAIL_SUSPENDABLE_PROCESS
#endif // FABLE_ENGINE_CACTIVEENTITYSCRIPTBASE_H
