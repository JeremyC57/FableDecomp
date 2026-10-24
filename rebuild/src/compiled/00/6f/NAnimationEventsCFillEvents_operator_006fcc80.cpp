// NAnimationEvents::CFillEvents::operator()(const CEvent& event) -- append (symbol name of the event index, event time) to the functor's vector
// Leaf: ??RCFillEvents@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCC80, 145 bytes, `ret 4`.
// Label correction: catalog "ParticleMorphs_BuildEntry" is propagated; Ego_r.pdb `??RCFillEvents@NAnimationEvents@@`
//   names this body (functor of CManager::GetExtraEvents).
// Callees: CSymbolMap::FindFirstSymbolWithVal 0x009E5090, CCharString copy constructor 0x0099EC30 / destructor
//   0x0099EAE0, vector<pair<CCharString,float> >::_M_insert_overflow 0x006FC130 (same-TU noinline STAND-IN body so the
//   empty tag shares the dead `event` slot).
// Shape: a NAMED pair local `entry(name, event.Time)` (built with the two-argument constructor) pushed back, then
//   destroyed.
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
    // 0x006FC130 (separate function; same-TU stand-in so the empty tag can share the dead `event` slot)
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

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
}

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};

class CFillEvents {                                       // Ego_r.pdb: 8 bytes
public:
    std::vector<std::pair<CCharString, float> >* Events;  // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEvents(std::vector<std::pair<CCharString, float> >& events, const CSymbolMap& symbols) : Events(&events), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event);
};

void CFillEvents::operator()(const CEvent& event)
{
    std::pair<CCharString, float> entry(EventSymbolMap->FindFirstSymbolWithVal(event.Index)->EntryName, event.Time);
    Events->push_back(entry);
}
}