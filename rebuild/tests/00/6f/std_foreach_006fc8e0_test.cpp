// Behaviour test for 006fc8e0 (std::for_each with NAnimationEvents::CFillEventNames: the symbol name of every event index is appended in order, and the functor comes back by value).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gLiveStrings = 0;

class CCharString {                             // stand-in: a counted handle over a literal
public:
    const char* Text;
    CCharString(const char* text) : Text(text) { ++gLiveStrings; }
    CCharString(const CCharString& other) : Text(other.Text) { ++gLiveStrings; }   // 0x0099EC30
    ~CCharString() { --gLiveStrings; }                                            // 0x0099EAE0
};

class CSymbolInfo {
public:
    CCharString GroupName;
    CCharString EntryName;
    long Val;
    CSymbolInfo(const char* group, const char* entry, long val) : GroupName(group), EntryName(entry), Val(val) {}
};

class CSymbolMap {                              // FindFirstSymbolWithVal 0x009E5090 stand-in over a fixed table
public:
    const CSymbolInfo* Symbols;
    int Count;
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const
    {
        for (int i = 0; i < Count; ++i)
            if (Symbols[i].Val == value)
                return Symbols + i;
        return 0;
    }
};

struct __false_type { __false_type() {} };
template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector()
    {
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        std::free(_M_start);
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {   // the real grow reduced to the push_back case
        if (!atend || fill_len != 1 || position != _M_finish)
            return;
        long size = (long)(_M_finish - _M_start);
        long len = size ? 2 * size : 1;
        T* fresh = (T*)std::malloc(len * sizeof(T));
        for (long i = 0; i < size; ++i) {
            _Construct(fresh + i, _M_start[i]);
            _M_start[i].~T();
        }
        _Construct(fresh + size, x);
        std::free(_M_start);
        _M_start = fresh;
        _M_finish = fresh + size + 1;
        _M_end_of_storage = fresh + len;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
    long size() const { return (long)(_M_finish - _M_start); }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

namespace NAnimationEvents {
class CEvent {
public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;
};
}

static void SetEvent(NAnimationEvents::CEvent& e, unsigned long index, float time)
{
    e.Index = index;
    e.ExternallyImported = 1;
    e.StopEvent = 1;
    e.Time = time;
}

namespace NAnimationEvents {
class CFillEventNames {
public:
    vector<CCharString>* EventNames;
    const CSymbolMap* EventSymbolMap;
    CFillEventNames(vector<CCharString>& names, const CSymbolMap& symbols) : EventNames(&names), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event)
    {
        EventNames->push_back(EventSymbolMap->FindFirstSymbolWithVal(event.Index)->EntryName);
    }
};
}
template<class InputIter, class Function> Function for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

int main()
{
    CSymbolInfo* table = (CSymbolInfo*)std::malloc(3 * sizeof(CSymbolInfo));
    new (table + 0) CSymbolInfo("events", "footstep", 3);
    new (table + 1) CSymbolInfo("events", "swoosh", 0x3fffffff);
    new (table + 2) CSymbolInfo("events", "impact", 7);
    CSymbolMap symbols;
    symbols.Symbols = table;
    symbols.Count = 3;
    NAnimationEvents::CEvent events[3];
    SetEvent(events[0], 7, 0.25f);
    SetEvent(events[1], 0x3fffffff, 0.5f);
    SetEvent(events[2], 3, 0.75f);
    int ok = 1;
    {
        vector<CCharString> names;
        NAnimationEvents::CFillEventNames f = for_each((const NAnimationEvents::CEvent*)events, (const NAnimationEvents::CEvent*)events + 3,
                                                       NAnimationEvents::CFillEventNames(names, symbols));
        if (f.EventNames != &names || f.EventSymbolMap != &symbols || names.size() != 3 ||
            std::strcmp(names._M_start[0].Text, "impact") || std::strcmp(names._M_start[1].Text, "swoosh") ||
            std::strcmp(names._M_start[2].Text, "footstep") || gLiveStrings != 9)
            ok = 0;
    }
    if (gLiveStrings != 6)
        ok = 0;
    if (!ok) {
        std::printf("006fc8e0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fc8e0_TEST PASS\n");
    return 0;
}