// _global::GetWindowTitle() -- the main window's title: the TEXT_GUI_WINDOW_TITLE entry of a freshly opened text bank (default "Fable - The Lost Chapters") followed by a space
// Leaf: ?GetWindowTitle@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004023F0, 274 bytes (+14 int3), plain `ret`; CWideString by hidden pointer in ecx
// (/Gr link-time __fastcall).  Replaces the landed asm_bake bootstrap with compiler-emitted C++.
// Callees: CWideString(const wchar_t*) 0x0099B6B0, CCharString(const char*, long) 0x0099EBF0, operator new 0x00BFEA1A (0x214),
// NGameText::CDataBank::CDataBank 0x009C85A0, vtable slot 1 OpenReadOnly 0x009D56C0 (as the landed InitialiseText 0x00416832),
// NLocalisation::GetFontBankName 0x00415260, ~CCharString 0x0099EAE0, CCRC::Calc 0x004014A0, CDataBank::LookupValue 0x009CC410,
// CDataBank::GetPGroupEntry 0x009C9070, CGroupEntry::GetText 0x009C7A40, CWideString::operator= 0x0099B7D0, ~CWideString
// 0x0099B510, operator+(const CWideString&, const wchar_t*) 0x0099BF30 (L" " @ 0x0122D820), slot 0 deleting destructor.
// Shape: the bank is owned by an auto_ptr (Ego PDB has auto_ptr<NGameText::CDataBank>) declared between the title and the tag
// and reset after the tag is built -- that is what puts the deleting-destructor call (no null test: the reset's delete of
// the null pointer folds away, and VC7.1 drops the test after the member calls) between ~tag and ~title, after the
// `return title + L" "` temporary is built straight into the hidden return slot.  The tag's CRC is the inline
// CCRC::CalcFromString with both accessors' null arms merged; the group entry's release is the pointee's
// `DecRef(); if (GetRefCount() == 0) Delete();` behind a re-tested pointer (as the landed CTCQuestCard::GetQuestName 0x00702310).
#include <new>
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
CWideString operator+(const CWideString& lhs, const wchar_t* rhs);   // 0x0099BF30
class CCharString {
public:
    struct CStringData {
        const char* Data;                               // +0x0
        long        Length;                             // +0x4
    };
    CStringData* PStringData;                           // +0x0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
class CCRC {
public:
    static unsigned long Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
    static unsigned long CalcFromString(const CCharString& s) { return Calc(0, s.GetString(), s.Length()); }
};
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);                    // slot 0
    virtual void Delete();                              // slot 1
    unsigned long IVRefCount;                           // +0x04
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> struct CIVCountedPointer {           // 4 bytes
    T* Object;
    T* operator->() const { return Object; }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
};
namespace NGameText {
struct CGroupEntry : CIVCountedPointeeBase {
    CWideString GetText() const;                        // 0x009C7A40
};
class CDataBank {
public:
    virtual ~CDataBank();                               // slot 0 (0x009CB840)
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);   // slot 1 (0x009D56C0)
    CDataBank();                                        // 0x009C85A0
    unsigned long LookupValue(unsigned long key);       // 0x009CC410
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
private:
    unsigned char BankFileAndResourceBank[0x210];
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
namespace NLocalisation { CCharString GetFontBankName(); }   // 0x00415260

CWideString GetWindowTitle()
{
    CWideString title(L"Fable - The Lost Chapters");
    std::auto_ptr<NGameText::CDataBank> bank;
    CCharString tag("TEXT_GUI_WINDOW_TITLE");
    bank.reset(new NGameText::CDataBank);
    bank->OpenReadOnly(NLocalisation::GetFontBankName(), 0x100);
    unsigned long index = bank->LookupValue(CCRC::CalcFromString(tag));
    if (index > 0) {
        CIVCountedPointer<NGameText::CGroupEntry> entry = bank->GetPGroupEntry(index, true);
        if (entry.Object)
            title = entry->GetText();
    }
    return title + L" ";
}