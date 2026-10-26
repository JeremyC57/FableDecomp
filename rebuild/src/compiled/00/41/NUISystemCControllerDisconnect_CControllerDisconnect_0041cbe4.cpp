// NUISystem::CControllerDisconnect::CControllerDisconnect(long state) -- a changing-state UI component with one extra dword cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CBE4, length 45 bytes.
// Callee: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (its
// vtables 0x01245DE4 / 0x01245DBC / 0x01245DB4 name it through rtti-vtables.tsv). This class's
// three vtables are 0x0122FD64 (primary), 0x0122FD3C (for the base at +4) and 0x0122FD34 (for the
// base at +0x18): CChangingStateComponent has a root polymorphic base at +0 and two further
// polymorphic bases at +4 (0x14 bytes) and +0x18 (the retail PDB shows it as 0x18c bytes), and
// CControllerDisconnect adds one dword at +0x15c. The argument is passed straight through (`push [esp+8]`) and the
// member is cleared with `and [..],0`: a size-optimised translation unit (#pragma optimize("s")).
// The manifest label "CConsoleCommandParameters::CConsoleCommandParameters" is propagated and rejected.
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

struct CControllerDisconnect : CChangingStateComponent {         // vtables 0x0122FD64 / 0x0122FD3C / 0x0122FD34
    long m_result;                             // +0x15c
    virtual void v00();
    CControllerDisconnect(long state);
};

CControllerDisconnect::CControllerDisconnect(long state)
    : CChangingStateComponent(state), m_result(0)
{
}

}