// CSoundBankLH::~CSoundBankLH() -- Lionhead-audio sound bank destructor: restore the CSoundBankLH vtable, release the library bank object (its slot-0 call, pointer cleared), release the counted pointer at +0x14, then the implicit CASoundBank destructor (counted pointer at +4) and the CBaseClassNonCopyable base
// Leaf: ??1CSoundBankLH@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0B9A0, 115 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (proven): stores vtable 0x0129C9DC first (RTTI .?AVCSoundBankLH@@ : CASoundBank : CBaseClassNonCopyable : CBaseClass);
//   sole caller is that vtable's slot-0 scalar deleting destructor 0x00A0BDF0 (call at 0x00A0BDF3). The catalog label
//   C3DMeshLODInfo::~C3DMeshLODInfo is propagated from a masked twin; ~C3DMeshLODInfo is 0x00A24470. CSoundBankLH is in
//   neither PDB, so its name is RTTI's and its member / slot names are descriptive (address-suffixed).
// Callees: the library bank object's slot 0 (no arguments), the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete
//   0x00BFE9BC (count blocks), ~CBaseClassNonCopyable 0x0099A300.
// Layout (from the retail constructors): CASoundBank 0x00A0BD00 = CBaseClassNonCopyable + PSymbolMap at +0x04 (Ego_r.pdb CASoundBank,
//   12 bytes, as the landed implicit ~CASoundBank 0x00A0BD40; abstract: slots 1.. are _purecall 0x00BFEA08); CSoundBankLH ctor 0x00A0BAC0 (one argument) = Owner +0x0C (the argument; slot 1
//   0x00A0B630 reads Owner->+0x40, CSoundManagerLH::m_pSoundSystem, to create the bank object; the callers 0x00A3E444 / 0x0042E1A0 pass the
//   CSoundManagerLH `this` / PSoundManager), BankObject +0x10, a counted pointer +0x14,
//   Loaded +0x1C (slot 10 0x00A0BDE0 returns it, slot 8 0x00A0B990 sets it).
// Shape: CSoundBankLH's slot 2 (0x00A0B6C0, `if (p) { p->slot0(); p = 0; }`) is called from the user destructor -- devirtualised
//   in the destructor and expanded from its same-TU body, as the font entries' Unload; then the +0x14 counted pointer's
//   destructor and the implicit ~CASoundBank (no vtable store: compiler-generated, its out-of-line copy is 0x00A0BD40) expanded
//   inline; engine/CCountedPointer.h bodies share the one zero register (edi) with the slot-2 clear.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
class CSoundBankObject_00a0b6c0 {                           // audio-library bank object (no RTTI); only slot 0 is used here
public:
    virtual void Release() = 0;                             // slot 0 (descriptive)
};
class CCRCSymbolMap;
class CSoundBankResource_00a0b9a0;                          // counted-pointer target (descriptive)
class CSoundManagerLH;
class CASoundBank : public CBaseClassNonCopyable {          // RTTI .?AVCASoundBank@@, vtable 0x0129C9AC; implicit dtor 0x00A0BD40
public:
    CCountedPointer<CCRCSymbolMap> PSymbolMap;              // +0x04 (Ego_r.pdb CASoundBank, 12 bytes)
    virtual void Load_00a0b630(long) = 0;                   // slot 1
    virtual void Unload_00a0b6c0() = 0;                     // slot 2
};
class CSoundBankLH : public CASoundBank {                   // RTTI .?AVCSoundBankLH@@, vtable 0x0129C9DC
public:
    CSoundManagerLH*                             Owner;      // +0x0c
    CSoundBankObject_00a0b6c0*                   BankObject; // +0x10
    CCountedPointer<CSoundBankResource_00a0b9a0> Resource;   // +0x14
    bool                                         Loaded;     // +0x1c
    virtual ~CSoundBankLH();
    virtual void Load_00a0b630(long);
    virtual void Unload_00a0b6c0();
};
CSoundBankLH::~CSoundBankLH()
{
    Unload_00a0b6c0();
}
void CSoundBankLH::Unload_00a0b6c0()                        // 0x00A0B6C0 (slot 2; not graded here)
{
    if (BankObject) {
        BankObject->Release();
        BankObject = 0;
    }
}