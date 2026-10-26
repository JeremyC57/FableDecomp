// std::__unguarded_insertion_sort_aux(SNamedCountedEntry* first, SNamedCountedEntry* last, SNamedCountedEntry*, SNamedCountedEntryLess comp) -- unguarded insertion sort of {CCharString, CCountedPointer} pairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBB2D0, length 69 bytes.
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, _Tp(*i), comp)`
// @ 0x00CBA9B0 (`ret 10h` = 12-byte value + comparator; it assigns the string through CCharString::operator= and the
// counted pointer {object +4, count block +8} through the inline release/add-ref). Element: pair<CCharString,
// CCountedPointer<T> > (callers 0x00CBB917 quicksort partition area and 0x00CBD455); T is not identified.
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the 4-byte empty
// comparator on the stack (`ret 8`).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, std::__unguarded_linear_insert @ 0x00CBA9B0.
// Shape: the value is built in the argument slot by the STLport memberwise pair copy constructor; the counted pointer
// copy constructor copies both words and adds a reference on the block under a null test (`inc dword ptr [eax]`).
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
};
class CCPPointerInfo { public:                     // counted-pointer block
    long  RefCount;                         // +0x00
    void  (__fastcall* DeleteFunc)(void* data);   // +0x04
    void* Data;                             // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                   // +0x00
    CCPPointerInfo* Info;                   // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}
struct CCountedObject;
typedef std::pair<CCharString, CCountedPointer<CCountedObject> > SNamedCountedEntry;
struct SNamedCountedEntryLess { };          // empty comparator
namespace std {
void __fastcall __unguarded_linear_insert(SNamedCountedEntry* last, SNamedCountedEntry val, SNamedCountedEntryLess comp);   // 0x00CBA9B0
void __fastcall __unguarded_insertion_sort_aux(SNamedCountedEntry* first, SNamedCountedEntry* last, SNamedCountedEntry*, SNamedCountedEntryLess comp)
{
    for (SNamedCountedEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SNamedCountedEntry(*i), comp);
}
}