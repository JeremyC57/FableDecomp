// std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>::clear() @ 0x00618170 (37 B, thiscall, `ret`)
// Leaf: ?clear@?$vector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$allocator@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@std@@@std@@QAEXXZ
// Identity (NF385): the sole retail caller is CPersistContext::TransferVector<ECategory> 0x00619890 (call #2 at 0x006198AF, after
// CheckCRC); the FableWin body of ??$TransferVector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@CPersistContext@@... (tc_inventory_map.obj)
// makes the same calls in the same order: CheckCRC, then ?clear@?$vector@W4ECategory@...@std@@QAEXXZ (FableWin.pdb 0x020AFFA0), then
// GFSerialiseVectorBinaryOut / In. The body is STLport erase(begin(), end()) for a 4-byte POD: copy [end, end) to begin (a dead loop),
// then _M_finish = the copy's end. Was the propagated stand-in ?CComponent_SetParentColour@@YIXPAUCComponent@@@Z (shared with
// 0x0074D5D0 / 0x00411980, which are other vector<T>::clear instances).
class CHeroLogBook { public:
    class CHeroLogBookEntry { public:
        enum ECategory { ECategory_Default };
    };
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;            // +0x0
    T* _M_finish;           // +0x4
    T* _M_end_of_storage;   // +0x8
    void clear();
};
}

static void copy_range(int* first, int* last, int* dst, std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>* self)
{
    int n = (int)(last - first);
    while (n > 0) {
        *dst = *first;
        ++first;
        ++dst;
        --n;
    }
    self->_M_finish = (CHeroLogBook::CHeroLogBookEntry::ECategory*)dst;
}

template <> void std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>::clear()
{
    copy_range((int*)_M_finish, (int*)_M_finish, (int*)_M_start, this);
}
