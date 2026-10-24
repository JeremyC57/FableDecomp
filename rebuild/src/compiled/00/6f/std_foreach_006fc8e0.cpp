// std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last, CFillEventNames f) -- push the symbol name of every event index onto the functor's name vector, return the functor
// Leaf: ??$for_each@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC8E0, 112 bytes, `ret 0xc` (/Gr: hidden functor result in ecx, first in edx).
// Label correction: catalog "_For_each<CThingCreatureBase**,CApplyAdditionalImpulseIfInCombatAndTooClose>" is
//   propagated; this is std::for_each<const NAnimationEvents::CEvent*, NAnimationEvents::CFillEventNames>.
// Callees: CSymbolMap::FindFirstSymbolWithVal 0x009E5090, CCharString copy constructor 0x0099EC30,
//   vector<CCharString>::_M_insert_overflow 0x00433530 (same-TU noinline STAND-IN body -- only its call matters -- so
//   the empty tag shares the live `last` slot, GOTCHAS b48).
// Shape: CFillEventNames::operator() inlined as `EventNames->push_back(map->FindFirstSymbolWithVal(event.Index)->
//   EntryName)`; the 30-bit Index field gives the `and eax,3FFFFFFFh`.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString {                                       // 4 bytes
public:
    void* PStringData;                                    // +0x00
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};

class CSymbolInfo {                                       // 12 bytes
public:
    CCharString GroupName;                                // +0x00
    CCharString EntryName;                                // +0x04
    long Val;                                             // +0x08
};

class CSymbolMap {                                        // retail 0x24 bytes
public:
    unsigned char _body[0x24];
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const;   // 0x009E5090
};

namespace std {
struct __false_type { __false_type() {} };

template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
#pragma auto_inline(off)
    // 0x00433530 (separate function; same-TU stand-in so the empty tag can share the live `last` slot, GOTCHAS b48)
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        if (atend && fill_len == 1 && position == _M_finish)
            _Construct(_M_start, x);
    }
#pragma auto_inline(on)
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

template<class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
}

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};

class CFillEventNames {                                   // Ego_r.pdb: 8 bytes
public:
    std::vector<CCharString>* EventNames;                 // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEventNames(std::vector<CCharString>& names, const CSymbolMap& symbols) : EventNames(&names), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event)
    {
        EventNames->push_back(EventSymbolMap->FindFirstSymbolWithVal(event.Index)->EntryName);
    }
};
}

template NAnimationEvents::CFillEventNames __fastcall std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last,
                                                                    NAnimationEvents::CFillEventNames f);