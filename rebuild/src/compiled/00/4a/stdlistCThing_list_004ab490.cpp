// std::list<CThing*>::~list() -- STLport list destructor: clear() (free every node after the header, the header pointer re-read after each free, relink the header to itself), then free the header
// Leaf: ??1?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB490, 62 bytes, thiscall `ret`.
// Identity (sweep3; FINDINGS chunk 478 "0x4AB490 = list<CThing*>::~list"; was _global::StdList_Clear_004ab490, source
//   CGuiControlTreePane::SortTreeRecursively): ~CTCCreatureItemLevitationBase 0x004DE6ED calls it on this+0x0C and then
//   tail-jumps to ~CTCBase 0x00686830; Ego_r.pdb CTCCreatureItemLevitationBase has list<CThing*> LevitatingItems at +0x0C
//   (right after the 0xC-byte CTCBase). Other callers: CWorld::LoadGameState (0x004A4CDF, 0x004A5499), CWorldMap::
//   CompileLoadableGlobalThingsToSingleFile 0x004FE1F3, CTCSummonSpell (0x0079AA69), CTCAssassinRush (0x007AA9B7) -- the
//   pointee is never read, the element is a plain pointer (no element destructor in the loop). Retail has no ICF, so the
//   one body is the one list<CThing*> instance.
// Callee: free @ 0x00BFEA14 (STLport malloc allocator; the header behind a null test).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
#define FREE free
namespace std {
template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list: one header-node pointer
public:
    _List_node_base* _M_node;                           // +0x00

    void clear()
    {
        _List_node_base* __cur = _M_node->_M_next;
        while (__cur != _M_node) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            FREE(__tmp);                                // _Destroy(CThing**) is empty; deallocate(tmp, 1)
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            FREE(_M_node);                              // the base destructor: deallocate(header, 1)
    }
};
}

class CThing;

template std::list<CThing*>::~list();
