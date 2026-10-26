// CTCQuickAccessMenu::CTCQuickAccessMenu(CThing* thing) -- CTCBase(thing), own vtable, inactive, no components, not quitting, then join the global update list
// Leaf: ??0CTCQuickAccessMenu
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006259E0, length 48 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", propagated),
// CTCBase::AddToGlobalTCs() @ 0x00686ED0 (manifest "std::list::push_back", propagated; named by the landed
// CTCActionUseBuyHouse constructor 0x0072E690). Vtable 0x01258B74 = CTCQuickAccessMenu (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCQuickAccessMenu (0x38) with the 12-byte retail vector: Active +0x0c, Components
// (vector<NUISystem::CComponent*>) +0x10, Quitting +0x1c; LastRenderTime / FirstRenderFrame are left uninitialised.
// Shape: speed TU (`mov eax,[esp+4] ; push eax`); all initialisers follow the derived vtable store from one `xor eax,eax`
// (the two bools as `al`, the vector's three pointers through its inline zeroing constructor), then the body's call.
class CThing;

class CTCBase { public:                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                           // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

namespace NUISystem { class CComponent; }
struct CRetailVector_Component {                            // retail std::vector, 12 bytes
    NUISystem::CComponent** _Myfirst;
    NUISystem::CComponent** _Mylast;
    NUISystem::CComponent** _Myend;
    CRetailVector_Component() : _Myfirst(0), _Mylast(0), _Myend(0) { }
};

class CTCQuickAccessMenu : public CTCBase { public:                 // vtable 0x01258B74
    bool                    Active;                         // +0x0c
    CRetailVector_Component Components;                     // +0x10
    bool                    Quitting;                       // +0x1c
    double                  LastRenderTime;                 // +0x20
    bool                    FirstRenderFrame;               // +0x28
    virtual void v00();
    CTCQuickAccessMenu(CThing* thing);
};

CTCQuickAccessMenu::CTCQuickAccessMenu(CThing* thing)
    : CTCBase(thing)
    , Active(false)
    , Components()
    , Quitting(false)
{
    AddToGlobalTCs();
}