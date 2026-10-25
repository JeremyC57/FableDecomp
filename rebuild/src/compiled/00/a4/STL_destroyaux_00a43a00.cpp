// std::__destroy_aux(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last, const __false_type&) -- destroy a run of 8-byte text-bank entries: release each entry's counted text pointer (virtual delete at a zero count) and null it
// Leaf: ??$__destroy_aux@PAVCDataBankTextEntry@NGameText@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A43A00, 50 bytes, `ret 4` (link-time __fastcall: first in ecx, last in edx, the
//   __false_type tag reference on the stack, never read).
// Identity: the `_M_clear` destroy of NGameText::CDataBank's TextEntryBuffer (CArray<CDataBankTextEntry>): the landed
//   vector::_M_insert_overflow 0x00A43B20 and CLipSyncDataBank::~CLipSyncDataBank 0x00A42CE0 / Close 0x00A42DA0 call it;
//   the sibling COMDATs 0x00A43A66..0x00A43B09 (erase / assign of the same vector) too.
// Callees: virtual slot 1 of the pointee (CIVCountedPointeeBase delete) when the reference count reaches zero.
// Layout: Ego_r.pdb CDataBankTextEntry (8 bytes): PText CIVCountedPointer<NGameText::CTextEntry> +0, NarratorIndex +4;
//   CIVCountedPointeeBase: vptr +0, IVRefCount +4. Local model: the generated engine/CIVCountedPointeeBase.h carries
//   the vptr as a data word, and the release needs the real virtual slot 1.
// Shape: STLport 4.x `for (; first != last; ++first) _Destroy(&*first);` with the entry's implicit destructor inlined:
//   ~CIVCountedPointer = `T* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; }`
//   (the pointer copied to a local, or VC7.1 re-reads it after the decrement) (the null store
//   sits inside the test, `je` past it).
// Label correction: catalogued as `vector<NGameText::CDataBankGroupEntry>::_Destroy` (propagated); the element is the
//   8-byte CDataBankTextEntry, not the 4-byte CDataBankGroupEntry.
class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);                 // slot 0
    virtual void Delete();                           // slot 1
    unsigned long IVRefCount;                        // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                       // +0x00
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { };

class CDataBankTextEntry {                           // 8 bytes
public:
    CIVCountedPointer<CTextEntry> PText;             // +0x00
    long                          NarratorIndex;     // +0x04
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __destroy_aux<NGameText::CDataBankTextEntry*>(NGameText::CDataBankTextEntry*, NGameText::CDataBankTextEntry*, const __false_type&);
}