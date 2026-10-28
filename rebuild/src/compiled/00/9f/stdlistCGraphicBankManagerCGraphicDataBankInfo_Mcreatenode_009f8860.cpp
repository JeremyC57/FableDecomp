// std::list<CGraphicBankManager::CGraphicDataBankInfo>::_M_create_node(const CGraphicDataBankInfo&) -- allocates and copy-constructs a list node
// Retail: Fable.exe 0x009F8860, 84 bytes (thiscall, `this` unused, `ret 4`): malloc(0x40) (0x00BFEA0E), then the
// null-tested placement copy into node +8: the 0x2C-byte CGraphicDataBankInit (rep movsd x 0xB), BankHandle
// (CCharString copy ctor 0x0099EC30), GraphicDataBank (CCountedPointer copy: Data, Info, ++Info->RefCount); returns
// the node.
// Identity: its callers 0x009F8985 (list::insert(pos, x), linking the node in) and 0x009F8A78 sit between the
// landed CGraphicBankManager list helpers (erase 0x009F8920, the _Construct 0x009F8780 with this exact copy); the
// 0x38-byte element is Ego_r.pdb CGraphicBankManager::CGraphicDataBankInfo (Init +0, BankHandle +0x2C,
// GraphicDataBank +0x30). STLport 4.6 list::_M_create_node = allocate(1) + _Construct(&__p->_M_data, __x) (no
// try/unwind: the TU builds without EH). Access spelled public like the four bound list _M_create_node rows.
// Leaf: ?_M_create_node@?$list@VCGraphicDataBankInfo@CGraphicBankManager@@
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    void* PStringData;
};

class CCPPointerInfo { public: long RefCount; };

template <class T> class CCountedPointer {
public:
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
};

class CGraphicDataBank;
class CGraphicDataBankInit { public: unsigned long Words[11]; };   // 0x2C bytes, trivially copied

class CGraphicBankManager {
public:
    class CGraphicDataBankInfo {
    public:
        CGraphicDataBankInit              Init;             // +0x00
        CCharString                       BankHandle;       // +0x2C
        CCountedPointer<CGraphicDataBank> GraphicDataBank;  // +0x30
    };
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
    public:
        _Tp _M_data;
        _Tp allocate(unsigned int) { return (_Tp)malloc(0x40); }
    };
    template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

    struct _List_node_base {
        _List_node_base* _M_next;                       // +0x0
        _List_node_base* _M_prev;                       // +0x4
    };
    template <class _Tp> struct _List_node : public _List_node_base {
        _Tp _M_data;                                    // +0x8
    };
    template <class _Tp, class _Alloc> class _List_base {
    public:
        typedef _List_node<_Tp> _Node;
        _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
    };
    template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    public:
        typedef _List_node<_Tp> _Node;
        protected:
        _Node* _M_create_node(const _Tp& __x)
        {
            _Node* __p = this->_M_node.allocate(1);
            _Construct(&__p->_M_data, __x);
            return __p;
        }
        public:
    };

    template class list<CGraphicBankManager::CGraphicDataBankInfo>;
}