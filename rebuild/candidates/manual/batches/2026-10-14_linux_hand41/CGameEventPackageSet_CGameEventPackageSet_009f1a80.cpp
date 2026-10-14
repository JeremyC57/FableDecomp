// CGameEventPackageSet::CGameEventPackageSet() -- fifty packages built by an inlined counted loop (each clears NoEvents and runs the vector constructor iterator over its forty events), then NoPackages cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F1A80, length 64 bytes.
// Callees: `vector constructor iterator' @ 0x004038C0 (element size 0x28, count 0x28, CGameEvent::CGameEvent @ 0x004159A0).
// Layout from the PDB: CGameEventPackage {TimeStamp +0, NoEvents +4, CGameEvent[40] +8} = 0x648; the set is
// {NoPackages +0, CGameEventPackage[50] +4} (engine/CGameEventPackageSet.h, which is a POD snapshot).
// Shape: the package constructor is inline, so the outer array is an inlined `dec edi ; jne` loop while the inner CGameEvent
// array (out-of-line constructor) goes through the iterator; NoPackages is stored AFTER the loop, so it is a body assignment
// (an initialiser-list entry would be stored first, in declaration order). TimeStamp is left uninitialised.
// Leaf: ??0CGameEventPackageSet
struct CGameEvent {
    long          Type;                             // +0x00
    signed char   Player;                           // +0x04
    unsigned char Data[32];                         // +0x05
    unsigned char EndPos;                           // +0x25
    bool          Valid;                            // +0x26
    bool          Replacement;                      // +0x27
    CGameEvent();                                   // 0x004159A0
};

struct CGameEventPackage {
    long          TimeStamp;                        // +0x00
    unsigned long NoEvents;                         // +0x04
    CGameEvent    Events[40];                       // +0x08
    CGameEventPackage() : NoEvents(0) {}
};

struct CGameEventPackageSet {
    unsigned long     NoPackages;                   // +0x00
    CGameEventPackage Packages[50];                 // +0x04
    CGameEventPackageSet();
};

CGameEventPackageSet::CGameEventPackageSet()
{
    NoPackages = 0;
}
