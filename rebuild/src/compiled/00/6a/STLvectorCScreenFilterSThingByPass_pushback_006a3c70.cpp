// std::vector<CScreenFilterSThingByPass>::push_back(const CScreenFilterSThingByPass& x) -- STLport push_back of a 9-byte screen-filter bypass entry (thing pointer + flag): copy-construct it at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3C70, length 92 bytes (+ int3 padding), `ret 4` (thiscall, &x).
// Callers: CTCBulletTime 0x0079D6B0 / 0x0079D9D4 and CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter
//   0x008A99F0 (landed; 0x008A9A44), each pushing a {CIntelligentPointer<CThing>(thing), true} entry.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90
//   (the inline CIntelligentPointer<CThing> copy constructor, vtable 0x01238C6C, as the out-of-line copy 0x004A9730),
//   _M_insert_overflow(pos, x, const __false_type&, 1, true) @ 0x006A3950.
// Label: catalog "std::vector_push_back_CBaseIntelligentPointer" is descriptive.
// Element: 9 bytes (`add dword ptr [edi+4],9`; the callers build it at [ebp-19h], the flag at [ebp-11h]): a packed
//   {CIntelligentPointer<CThing> thing; bool bypassSet;}. The PDBs (Ego_r / FableWin) name the screen filter's bypass
//   vector element CScreenFilterSThingByPass {CScriptThing thing; bool bypassSet;} (16 bytes) -- the TLC element holds
//   the thing's intelligent pointer instead, under pack(1); the type name is the PDB's, the layout the retail stride's.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement copy is null tested, the __false_type
//   temporary sits in the dead `&x` argument slot (`lea ecx,[esp+14h]`).
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // slot 0 (0x004A9710 in CIntelligentPointer<CThing>'s table)
    CBaseObject* PData;                                     // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

#pragma pack(push, 1)
struct CScreenFilterSThingByPass {                          // TLC layout, 9 bytes
    CIntelligentPointer<CThing> thing;                      // +0x00
    bool                        bypassSet;                  // +0x08
};
#pragma pack(pop)

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x006A3950
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template void std::vector<CScreenFilterSThingByPass>::push_back(const CScreenFilterSThingByPass&);