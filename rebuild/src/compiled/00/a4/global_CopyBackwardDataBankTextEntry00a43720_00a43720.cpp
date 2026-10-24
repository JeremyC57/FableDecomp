// _global::CopyBackward_DataBankTextEntry_00a43720(NGameText::CDataBankTextEntry*, NGameText::CDataBankTextEntry*, NGameText::CDataBankTextEntry*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the text bank's 8-byte text entries
// Leaf: CopyBackward_DataBankTextEntry_00a43720
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43720, length 94 bytes (+ int3 padding).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// >> 3, both pointers step back one element, the moved-back result is returned.
// Element (Ego_r.pdb): NGameText::CDataBankTextEntry, 8 bytes = CIVCountedPointer<NGameText::CTextEntry> PText +0,
// long NarratorIndex +4 (CDataBank::TextEntryBuffer is a CArray of them; the TU neighbours are its vector helpers,
// 0x00A43890 _Fill / 0x00A438E0 _Move / 0x00A43CD0 insert range). The implicit operator= runs the inlined intrusive
// CIVCountedPointer::operator= (same-object skip, release through virtual slot 1 at refcount 0 and clear the slot,
// adopt + add a reference; modelled as the landed 0x0083CE60 / 0x00BBC310), then copies NarratorIndex.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

struct CIVCountedPointer {                   // CIVCountedPointer<T>, 4 bytes
    CIVCountedPointeeBase* Object;           // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CDataBankTextEntry {                  // NGameText::CDataBankTextEntry, 8 bytes
    CIVCountedPointer PText;                 // +0x00 CIVCountedPointer<NGameText::CTextEntry>
    long              NarratorIndex;         // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CDataBankTextEntry* __fastcall CopyBackward_DataBankTextEntry_00a43720(CDataBankTextEntry* first, CDataBankTextEntry* last, CDataBankTextEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}