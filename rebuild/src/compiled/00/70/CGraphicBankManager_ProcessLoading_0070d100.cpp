// CGraphicBankManager::ProcessLoading(vector<SBankEntry*>& out) -- appends the address of every entry of the two entry lists (+0x58, +0x64) to the caller's pointer vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D100, length 156 bytes (padded to 160).
// Callees: the 4-byte-element vector's reallocating insert @ 0x0066C3F0 (manifest "std::vector<uint>::_Ufill", the
//          PushBackConstant family's InsertGrow shape: position, value by reference, the empty tag, count 1, true).
// Shape: two STLport list walks; the inlined retail push_back of `&entry` -- the value is an rvalue homed in the dead
// argument slot (the vector pointer was moved to esi), stored in place when there is room (null-checked placement new,
// `add [esi+4],ebp` with the hoisted 4), else handed to the insert with the empty-tag temporary in the same slot.
// Model: local (the list element type is opaque; only its address is used).
#include <new>
struct SScalarTag { SScalarTag() { } };
struct SBankEntry;
struct CRetailVector_ptr {
    SBankEntry** first;
    SBankEntry** last;
    SBankEntry** end;
    void InsertGrow(SBankEntry** where, SBankEntry* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0066C3F0
    void push_back(SBankEntry* const& value)
    {
        if (last != end) {
            new (last) SBankEntry*(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* FindRef_CRetailVector_ptr_InsertGrow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CRetailVector_ptr::InsertGrow(SBankEntry** where, SBankEntry* const& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    void* p = FindRef_CRetailVector_ptr_InsertGrow_0();
    if (!p)
        return;
    if (*(void**)&value != p)
        return;
}
#pragma auto_inline(on)
struct _List_node {
    _List_node*   _M_next;
    _List_node*   _M_prev;
    unsigned char _M_data[4];                      // +0x08: the entry (opaque)
};
struct list {
    _List_node* _M_node;
};
class CGraphicBankManager { public:
    unsigned char _pad00[0x58];
    list          m_loadingA;                      // +0x58
    unsigned char _pad5c[8];
    list          m_loadingB;                      // +0x64
    void ProcessLoading(CRetailVector_ptr& out);
};

void CGraphicBankManager::ProcessLoading(CRetailVector_ptr& out)
{
    for (_List_node* node = m_loadingA._M_node->_M_next; node != m_loadingA._M_node; node = node->_M_next)
        out.push_back((SBankEntry*)node->_M_data);
    for (_List_node* node2 = m_loadingB._M_node->_M_next; node2 != m_loadingB._M_node; node2 = node2->_M_next)
        out.push_back((SBankEntry*)node2->_M_data);
}