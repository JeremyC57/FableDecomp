// _global::GetWindowTitle() -- the main window's title: the TEXT_GUI_WINDOW_TITLE entry of a freshly opened text bank (default "Fable - The Lost Chapters") followed by a space
// Leaf: ?GetWindowTitle@@YI?AVCWideString@@XZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004023F0, 274 bytes (+14 int3), plain `ret`; CWideString by hidden pointer in ecx
// (/Gr link-time __fastcall).  Replaces the landed asm_bake bootstrap with compiler-emitted C++.
// Callees: CWideString(const wchar_t*) 0x0099B6B0, CCharString(const char*, long) 0x0099EBF0, operator new 0x00BFEA1A (0x214),
// NGameText::CDataBank::CDataBank 0x009C85A0, vtable slot 1 OpenReadOnly 0x009D56C0 (as the landed InitialiseText 0x00416832),
// NLocalisation::GetTextBankName 0x00415260, ~CCharString 0x0099EAE0, CCRC::Calc 0x004014A0, NGameText::CDataBank::LookupValue 0x009CC410,
// CDataBank::GetPGroupEntry 0x009C9070, CGroupEntry::GetText 0x009C7A40, CWideString::operator= 0x0099B7D0, ~CWideString
// 0x0099B510, operator+(const CWideString&, const wchar_t*) 0x0099BF30 (L" " @ 0x0122D820), slot 0 deleting destructor.
// Shape: the bank is owned by an auto_ptr (Ego PDB has auto_ptr<NGameText::CDataBank>) declared between the title and the tag
// and reset after the tag is built -- that is what puts the deleting-destructor call (no null test: the reset's delete of
// the null pointer folds away, and VC7.1 drops the test after the member calls) between ~tag and ~title, after the
// `return title + L" "` temporary is built straight into the hidden return slot.  The tag's CRC is the inline
// CCRC::CalcFromString with both accessors' null arms merged; the group entry's release is the pointee's
// `DecRef(); if (GetRefCount() == 0) Delete();` behind a re-tested pointer (as the landed CTCQuestCard::GetQuestName 0x00702310).
#include <new>
// NF47 (round 7): 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0 ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z, the base member
//   of the freshly opened text bank (NGameText::CDataBank : CBankFile, polymorphic: slot 0 deleting dtor, slot 1 OpenReadOnly); was the
//   made-up NGameText::CDataBank::LookupValue (in neither PDB). The local CWideString / CCharString / CCRC models are replaced by
//   fable_string.h (inline GetCRC) and the row's own NGameText::CDataBank model now derives from engine/CBankFile.h (STATICBLOCKS: the
//   polymorphic 0x110 base) with its remaining 0x104 bytes opaque.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CBankFile.h"
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) const;                    // slot 0
    virtual void Delete() const;                              // slot 1
    mutable unsigned long IVRefCount;                           // +0x04
    void DecRef() const { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:           // 4 bytes
    T* Object;
    T* operator->() const { return Object; }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public:
    CWideString GetText() const;                        // 0x009C7A40
};
class CDataBank : public CBankFile {
public:
    virtual ~CDataBank();                               // slot 0 (0x009CB840)
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);   // slot 1 (0x009D56C0)
protected:
    virtual unsigned long GetDataFormatVersion();       // ?GetDataFormatVersion@CDataBank@NGameText@@MAEKXZ (both PDBs): CBankFile's pure slot
public:
    CDataBank();                                        // 0x009C85A0
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
private:
    unsigned char ResourceBankAndText[0x214 - 0x110];
};
}
namespace std {
template <class _Tp> class auto_ptr {
public:
    _Tp* _M_p;
    explicit auto_ptr(_Tp* px = 0) : _M_p(px) { }
    ~auto_ptr() { _Tp* pt = get(); delete pt; }
    _Tp* get() const { return _M_p; }
    _Tp* operator->() const { return get(); }
    void reset(_Tp* px = 0)
    {
        _Tp* pt = get();
        if (px != pt) delete pt;
        _M_p = px;
    }
};
}
namespace NLocalisation { CCharString GetTextBankName(); }   // 0x00415260

CWideString GetWindowTitle()
{
    CWideString title(L"Fable - The Lost Chapters");
    std::auto_ptr<NGameText::CDataBank> bank;
    CCharString tag("TEXT_GUI_WINDOW_TITLE");
    bank.reset(new NGameText::CDataBank);
    bank->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);
    unsigned long index = bank->FindIndexBySymbolCRC(tag.GetCRC());
    if (index > 0) {
        CIVCountedPointer<const NGameText::CTextEntry> entry = bank->GetPTextEntry(index, true);
        if (entry.Object)
            title = entry->GetText();
    }
    return title + L" ";
}