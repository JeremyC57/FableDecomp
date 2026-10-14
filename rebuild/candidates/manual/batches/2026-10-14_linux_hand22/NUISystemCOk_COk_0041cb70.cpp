// NUISystem::COk::COk(long state) -- a changing-state UI component with one extra dword cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CB70, length 45 bytes.
// Callee: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (its
// vtables 0x01245DE4 / 0x01245DBC / 0x01245DB4 name it through rtti-vtables.tsv). This class's
// three vtables are 0x0122FAF4 (primary), 0x0122FAD0 (for the base at +4) and 0x0122FAC8 (for the
// base at +0x18): CChangingStateComponent has a root polymorphic base at +0 and two further
// polymorphic bases at +4 (0x14 bytes) and +0x18 (the retail PDB shows it as 0x18c bytes), and
// COk adds one dword at +0x15c. The argument is passed straight through (`push [esp+8]`) and the
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

struct CChangingStateComponent : CComponentRoot_Retail, CComponentBase2_Retail, CComponentBase3_Retail {
    unsigned char _body[0x15c - 0x1c];         // +0x1c..+0x15c
    CChangingStateComponent(long state);       // 0x0052CC50
};

struct COk : CChangingStateComponent {         // vtables 0x0122FAF4 / 0x0122FAD0 / 0x0122FAC8
    long m_result;                             // +0x15c
    virtual void v00();
    COk(long state);
};

COk::COk(long state)
    : CChangingStateComponent(state), m_result(0)
{
}

}
