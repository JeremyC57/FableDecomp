// CTCCreditsUI::Activate(CCharString part) -- an inactive credits screen remembers the part, has the front-end manager create its component, resets it into state 1 and keeps it; the screen is then active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E4B60, 165 bytes (`ret 4`; the by-value string is destroyed on both paths).
// Callees: ~CCharString @ 0x0099EAE0, CCharString::operator=(const CCharString&) @ 0x0099EFB0, NUISystem::CFrontEndManager::
// GetInstance() @ 0x0041E5F2 (`__fastcall`, no arguments), NUISystem::CManager::CreateComponent(const CCharString&, bool)
// @ 0x0041DB1D, the component's virtual slot 43 (+0xac; a reset hook, named descriptively) and slot 48 ChangeState(long)
// (+0xc0), the Components vector's _M_insert_overflow @ 0x00485DBC (manifest "Std_Vector_Insert_Range_Fill").
// Layout: as FrameUpdate 0x007E4A90: Active +0x0c, Components +0x10, CreditPart +0x20.
// Shape: the early return destroys the parameter inline; CreateComponent's arguments are pushed before GetInstance runs; the
// component pointer is a named local (the `push ecx` word) whose address the inline push_back hands to _M_insert_overflow
// (with the __true_type tag in the dead parameter word); Active is set after the push_back on both arms. The component stays
// in esi across the virtual calls only when VC7.1 can see that _M_insert_overflow does not let `x` escape: retail had that
// knowledge (link-time code generation), so a stand-in body is compiled above under `auto_inline(off)` (GOTCHAS wave 25 /
// chunk 180; the lander grades only the Activate symbol).
// Leaf: ?Activate@CTCCreditsUI@@QAEXVCCharString@@@Z
// Flags: /d2inlS12
// Inline-budget: /d2inlS12 -- only the size-optimised scaffolding COMDATs see it (the row body is /O2): it expands
//   allocator::allocate -> malloc in the _M_insert_overflow copy as the owner 0x00485DBC does (smallest N, the 0x005414A0 sweep).
// NF213 (2026-11-21, q6333 / q6342): the header-less CRetailVector stand-in (its same-TU explicit instantiation bound 0x00485DBC as
//   ?_M_insert_overflow@?$CRetailVector@PAVCComponent@NUISystem@@@@QAE...K_N@Z) is the STLport vector<NUISystem::CComponent*> of
//   stlport/fable_containers.h. push_back's growth call binds the protected IAE ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@
//   V?$allocator@... (the 0x00485DBC self leaf). The body stays visible and out of line (OVERFLOW_OUTOFLINE, as the HEAD same-TU body);
//   its pick-any copy is the owner's through FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY with
//   COPY_TRIVIAL_OUTOFLINE + FILL_N_FASTCALL, and push_back's through FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
//   (Fidelity: scaffolding, not retail; owners 0x00485DBC / 0x00486024). The other local models are HEAD debt, unchanged.
// NF214 (2026-11-21, q6333 / q6342, NF213 option d): the overflow body's callee fill_n<NUISystem::CComponent**> is visible and out of
//   line through FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY (optimize("s", on), as its owner row 0x0048514C sets): with it
//   declared only, the caller lost retail's register allocation (DIFFER 158v165). Its pick-any copy grades MATCH against 0x0048514C
//   (23 bytes); the overflow copy equals the owner 0x00485DBC (137 = 137, bytes and relocations, both naming the same fill_n symbol).
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#define FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CCharString { public:
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NUISystem {
class CComponent { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42)
    virtual void Reset();                               // slot 43 (+0xac)
    V(44) V(45) V(46) V(47)
#undef V
    virtual void ChangeState(long state);               // slot 48 (+0xc0)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
class CTCBase { public:
    virtual void v00();
    class CThing* ParentThing;                         // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
class CTCCreditsUI : public CTCBase { public:
    bool Active;                                        // +0x0c
    std::vector<NUISystem::CComponent*> Components;   // +0x10
    bool Quitting;                                      // +0x1c
    CCharString CreditPart;                             // +0x20
    void Activate(CCharString part);
};

void CTCCreditsUI::Activate(CCharString part)
{
    if (Active)
        return;
    CreditPart = part;
    NUISystem::CComponent* component = NUISystem::CManager::GetInstance()->CreateComponent(CreditPart, false);
    component->Reset();
    component->ChangeState(1);
    Components.push_back(component);
    Active = true;
}