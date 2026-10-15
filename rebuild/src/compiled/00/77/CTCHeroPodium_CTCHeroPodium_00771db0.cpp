// CTCHeroPodium::CTCHeroPodium(CThing*) -- component constructor: CTCBase(parent), no crowd occupants, an empty occupation vector
// Leaf: ??0CTCHeroPodium
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771DB0, 39 bytes (+9 int3 padding). Vtable 0x0126A89C (CTCHeroPodium).
// Single caller: the component factory at 0x004D4015 (global CreateCTCHeroPodium, landed).
// Callee: CTCBase::CTCBase(CThing*) @ 0x00686800.
//
// Shape: speed TU (argument through eax, `xor eax,eax` zero register). NumberOfCrowdOccupants (+0x0C) is zeroed
// BEFORE the vtable store -- a plain init-list member -- while the three vector words (+0x10/+0x14/+0x18) follow
// it: the CrowdOccupation vector is a member object with its own inline constructor.
// Layout: retail vector<long> is 12 bytes (PDB 16), so E_X / E_Y sit at +0x1C / +0x28 (PDB +0x20 / +0x2C); they
// are C3DVectors with an empty default constructor and stay uninitialised. Local model (no engine header).

struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing* parent);                   // 0x00686800
};

struct C3DVector {
    float x, y, z;
    C3DVector() {}
};

struct CLongVector {                           // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};

struct CTCHeroPodium : CTCBase {
    long        NumberOfCrowdOccupants;        // +0x0C
    CLongVector CrowdOccupation;               // +0x10 (PDB +0x10, 16 bytes)
    C3DVector   E_X;                           // +0x1C (PDB +0x20)
    C3DVector   E_Y;                           // +0x28 (PDB +0x2C)
    virtual void v00();
    CTCHeroPodium(CThing* parent);
};

CTCHeroPodium::CTCHeroPodium(CThing* parent)
    : CTCBase(parent), NumberOfCrowdOccupants(0)
{
}