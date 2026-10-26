// NUISystem::CYesNo::CYesNo(long) -- a changing-state UI component with two extra dwords cleared
// Leaf: ??0CYesNo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CADC, 52 bytes. Manifest label "CTCHeroPodium::CTCHeroPodium" is
// propagated from an unrelated body: the three vtables stored are NUISystem::CYesNo's (0x0122F884 primary, 0x0122F85C
// for the base at +4, 0x0122F854 for the base at +0x18 -- rtti-vtables.tsv); single caller 0x0041D6FF.
// Callee: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50.
//
// Shape: the sibling of NUISystem::COk::COk (0x0041CB70, landed): the argument passed straight through
// (`push dword ptr [esp+8]`), the two members cleared with `and dword ptr [..],0` BEFORE the three vtable stores
// (init-list members of the most-derived class), `mov eax,esi ; ret 4` -- a size-optimised TU
// (#pragma optimize("s", on)). CChangingStateComponent has a root polymorphic base at +0 and two further
// polymorphic bases at +4 (0x14 bytes) and +0x18; its state runs to +0x15C.
#pragma optimize("s", on)
namespace NUISystem {

struct CComponentRoot_Retail {                 // primary base: vptr only
    virtual void v00();
};

struct CComponentBase2_Retail {                // base at +0x04, 0x14 bytes
    virtual void v00();
    unsigned char _body[0x10];
};

struct CComponentBase3_Retail {                // base at +0x18
    virtual void v00();
};

class CChangingStateComponent : public CComponentRoot_Retail, public CComponentBase2_Retail, public CComponentBase3_Retail { public:
    unsigned char _body[0x15c - 0x1c];         // +0x1c..+0x15c
    CChangingStateComponent(long state);       // 0x0052CC50
};

class CYesNo : public CChangingStateComponent { public:      // vtables 0x0122F884 / 0x0122F85C / 0x0122F854
    long m_result;                             // +0x15C
    long m_selection;                          // +0x160
    virtual void v00();
    CYesNo(long state);
};

CYesNo::CYesNo(long state)
    : CChangingStateComponent(state), m_result(0), m_selection(0)
{
}

}