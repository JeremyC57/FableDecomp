// CFontDataBank::CloseFonts() -- pop every font entry off the back of the Fonts array: a loaded entry is asked CanUnload and unloaded, every entry is deleted (virtual deleting destructor) and its slot cleared; then the bank's Filename is emptied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A633A0, length 123 bytes (+ int3 padding).
// Callees: CFontDataBankEntry virtual slots (vtable 0x0129DA70, rtti-vtables.tsv): 0 = deleting destructor (`push 1 ; call [eax]`),
// 1 = IsLoaded @ 0x00A63550, 2 = CanUnload @ 0x00A63560 (result unused), 3 = Unload @ 0x00A63580; CCharString::operator=(const char*)
// @ 0x0099EFE0 with the literal at 0x0122D70E (the empty string).
// Layout: engine/CFontDataBank.h (Ego_r.pdb, 0x198 bytes) puts Fonts at +0x184 (a 16-byte CArray) and Filename at +0x194; the retail object
// holds the 12-byte CArray at +0x120 and Filename at +0x12c (0x64 bytes lower), so `this` is modelled locally with the PDB names.
// Shape: `while (!empty())` tests first/last at the top and re-tests with the freshly stored last at the bottom; back() is
// `*(end() - 1)` re-read from the member for the clear; `delete` on a tested pointer is the bare virtual deleting-destructor call;
// pop_back is the load / `add -4` / store of the last pointer.
class CFontDataBankEntry {
public:
    virtual ~CFontDataBankEntry();          // slot 0 (deleting destructor)
    virtual bool IsLoaded();                // slot 1, 0x00A63550
    virtual bool CanUnload();               // slot 2, 0x00A63560
    virtual void Unload();                  // slot 3, 0x00A63580
};

struct CFontEntryArray {                    // retail CArray<CFontDataBankEntry*>: first / last / end
    CFontDataBankEntry** m_pFirst;
    CFontDataBankEntry** m_pLast;
    CFontDataBankEntry** m_pEnd;
    bool empty() { return m_pFirst == m_pLast; }
    CFontDataBankEntry*& back() { return *(m_pLast - 1); }
    void pop_back() { m_pLast = m_pLast - 1; }
};

class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const char* text);   // 0x0099EFE0
};

class CFontDataBank {
public:
    virtual void v00();
    unsigned char   _pad04[0x11c];
    CFontEntryArray Fonts;                  // +0x120 (PDB +0x184)
    CCharString     Filename;               // +0x12c (PDB +0x194)
    void CloseFonts();
};

void CFontDataBank::CloseFonts()
{
    while (!Fonts.empty()) {
        CFontDataBankEntry* entry = Fonts.back();
        if (entry) {
            if (entry->IsLoaded()) {
                entry->CanUnload();
                entry->Unload();
            }
            delete entry;
            Fonts.back() = 0;
        }
        Fonts.pop_back();
    }
    Filename = "";
}
