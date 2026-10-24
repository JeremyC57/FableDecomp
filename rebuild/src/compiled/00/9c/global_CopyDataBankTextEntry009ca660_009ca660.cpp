// _global::Copy_DataBankTextEntry_009ca660(const CDataBankTextEntry* first, const CDataBankTextEntry* last, CDataBankTextEntry* result, const random_access_iterator_tag&, int*) -- assign [first, last) of text-bank entries onto result (counted text pointer + narrator index), return the advanced result
// Leaf: Copy_DataBankTextEntry_009ca660
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CA660, length 88 bytes (`ret 0Ch`).
// Identity: `__copy<NGameText::CDataBankTextEntry*>` (Ego_r.pdb, 8 bytes: CIVCountedPointer<CTextEntry> PText, NarratorIndex);
//   the landed NGameText::CDataBank::CloseTextAndGroups 0x009C8660 calls it from the inlined vector erase. The catalog
//   `_Move` is the Dinkumware spelling.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The implicit operator= runs the inlined intrusive pointer assignment (same-object skip; release through virtual
//   slot 1 when the count reaches zero and clear the slot; adopt + add a reference) and then the plain words.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CIVCountedPointeeBase {                  // intrusive pointee: vtable +0 (slot 1 deletes), reference count +4
    virtual void Destroy(int flags);            // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    unsigned long IVRefCount;                   // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

template <class T>
struct CIVCountedPointer {                      // 4 bytes; operator= as the landed 0x00A43720 / 0x0083CE60
    CIVCountedPointeeBase* Object;              // +0x00
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

namespace NGameText {
struct CTextEntry;
struct CDataBankTextEntry {                     // Ego_r.pdb, 8 bytes
    CIVCountedPointer<CTextEntry> PText;        // +0x00
    long                          NarratorIndex;// +0x04
};
}
using NGameText::CDataBankTextEntry;

CDataBankTextEntry* __fastcall Copy_DataBankTextEntry_009ca660(const CDataBankTextEntry* first, const CDataBankTextEntry* last, CDataBankTextEntry* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}