// CCreatureActionHeroRunningStop::CCreatureActionHeroRunningStop(CThingCreatureBase&) -- base CCombatActionBase(owner, 0, zeroed CSoundPair)
// Leaf: ??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085BBA0, length 49 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030.
// Vtable 0x012767EC: rtti-vtables.tsv .?AVCCreatureActionHeroRunningStop@@ (the class this constructor builds).
// Naming (NF457): this is ??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z (Ego_r + FableWin name sets), was
//   ??0CCombatAction_GenericResponseToDecapitation@@QAE@PAVCThingCreatureBase@@@Z (a propagated label). Evidence: the constructor stores
//   vftable 0x012767EC = RTTI CCreatureActionHeroRunningStop (retail `ret 4`, one 4-byte argument); the row sits among the landed
//   CCreatureActionHeroRunningStop members (Initialise 0x0085B8B0, IsClearingMovementVector 0x0085BC00, ??_G 0x0085BC60, Clone 0x0085BD20,
//   whose vftable is 0x012767EC); the Decapitation constructor is 0x0066A560 (stores its own vftable 0x0125AD94, bound by its caller
//   0x00667450). No retail call or reference reaches 0x0085BBA0. A reference passes the same pointer, so the object is unchanged but for the names.
// Shape: the action code is 0, so VC reuses the `xor eax,eax` of the sound pair for the pushed code, and the
// two zero stores of the pair are sunk below the pushes (`push ecx ; push eax ; push edx ; mov [esp+10h],eax ;
// mov [esp+14h],eax ; call`). The row keeps HEAD's local model (base sizeof 0x114);
// engine/CCreatureActionHeroRunningStop.h has a retail opt-in (FABLE_RETAIL_CCREATUREACTIONHERORUNNINGSTOP_VIRTUAL_MODEL) this row is not ported to.
class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    // NF123 (2026-11-20, q6404): creature by reference, Ego_r.pdb ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (0x00858030; was PAV).
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureActionHeroRunningStop : public CCombatActionBase { public:
    virtual void v00();                         // own vtable 0x012767EC
    CCreatureActionHeroRunningStop(CThingCreatureBase& owner);
};

CCreatureActionHeroRunningStop::CCreatureActionHeroRunningStop(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 0, CSoundPair())
{
}