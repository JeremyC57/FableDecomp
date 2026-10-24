// NCombatActionHelpers::GetDustCreationHelperNames(const CThing&, vector<CCharString>&) -- append the two foot helper names ("weapon_pos_left_foot", "weapon_pos_right_foot")
// Leaf: ?GetDustCreationHelperNames@
// Retail 0x008F78E0, 0x70 bytes, __fastcall bool (thing in ecx, unused; the name vector in edx; plain `ret`). Manifest
//   label "CreateFootEffect_PushStrings" is descriptive; the FableWin PDB name set has
//   `?GetDustCreationHelperNames@NCombatActionHelpers@@YI_NABVCThing@@AAV?$vector@VCCharString@@...@std@@@Z`.
// Callees: CCharString copy ctor @ 0x0099EC30 (placement copy), vector<CCharString>::_M_insert_overflow @ 0x00433530.
// Globals: the two file-static helper names g_CCharString_013baff4 "weapon_pos_left_foot" and g_CCharString_013baff8
//   "weapon_pos_right_foot" (their landed dynamic initializers name them).
// Shape: two inline STLport push_backs (`finish != end_of_storage` -> null-tested placement copy, `add [esi+4],4`, else
//   _M_insert_overflow(finish, x, __false_type(), 1, true) with the empty tag in the `push ecx` frame word), then
//   `return true`.
inline void* operator new(unsigned int, void* __p) { return __p; }

class CCharString {
public:
    void* PStringData;                          // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

extern CCharString g_CCharString_013baff4;      // 0x013BAFF4 "weapon_pos_left_foot"
extern CCharString g_CCharString_013baff8;      // 0x013BAFF8 "weapon_pos_right_foot"

namespace _STL {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);      // 0x00433530 for CCharString
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

class CThing;

namespace NCombatActionHelpers {
bool __fastcall GetDustCreationHelperNames(const CThing& thing, _STL::vector<CCharString>& helperNames)
{
    helperNames.push_back(g_CCharString_013baff4);
    helperNames.push_back(g_CCharString_013baff8);
    return true;
}
}