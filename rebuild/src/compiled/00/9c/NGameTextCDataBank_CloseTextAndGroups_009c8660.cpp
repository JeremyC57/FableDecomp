// NGameText::CDataBank::CloseTextAndGroups() -- empty the entry table and the text and group entry buffers (STLport erase(begin(), end()) on each: move the empty tail down, destroy the rest, rewrite the last pointer)
// Leaf: ?CloseTextAndGroups@CDataBank@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8660, 154 bytes (+6 int3 padding), plain `ret`.
// Name: FableWin.pdb ?CloseTextAndGroups@CDataBank@NGameText@@IAEXXZ (protected, non-virtual). The catalog label
// "CommitChanges" is an autoname (labels_forge_autoname2); the only callers are CDataBank::Close 0x009C8BF0 (vtable slot 5:
// `CloseTextAndGroups(); CBankFile::Close();`) and 0x009C87DA -- see NOTES label corrections.
// Callees: memmove @ 0x00BFEAE6 (the scalar pointer table's inline move), `__fastcall __copy(first, last, result, const random_access_iterator_tag&, int*)`
// @ 0x009CA660 / 0x009CA6C0 and `__fastcall _Destroy(first, last, allocator tag)` @ 0x009CABC0 / 0x009CAC00 for the two
// 8-byte counted-pointer entry buffers.
// Layout: Entries +0x1E0, TextEntryBuffer +0x1EC, GroupEntryBuffer +0x1F8 (retail 12-byte CArrays; see the ctor 0x009C85A0).
// Shape: the two entry buffers go through an inline STLport `erase(first, last)` called as `clear() { erase(First, Last); }`
// (`__copy(last, Last, first, tag, (int*)0)`, `_Destroy(kept, Last, alloc)`, `Last = kept`; both empty tags homed in the high byte
// of the one frame slot, as the landed CLipSyncDataBank::Close 0x00A42DA0) -- explicit locals hoist the loads above the `push 0`;
// the pointer table's scalar move is inlined with its member operands read in place: `if (first != last)` on the two
// identical end pointers is kept as `cmp ecx,ecx ; je` (one local for both folds the test away), then memmove + n.
// Fixed (rename1, 2026-10-24): the out-of-line copy 0x009CA660 / 0x009CA6C0 was declared as `_Move(first, last, dest, tag&, bool)`
//   (the Dinkumware spelling); it is STLport's counted `__copy(first, last, result, const random_access_iterator_tag&,
//   int*)` (the landed callee body has that signature; the trailing word is the null distance pointer, pushed as 0
//   either way), and the erase(begin(), end()) around it is STLport's, not VC6/VC7.1 Dinkumware. Bytes unchanged.
typedef unsigned int size_t;
extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);

namespace NGameText {
class CDataBankEntry;
class CDataBankTextEntry { public: unsigned char m_body[8]; };
class CDataBankGroupEntry { public: unsigned char m_body[8]; };
}

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct _Allocator_tag { _Allocator_tag() { } };

template<class T> inline T* _Move_scalar(T* first, T* last, T* dest)
{
    if (first != last) {
        size_t n = (char*)last - (char*)first;
        dest = (T*)((char*)memmove(dest, first, n) + n);
    }
    return dest;
}

NGameText::CDataBankTextEntry* __fastcall __copy(const NGameText::CDataBankTextEntry* first, const NGameText::CDataBankTextEntry* last,
    NGameText::CDataBankTextEntry* result, const random_access_iterator_tag&, int*);                                  // 0x009CA660
NGameText::CDataBankGroupEntry* __fastcall __copy(const NGameText::CDataBankGroupEntry* first, const NGameText::CDataBankGroupEntry* last,
    NGameText::CDataBankGroupEntry* result, const random_access_iterator_tag&, int*);                                 // 0x009CA6C0
void __fastcall _Destroy(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last, const _Allocator_tag& allocator);   // 0x009CABC0
void __fastcall _Destroy(NGameText::CDataBankGroupEntry* first, NGameText::CDataBankGroupEntry* last, const _Allocator_tag& allocator); // 0x009CAC00

template<class T> class CArray { public:                       // retail CArray: 12 bytes
    T* First;
    T* Last;
    T* End;
    T* erase(T* first, T* last)
    {
        T* kept = __copy(last, Last, first, random_access_iterator_tag(), (int*)0);
        _Destroy(kept, Last, _Allocator_tag());
        Last = kept;
        return first;
    }
    void clear() { erase(First, Last); }
};

namespace NGameText {
class CDataBank {
public:
    unsigned char _bases[0x1e0];                        // CBankFileAsync +0, CResourceBank +0x164
    CArray<CDataBankEntry*>     Entries;                // +0x1e0
    CArray<CDataBankTextEntry>  TextEntryBuffer;        // +0x1ec
    CArray<CDataBankGroupEntry> GroupEntryBuffer;       // +0x1f8
protected:
    void CloseTextAndGroups();
};

void CDataBank::CloseTextAndGroups()
{
    Entries.Last = _Move_scalar(Entries.Last, Entries.Last, Entries.First);

    TextEntryBuffer.clear();
    GroupEntryBuffer.clear();
}
} // namespace NGameText