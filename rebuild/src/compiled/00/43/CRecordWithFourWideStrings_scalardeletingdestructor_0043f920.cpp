// CRecordWithFourWideStrings::`scalar deleting destructor'(unsigned int flags) -- destroys the four CWideString members (+0xc, +8, +4, +0), frees the object when bit 0 is set
// Leaf: ??_GCRecordWithFourWideStrings@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0043F920, 54 bytes, `ret 4`; no code or data references (an orphaned COMDAT beside the
// 0x0043F720 / 0x0043F750 / 0x0043F980 deleting destructors of the same family). Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array loop, no vptr). Body: ~CWideString
// (0x0099B510) on +0xc, +8, +4 and +0 (reverse member order), then `operator delete(this)` (0x00BFE9BC) when
// (flags & 1); returns this. The 16-byte owner has no vptr and four CWideString members; no Ego_r.pdb struct of that
// shape was found, so CRecordWithFourWideStrings is behavioural.
// Shape: user-declared, non-virtual destructor; `delete` on it makes VC7.1 emit the scalar deleting destructor.
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
class CRecordWithFourWideStrings {
public:
    CWideString Text0;                                   // +0x00
    CWideString Text1;                                   // +0x04
    CWideString Text2;                                   // +0x08
    CWideString Text3;                                   // +0x0c
    ~CRecordWithFourWideStrings() {}
};
void DeleteCRecordWithFourWideStrings(CRecordWithFourWideStrings* p)      // emission helper, not a retail function
{
    delete p;
}