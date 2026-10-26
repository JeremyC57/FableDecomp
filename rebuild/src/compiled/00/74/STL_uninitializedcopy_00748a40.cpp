// std::__uninitialized_copy(pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* last, pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* result, const __false_type&) -- copy-construct the danger-thing entries of [first, last) into raw storage at result; returns the end of the copies
// Leaf: __uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748A40, 84 bytes (+12 int3), `ret 8` (link-time __fastcall: first in ecx, last in
//   edx, result and the __false_type reference on the stack).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90
//   (the inlined CIntelligentPointer<const CThing> copy constructor; its vtable 0x01237F1C has RTTI
//   `.?AV?$CIntelligentPointer@$$CBVCThing@@@@`).
// Callers: the DangerThings vector's _M_insert_overflow 0x00749420 (0x00749475 / 0x007494E0: the two halves around the
//   inserted run).
// Label: the manifest's `_Copy_backward<CIntelligentPointer<CThing>*,...>` is a propagation; the body is STLport
//   `__uninitialized_copy(first, last, result, __false_type)` over CTCStealth's 16-byte danger-thing pairs.
// Shape: `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- placement new tests the
//   destination, the pair copy constructor builds the intelligent pointer (base ctor, vtable, SetPItem(GetPItem())) and
//   copies the CDangerStatus words; `ebx` walks the source's +8 (strength-reduced) and is pushed after the empty-range
//   exit.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                                    // Ego_r.pdb 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long          Timer;                                 // +0x04
};
}

namespace std {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;   // 16 bytes

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template TDangerPair* __fastcall __uninitialized_copy(TDangerPair*, TDangerPair*, TDangerPair*, const __false_type&);
}