// CStreamingFontDataBank::CloseFonts() -- unload and delete every streaming font entry, empty the font table and forget the bank's file name
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F2F0, 161 bytes (+ int3 padding), `ret`.
// Leaf: CloseFonts
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Callees: CStreamingFontDataBankEntry virtuals (vtable 0x0129D5B4, RTTI): slot 0 deleting destructor (`push 1`),
//   slot 1 IsLoaded (0x00A5F220), slot 2 CanUnload (0x00A5F230, result unused), slot 3 Unload (0x00A5F250);
//   memmove @ 0x00BFEAE6 (the erase tail copy), CCharString::operator=(const char*) @ 0x0099EFE0 ("" @ 0x0122D70E).
// Shape: the sibling of the landed CShaderDataBank::CloseShaders 0x00A62050: an indexed walk `for (i = 0; i < size(); ++i)`
//   (unsigned, size re-read each step) with the element through STLport `*(begin() + n)`; a loaded entry is unloaded
//   before the delete (the null test already proven, so `delete` adds none). clear() is the SGI erase(begin(), end()):
//   __copy_trivial of the empty tail [end, finish) with its `last == first` shortcut (`cmp eax,eax` after CSE), else
//   memmove, then finish = the copy's end.
// Retail layout: the CBankFileAsync base is 0x164 bytes (engine/CStreamingFontDataBank.h carries the PDB +0x1D8), so the
//   members sit 0x74 below it: StateBlock +0x164, Fonts +0x178, Filename +0x184, streamingEntry +0x188.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};

class CStreamingFontDataBankEntry {                     // vtable 0x0129D5B4
public:
    virtual ~CStreamingFontDataBankEntry();             // slot 0 (deleting destructor)
    virtual bool IsLoaded() const;                      // slot 1 (0x00A5F220)
    virtual bool CanUnload() const;                     // slot 2 (0x00A5F230)
    virtual void Unload();                              // slot 3 (0x00A5F250)
};

#include "engine/CArray.h"

class CStreamingFontDataBank {
public:
    virtual void v00();
    unsigned char                        _bank[0x160];  // CBankFileAsync part
    unsigned char                        StateBlock[0x14];   // +0x164 CStreamingFontBankStateBlock
    CArray<CStreamingFontDataBankEntry*> Fonts;         // +0x178
    CCharString                          Filename;      // +0x184
    void CloseFonts();
};

void CStreamingFontDataBank::CloseFonts()
{
    for (unsigned long i = 0; i < Fonts.size(); ++i) {
        CStreamingFontDataBankEntry* font = Fonts[i];
        if (font) {
            if (font->IsLoaded()) {
                font->CanUnload();
                font->Unload();
            }
            delete font;
        }
    }
    Fonts.clear();
    Filename = "";
}