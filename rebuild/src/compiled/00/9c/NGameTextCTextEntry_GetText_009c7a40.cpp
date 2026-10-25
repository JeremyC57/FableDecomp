// NGameText::CTextEntry::GetText() const -- returns a copy of the entry's wide text (CWideString member at +0x28) by value (hidden result pointer, out-of-line CWideString copy constructor)
// Leaf: ?GetText@CTextEntry@NGameText@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7A40, length 20 bytes.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720.
// Identity: the PDB name sets hold ?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ (CGroupEntry has no GetText;
// CBankFile::GetBankHandle returns a CCharString, not this wide copy).  Callers such as CItem::GetItemDescription
// 0x005BD8F3 call it on the pointee of CDataBank::GetPTextEntry 0x009C9070 (CIVCountedPointer<const CTextEntry>).
// `this` is a local model: no engine header exists for NGameText::CTextEntry; the text member's offset is from the
// bytes (`add ecx,28h`), and its name is descriptive.
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& o);                  // 0x0099B720
};

namespace NGameText {
class CTextEntry {
public:
    unsigned char _pad[0x28];
    CWideString Text;                                    // +0x28
    CWideString GetText() const;
};

CWideString CTextEntry::GetText() const
{
    return Text;
}
}
