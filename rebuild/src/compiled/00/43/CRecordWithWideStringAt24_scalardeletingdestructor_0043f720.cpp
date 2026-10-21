// CRecordWithWideStringAt24::`scalar deleting destructor'(unsigned int flags) -- destroys the CWideString member at +0x24, frees the object when bit 0 is set
// Leaf: ??_GCRecordWithWideStringAt24@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0043F720, 33 bytes, `ret 4`; no code or data references (an orphaned COMDAT). Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array loop, no vptr). Body: ~CWideString (0x0099B510) on
// +0x24, then `operator delete(this)` (0x00BFE9BC) when (flags & 1); returns this. The owning class has no vptr and no
// other non-trivial member; its name is unknown, so CRecordWithWideStringAt24 is behavioural.
// Shape: a class whose only non-trivial member is the CWideString (user-declared, non-virtual destructor); `delete` on it makes
// VC7.1 emit the scalar deleting destructor, as retail did.
class CWideString {
public:
    void* PData;
    ~CWideString();                                        // 0x0099B510
};
class CRecordWithWideStringAt24 {
public:
    unsigned char _pad00[0x24];
    CWideString Text;                                         // +0x24
    ~CRecordWithWideStringAt24() {}
};
void DeleteCRecordWithWideStringAt24(CRecordWithWideStringAt24* p)                 // emission helper, not a retail function
{
    delete p;
}