// CFontManager::GetStaticFont(unsigned long index) -- fetch font `index` from the font manager's static font data bank as a counted CFontBank pointer and point the font back at this manager
// Leaf: GetStaticFont
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2BA0, length 104 bytes (twin of 0x009E2C10 GetStreamingFont).
// Identity: the manifest label "CManager::GetEmbeddedEventNames" is wrong -- the PDB symbols (egor_signatures) list
//   ?GetStaticFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@K@Z; CFontBank::PManager (+0xc) receives `this`.
// Callees: CFontDataBank::GetFontInfo @ 0x00A635E0 (manifest label CTCInventoryAbilities::GetAbilityLevel is a byte-twin), CFontDataBankEntry::AcquireFont @ 0x00A634F0, operator delete @ 0x00BFE9BC.
// Layout (PDB CFontManager): PFontDataBank (+0x04) CCountedPointer whose Data is the bank; CFontBank (PDB, 0x10) PManager +0x0c.
// Shape: `CCountedPointer<T> p = ...; return p;` -- VC7.1 has no NRVO, so the hidden-result copy (count +1) and the local's release
//   (count -1, delete function + operator delete on zero) both appear; the local's hidden-return slot is pushed before GetFontInfo's argument.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCPPointerInfo { public:                            // the shared block of a CCountedPointer: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);     // called with Data in ecx
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;                          // +0
    CCPPointerInfo* Info;                          // +4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
class CFontManager;
class CFontBank { public:                                 // PDB, 0x10
    void*          _vptr;                          // +0x00
    unsigned long  FontHeight;                     // +0x04
    void*          Name;                           // +0x08 CCharString
    CFontManager*  PManager;                       // +0x0c
};
class CFontDataBankEntry { public:
    CCountedPointer<CFontBank> AcquireFont();      // 0x00A634F0
};
class CFontDataBank { public:
    CFontDataBankEntry* GetFontInfo(unsigned long index);   // 0x00A635E0
};
class CFontManager { public:
    void*                          _vptr;
    CCountedPointer<CFontDataBank> PFontDataBank;  // +0x04
    CCountedPointer<CFontBank> GetStaticFont(unsigned long index);
};
CCountedPointer<CFontBank> CFontManager::GetStaticFont(unsigned long index)
{
    CCountedPointer<CFontBank> font = PFontDataBank->GetFontInfo(index)->AcquireFont();
    font->PManager = this;
    return font;
}