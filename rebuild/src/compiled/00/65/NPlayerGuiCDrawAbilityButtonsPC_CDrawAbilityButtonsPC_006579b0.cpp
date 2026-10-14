// NPlayerGui::CDrawAbilityButtonsPC::CDrawAbilityButtonsPC(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawAbilityButtons(gui, list), own vtable, the button count at +0x28 set to 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006579B0, length 37 bytes.
// Callees: NPlayerGui::CDrawAbilityButtons::CDrawAbilityButtons(CPlayerGui*, vector<CDrawBase*>*) @ 0x00651A7E. Vtable 0x0125A7D8 =
// NPlayerGui::CDrawAbilityButtonsPC (rtti-vtables.tsv). Shape: a speed body (the arguments are loaded into registers, not pushed through),
// the member store follows the vtable store. No PDB layout for the PC subclass; the member is named descriptively.
struct CPlayerGui;

namespace NPlayerGui {

struct CDrawBase;

struct CDrawAbilityButtons {
    virtual void v00();
    unsigned char _body[0x28 - 4];                 // +0x04 .. +0x28: CDrawBase and the ability-button state
    CDrawAbilityButtons(CPlayerGui* gui, void* drawList);   // 0x00651A7E (vector<CDrawBase*>*)
};

struct CDrawAbilityButtonsPC : CDrawAbilityButtons {   // vtable 0x0125A7D8
    virtual void v00();
    long m_buttonCount;                            // +0x28
    CDrawAbilityButtonsPC(CPlayerGui* gui, void* drawList);
};

CDrawAbilityButtonsPC::CDrawAbilityButtonsPC(CPlayerGui* gui, void* drawList)
    : CDrawAbilityButtons(gui, drawList), m_buttonCount(2)
{
}

}