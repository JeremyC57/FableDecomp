// NGameText::CDataBank::CloseTextAndGroups() -- empty the entry table and the text and group entry buffers (VC7.1 Dinkumware erase(begin(), end()) on each: move the empty tail down, destroy the rest, rewrite the last pointer)
// Leaf: ?CloseTextAndGroups@CDataBank@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8660, 154 bytes (+6 int3 padding), plain `ret`.
// Name: FableWin.pdb ?CloseTextAndGroups@CDataBank@NGameText@@IAEXXZ (protected, non-virtual). The catalog label
// "CommitChanges" is an autoname (labels_forge_autoname2); the only callers are CDataBank::Close 0x009C8BF0 (vtable slot 5:
// `CloseTextAndGroups(); CBankFile::Close();`) and 0x009C87DA -- see NOTES label corrections.
// Callees: memmove @ 0x00BFEAE6 (the scalar pointer table's inline move), `__fastcall _Move(first, last, dest, tag, bool)`
// @ 0x009CA660 / 0x009CA6C0 and `__fastcall _Destroy(first, last, allocator tag)` @ 0x009CABC0 / 0x009CAC00 for the two
// 8-byte counted-pointer entry buffers.
// Layout: Entries +0x1E0, TextEntryBuffer +0x1EC, GroupEntryBuffer +0x1F8 (retail 12-byte CArrays; see the ctor 0x009C85A0).
// Shape: the two entry buffers go through an inline Dinkumware `erase(first, last)` called as `clear() { erase(First, Last); }`
// (`_Move(last, Last, first, tag, false)`, `_Destroy(kept, Last, alloc)`, `Last = kept`; both empty tags homed in the high byte
// of the one frame slot, as the landed CLipSyncDataBank::Close 0x00A42DA0) -- explicit locals hoist the loads above the `push 0`;
// the pointer table's scalar move is inlined with its member operands read in place: `if (first != last)` on the two
// identical end pointers is kept as `cmp ecx,ecx ; je` (one local for both folds the test away), then memmove + n.
typedef unsigned int size_t;
extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);

namespace NGameText {
class CDataBankEntry;
struct CDataBankTextEntry { unsigned char m_body[8]; };
struct CDataBankGroupEntry { unsigned char m_body[8]; };
}

struct _Nonscalar_ptr_iterator_tag { _Nonscalar_ptr_iterator_tag() { } };
struct _Allocator_tag { _Allocator_tag() { } };

template<class T> inline T* _Move_scalar(T* first, T* last, T* dest)
{
    if (first != last) {
        size_t n = (char*)last - (char*)first;
        dest = (T*)((char*)memmove(dest, first, n) + n);
    }
    return dest;
}

NGameText::CDataBankTextEntry* __fastcall _Move(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last,
    NGameText::CDataBankTextEntry* dest, const _Nonscalar_ptr_iterator_tag& tag, bool overlap);                       // 0x009CA660
NGameText::CDataBankGroupEntry* __fastcall _Move(NGameText::CDataBankGroupEntry* first, NGameText::CDataBankGroupEntry* last,
    NGameText::CDataBankGroupEntry* dest, const _Nonscalar_ptr_iterator_tag& tag, bool overlap);                      // 0x009CA6C0
void __fastcall _Destroy(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last, const _Allocator_tag& allocator);   // 0x009CABC0
void __fastcall _Destroy(NGameText::CDataBankGroupEntry* first, NGameText::CDataBankGroupEntry* last, const _Allocator_tag& allocator); // 0x009CAC00

template<class T> struct CArray {                       // retail CArray: 12 bytes
    T* First;
    T* Last;
    T* End;
    T* erase(T* first, T* last)
    {
        T* kept = _Move(last, Last, first, _Nonscalar_ptr_iterator_tag(), false);
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