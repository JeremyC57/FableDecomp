// CRecordWithCharStringAtC::`scalar deleting destructor'(unsigned int flags) -- destroys the CCharString member at +0xc, frees the object when bit 0 is set
// Leaf: ??_GCRecordWithCharStringAtC@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0043F980, 33 bytes, `ret 4`; no code or data references (an orphaned COMDAT). Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array loop, no vptr). Body: ~CCharString (0x0099EAE0) on
// +0xc, then `operator delete(this)` (0x00BFE9BC) when (flags & 1); returns this. The owning class has no vptr and no
// other non-trivial member; its name is unknown, so CRecordWithCharStringAtC is behavioural.
// Shape: a class whose only non-trivial member is the CCharString (user-declared, non-virtual destructor); `delete` on it makes
// VC7.1 emit the scalar deleting destructor, as retail did.
class CCharString {
public:
    void* PData;
    ~CCharString();                                        // 0x0099EAE0
};
class CRecordWithCharStringAtC {
public:
    unsigned char _pad00[0xc];
    CCharString Text;                                         // +0xc
    ~CRecordWithCharStringAtC() {}
};
void DeleteCRecordWithCharStringAtC(CRecordWithCharStringAtC* p)                 // emission helper, not a retail function
{
    delete p;
}