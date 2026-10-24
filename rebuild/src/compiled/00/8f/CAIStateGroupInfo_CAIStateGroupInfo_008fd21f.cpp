// CAIStateGroupInfo::CAIStateGroupInfo(CCharString, CAIStateGroupBase* (__fastcall*)()) -- registration record: copy the state-group name, store the allocator
// Leaf: ??0CAIStateGroupInfo
// Retail 0x008FD21F, 0x23 bytes. Manifest label "GetDefaultVal<CParticleMorphs>" (_global, __fastcall, 0 params) is a
// propagated name and wrong: `ret 8` with `this` in ecx is a two-argument thiscall that returns `this`. Its 203 callers
// are the AI module's static initialisers (0x0121F86F..): each builds a state-group name (e.g. "SG_MINION_WANDER" through
// the inline name getter 0x008FD242) in the argument slot, passes a factory (0x00901ED3 = `new CAIStateGroup_MinionWander`)
// and copies the record into a global (0x013BB008.., copy ctor 0x008FE940). The PDB name set holds
// `??0CAIStateGroupInfo@@QAE@VCCharString@@P6IPAVCAIStateGroupBase@@XZ@Z`; Ego_r layout: StateGroupName +0 (CCharString),
// AllocFunc +4.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0 (the
// by-value name parameter is destroyed by the callee, after the AllocFunc store).
// Unaligned start (size-optimised AI module, like its neighbours 0x008FD242 / 0x008FD255).
// Pragma: optimize("s").
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CAIStateGroupBase;
typedef CAIStateGroupBase* (__fastcall* TAIStateGroupAllocFunc)();

struct CAIStateGroupInfo {
    CCharString            StateGroupName;              // +0x00
    TAIStateGroupAllocFunc AllocFunc;                   // +0x04
    CAIStateGroupInfo(CCharString name, TAIStateGroupAllocFunc alloc);
};

CAIStateGroupInfo::CAIStateGroupInfo(CCharString name, TAIStateGroupAllocFunc alloc)
    : StateGroupName(name)
    , AllocFunc(alloc)
{
}