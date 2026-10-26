// SetPAcceptButtonRecursive(NUISystem::CComponent* component, NUISystem::CComponent* acceptButton) -- give every descendant of a component (depth first, the accept button itself skipped) the front-end screen's accept button
// Leaf: ?SetPAcceptButtonRecursive@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054E350, 119 bytes (128 with padding), plain `ret` (Ego_r.pdb
//   `?SetPAcceptButtonRecursive@@YIXPAVCComponent@NUISystem@@0@Z`: a free __fastcall, component in ecx, button in edx).
// Callees: CComponent virtual slots 53 GetChildren() (+0xd4, vector<CCountedPointer<CComponent> >&, re-called for every
//   use) and 7 SetPAcceptButton(CComponent*) (+0x1c; Ego_r.pdb `?SetPAcceptButton@CComponent@NUISystem@@UAEXPAV12@@Z`,
//   overridden by CSlider / CTextSlider); itself.
// Callers: itself (0x0054E3A7) and a tail jump at 0x0054E4E8 in NUISystem::CFrontEndScreen::Initialise 0x0054E4B0.
// Layout: STLport vector of 8-byte CCountedPointer<CComponent> { Data, Info } (the `sar 3` size); slot numbers as in the
//   landed CList::RestoreState 0x00536C64 (GetChildren slot 53). Local polymorphic view (no engine header for the slots).
// Shape: `for (unsigned int i = 0; i < component->GetChildren().size(); ++i)` (the unsigned `jbe` entry test, i in edi)
//   with GetChildren() called afresh for the compare, the virtual call and the recursion.
namespace NUISystem {
class CComponent;
}
class CCPPointerInfo;
template <class T> class CCountedPointer {      // 8 bytes: the object, then its shared count record
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};
typedef CCountedPointer<NUISystem::CComponent> CComponentPointer;
struct CComponentPointerVector {                // STLport vector<CCountedPointer<CComponent> > (12 bytes)
    CComponentPointer* _M_start;
    CComponentPointer* _M_finish;
    CComponentPointer* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    CComponentPointer& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NUISystem {
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006();
    virtual void SetPAcceptButton(CComponent* acceptButton);            // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual CComponentPointerVector& GetChildren();                     // slot 53 (+0xd4)
};
}
void __fastcall SetPAcceptButtonRecursive(NUISystem::CComponent* component, NUISystem::CComponent* acceptButton)
{
    for (unsigned int i = 0; i < component->GetChildren().size(); ++i) {
        if (component->GetChildren()[i].Get() != acceptButton) {
            component->GetChildren()[i]->SetPAcceptButton(acceptButton);
            SetPAcceptButtonRecursive(component->GetChildren()[i].Get(), acceptButton);
        }
    }
}