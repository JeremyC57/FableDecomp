// std::vector::push_back(const TeleportData& x) -- (vector<CQ_AmbushTradersScript::TeleportData>) STLport push_back of a 28-byte teleport record: copy-construct at finish and advance, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@UTeleportData@CQ_AmbushTradersScript@NScript@@V?$allocator@UTeleportData@CQ_AmbushTradersScript@NScript@@@std@@@std@@QAEXABUTeleportData@CQ_AmbushTradersScript@NScript@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0D490, 67 bytes (+ int3 padding), `ret 4`.
// Callees: CQ_AmbushTradersScript::TeleportData::TeleportData(const TeleportData&) @ 0x00F0CA60 (implicit memberwise copy:
//   CCharString 0x0099EC30, two dwords, CCharString, vector<CCharString> copy 0x007ACEA0), vector::_M_insert_overflow(pos, x,
//   __false_type, 1, true) @ 0x00F0D4E0. Callers 0x00F0D241 / 0x00F0E66B (CQ_AmbushTradersScript TU).
// Element: Ego_r.pdb NScript::CQ_AmbushTradersScript::TeleportData {defName, appearanceSeed, health, scriptName,
//   itemsCarried} = 32 bytes in the PDB, 0x1c in retail (STLport vector 12 bytes).
// Shape: STLport 4.6 push_back as the landed vector<CSoundBankEntry>::push_back 0x009936E0, but here the `__false_type()`
//   temporary is zero-stored (`mov byte ptr [esp+1Bh],0` in the `push ecx` frame slot): the tag type has NO user constructor
//   in this TU, so VC7.1 value-initialises it.
#include <new>
#include <stddef.h>

class CCharString {
public:
    void* PStringData;
};

namespace std {

struct __false_type { };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fillLen, bool atEnd = false);   // 0x00F0D4E0
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

}   // namespace std

namespace NScript {
class CQ_AmbushTradersScript {
public:
    struct TeleportData {                          // 0x1c bytes in retail
        CCharString               defName;         // +0x00
        unsigned long             appearanceSeed;  // +0x04
        float                     health;          // +0x08
        CCharString               scriptName;      // +0x0c
        std::vector<CCharString> itemsCarried;    // +0x10
        TeleportData(const TeleportData& other);   // 0x00F0CA60
        ~TeleportData();
    };
};
}

template void std::vector<NScript::CQ_AmbushTradersScript::TeleportData>::push_back(const NScript::CQ_AmbushTradersScript::TeleportData&);