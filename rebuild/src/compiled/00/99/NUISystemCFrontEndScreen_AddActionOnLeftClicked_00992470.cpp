// NUISystem::CFrontEndScreen::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>& action) -- store the left-click action (counted-pointer assignment into the member at +0x170)
// Leaf: ?AddActionOnLeftClicked@CFrontEndScreen@NUISystem@@QAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x00992470, 86 bytes, `ret 4`; callers 0x00991B32. Byte family: 0x00669280 / 0x00992470 / 0x009FD870
// differ only in the member offset (Data +0x7c / +0x170 / +0x280).
// Label doubt: Ego_r.pdb has AddActionOnLeftClicked virtual on CClickable / CComponent / CFrontEndScreen
// (?AddActionOnLeftClicked@CFrontEndScreen@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z, PLeftClickAction
// at PDB +0x190), but no retail vtable holds this address; the catalog name is kept as the best available.
// Body: Member = action -- the inline CCountedPointer::operator= / ShareData: unless the info blocks match, release the old
// one (`--count`, re-read; at zero call its destroy thunk (+4) on the item (+8), then operator delete 0x00BFE9BC on the
// block), copy Data / Info, `++count` on the new block.
// Layout: CCountedPointer {T* Data; CCPPointerInfo* Info} at +0x170; CCPPointerInfo {long Count; destroy thunk +4; item +8}
// (the landed 0x00695350 ShareData model). The member offset is retail evidence only.
// Shape: /O2; the source Data / Info bound to locals first (ebx / edi loaded before the compare).
struct CCPPointerInfo {
    long  Count;                                         // +0x00
    void (__fastcall *Destroy)(void* item);              // +0x04
    void* Item;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* data = o.Data;
        CCPPointerInfo* info = o.Info;
        if (Info != info)
        {
            if (Info)
            {
                if (--Info->Count == 0)
                {
                    Info->Destroy(Info->Item);
                    ::operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->Count;
        }
        return *this;
    }
};
namespace NUISystem {
class CAction;
class CFrontEndScreen {
public:
    unsigned char             _pad00[0x170];
    CCountedPointer<CAction>  PLeftClickAction;          // +0x170
    void AddActionOnLeftClicked(CCountedPointer<CAction>& action);
};
void CFrontEndScreen::AddActionOnLeftClicked(CCountedPointer<CAction>& action)
{
    PLeftClickAction = action;
}
}