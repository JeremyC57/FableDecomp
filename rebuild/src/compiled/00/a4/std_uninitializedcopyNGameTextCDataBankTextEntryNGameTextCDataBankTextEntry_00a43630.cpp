// _global::UninitCopy_CDataBankTextEntry_00a43630(const CDataBankTextEntry* first, const CDataBankTextEntry* last, CDataBankTextEntry* result, const __false_type&) -- STLport __uninitialized_copy<NGameText::CDataBankTextEntry*>: placement copy-construct text-bank entries (add-ref the text entry), return the advanced result
// Leaf: UninitCopy_CDataBankTextEntry_00a43630
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43630, length 51 bytes (+13 int3 pad), `ret 8`.
// Callers: 0x00A43B20 (vector<CDataBankTextEntry> reallocation: `sar eax,3` = 8-byte elements, catalog
//   "Vector_Reallocate_CDataBankGroupEntry") and 0x00A43CD0 (insert range, catalog "Vector_InsertRange_CDataBankTextEntry").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The 8-byte
//   element is Ego_r.pdb NGameText::CDataBankTextEntry: PText CIVCountedPointer<NGameText::CTextEntry> +0x00 (the
//   intrusive count lives at +0x04 of the pointee), NarratorIndex long +0x04. Byte twin of 0x009CA4C0 (another element
//   of the same shape in another TU).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` in a /Gr TU (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). The implicit memberwise copy constructor of the entry
//   runs CIVCountedPointer's user copy constructor (`P(x.P) { if (P) ++P->RefCount; }`: load, test, store, `inc [esi+4]`)
//   and then the narrator index; placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CTextEntry {                             // NGameText::CTextEntry (intrusively counted)
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};

struct CIVCountedPointer_CTextEntry {           // CIVCountedPointer<NGameText::CTextEntry>
    CTextEntry* P;
    CIVCountedPointer_CTextEntry(const CIVCountedPointer_CTextEntry& x) : P(x.P) { if (P) ++P->RefCount; }
};

struct CDataBankTextEntry {                     // NGameText::CDataBankTextEntry, 8 bytes
    CIVCountedPointer_CTextEntry PText;         // +0x00
    long                         NarratorIndex; // +0x04
};

CDataBankTextEntry* __fastcall UninitCopy_CDataBankTextEntry_00a43630(const CDataBankTextEntry* first, const CDataBankTextEntry* last, CDataBankTextEntry* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CDataBankTextEntry(*first);
    return result;
}