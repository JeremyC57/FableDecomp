// CRecordWithWideStringAt10::`scalar deleting destructor'(unsigned int flags) -- destroys the CWideString member at +0x10, frees the object when bit 0 is set
// Leaf: ??_GCRecordWithWideStringAt10@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0043F750, 33 bytes, `ret 4`; one caller, the STLport list<T>::erase at 0x00655A52, which
// destroys a node's value (+8) through it with flags 0 before freeing the node. Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array loop, no vptr). Body: ~CWideString (0x0099B510) on
// +0x10, then `operator delete(this)` (0x00BFE9BC) when (flags & 1); returns this. The owning class has no vptr and no
// other non-trivial member; its name is unknown, so CRecordWithWideStringAt10 is behavioural.
// Shape: a class whose only non-trivial member is the CWideString (user-declared, non-virtual destructor); `delete` on it makes
// VC7.1 emit the scalar deleting destructor, as retail did.
class CWideString {
public:
    void* PData;
    ~CWideString();                                        // 0x0099B510
};
class CRecordWithWideStringAt10 {
public:
    unsigned char _pad00[0x10];
    CWideString Text;                                         // +0x10
    ~CRecordWithWideStringAt10() {}
};
void DeleteCRecordWithWideStringAt10(CRecordWithWideStringAt10* p)                 // emission helper, not a retail function
{
    delete p;
}