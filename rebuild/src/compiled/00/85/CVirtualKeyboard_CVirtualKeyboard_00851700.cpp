// CVirtualKeyboard::CVirtualKeyboard(CComponent* entryScreen) -- empty profile name, not cancelled / not done, the entry screen from the argument, no entry box
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851700, length 32 bytes.
// Leaf: ??0CVirtualKeyboard
// Callee: CWideString::CWideString() @ 0x0099AED0 (null data + instance counter). Members from the PDB (engine/CVirtualKeyboard.h:
// ProfileName +0x00, Canceled +0x04, Done +0x05, PEntryScreen +0x08, PEntryBox +0x0c); local model because the header flattens
// the CWideString member into its data pointer and the constructor call needs the class.
// Shape: plain initialiser list; VC schedules the argument store last (Canceled, Done, PEntryBox from the xor register, then PEntryScreen).
struct CComponent;
struct CEditBox;
class CWideString { public:
    void* PStringData;
    CWideString();                          // 0x0099AED0
};
struct CVirtualKeyboard {
    CWideString  ProfileName;               // +0x00
    bool         Canceled;                  // +0x04
    bool         Done;                      // +0x05
    CComponent*  PEntryScreen;              // +0x08
    CEditBox*    PEntryBox;                 // +0x0c
    CVirtualKeyboard(CComponent* entryScreen);
};
CVirtualKeyboard::CVirtualKeyboard(CComponent* entryScreen)
    : Canceled(false), Done(false), PEntryScreen(entryScreen), PEntryBox(0)
{
}