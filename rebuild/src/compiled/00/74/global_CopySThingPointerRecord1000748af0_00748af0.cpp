// _global::Copy_SThingPointerRecord10_00748af0(const SThingPointerRecord10* first, const SThingPointerRecord10* last, SThingPointerRecord10* result, const random_access_iterator_tag&, int*) -- STLport __copy over 16-byte {intelligent thing pointer, 8-byte word pair} records: counted forward assignment, returns the advanced result
// Leaf: Copy_SThingPointerRecord10_00748af0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00748AF0, length 97 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Caller: the STLport `__copy_ptrs(first, last, result, const __false_type&)` @ 0x00748D60 (pushes `0`, `lea` of a dead
// slot, result) -- confirms the tag-reference + null distance-pointer signature.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed count
// `last - first` (`sar 4` = /16) computed once and kept in the dead `result` argument slot, then the record's assignment
// inlined per element: the intelligent pointer's SetPItem(src.GetPItem()) (`mov ecx,ebx ; call ; push eax ; mov ecx,ebp ;
// call`), then the 8-byte POD sub-record at +8 by its implicit assignment -- that nested record is what gives the
// strength-reduced src+8/dst+8 pointers (esi/edi) and the four-pointer loop; two separate words at +8/+0xC instead
// compile to `[esi+8]`/`[edi+8]` off the element pointers (72 bytes). Advanced result returned.
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on the stack.
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated (CTargetPoint is 28
// bytes); the 16-byte element is not named by any landed relative -- descriptive member names.
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct SWordPair08 {                             // 8 bytes, POD: copied by the implicit assignment as a nested record
    long m_first;
    long m_second;
};

struct SThingPointerRecord10 {                   // 16 bytes
    CIntelligentPointer<CThing> PThing;          // +0x00
    SWordPair08                 m_values;        // +0x08
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SThingPointerRecord10* __fastcall Copy_SThingPointerRecord10_00748af0(const SThingPointerRecord10* first, const SThingPointerRecord10* last, SThingPointerRecord10* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}