// NUISystem::CMenuEntry::MakeUnavailable() -- grey out an available menu entry: run the CComponent behaviour, make its text child unavailable and give the PC arrow child a this-instance deletion
// Leaf: ?MakeUnavailable@CMenuEntry@NUISystem@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053E0C0, 190 bytes (`ret`); CMenuEntry vtable 0x0124662C slot 92
//   (Ego_r.pdb / FableWin.pdb ?MakeUnavailable@CMenuEntry@NUISystem@@UAEXXZ).
// Callees: CComponent::MakeUnavailable 0x00530A40 (qualified), CComponent virtual slots 9 FindFirstChildOfType, 4 FindChildForward,
//   97 FindFirstChildOfTypeNonRecursive, 92 MakeUnavailable, 8 FindChildNonRecursive, 62 SetDeletion(CDeletion) (by value);
//   CCharString(const char*, long) 0x0099EBF0 (the by-value names built in the argument area), malloc 0x00BFEA0E (the
//   deletion's STLport list sentinel, inline ctor), CDeletion(const CDeletion&) 0x0042CD84 and ~list<CComponent*> 0x0042ABCA.
// Layout: CMenuEntry Available +0x198 (retail; the classes between CComponent (0x134) and CMenuEntry are not modelled).
// Shape: the three text lookups each make the found text unavailable in their own arm; VC7.1 cross-jumps the three identical
//   calls into one block, and only this form loads the third lookup's vptr into the dead eax like retail (a single
//   `if (text) text->MakeUnavailable();` after the fallbacks loads it into edx).
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
namespace NUISystem {
class CMenuEntry : public CComponent {
public:
    virtual void MakeUnavailable();
private:
    unsigned char _pad_0x134[0x198 - 0x134];
    bool Available;                                  // +0x198
};

void CMenuEntry::MakeUnavailable()
{
    if (!Available)
        return;
    CComponent::MakeUnavailable();
    CComponent* text = FindFirstChildOfType(UI_TYPE_TEXT);
    if (text)
        text->MakeUnavailable();
    else {
        text = FindChildForward(CCharString("MENU_ENTRY_TEXT", -1));
        if (text)
            text->MakeUnavailable();
        else {
            text = FindFirstChildOfTypeNonRecursive(UI_TYPE_TEXT);
            if (text)
                text->MakeUnavailable();
        }
    }
    CComponent* arrow = FindChildNonRecursive(CCharString("PC_MENU_ENTRY_ARROW", -1));
    if (arrow) {
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        arrow->SetDeletion(deletion);
    }
    Available = false;
}
}