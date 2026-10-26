// _global::CopyBackward_PairIntelligentPointerInt_0080dff0(const SPairIntelligentPointerInt* first, const SPairIntelligentPointerInt* last, SPairIntelligentPointerInt* result, const random_access_iterator_tag&, int*) -- assign [first, last) backwards onto the range ending at result ((thing pointer, int) pairs), return the moved-back result
// Leaf: CopyBackward_PairIntelligentPointerInt_0080dff0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080DFF0, length 79 bytes (`ret 0Ch`).
// Identity: catalog `_Move<std::pair<CIntelligentPointer<CThing const>,NStealth::CDangerStatus>*>` is propagated: that pair is
//   16 bytes and the body is a BACKWARD copy (both pointers start at their ends and step down 12 bytes). The 12-byte
//   {CIntelligentPointer, int} element matches the caller label `make_pair<CIntelligentPointer<CThing>,int>` 0x0080E200;
//   the other caller is the vector insert-range 0x0080EF50.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90.
// Shape: SGI `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`:
//   first in ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); both pointers
//   step back one element before each assignment; the moved-back result is returned.
//   last is kept in esi and the count in ebx; the int is copied after the SetPItem call.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

class CBaseObject;

struct CBaseIntelligentPointer {                // vtable +0, item handle +4
    void* PData;                                // +0x04
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing;

struct SPairIntelligentPointerInt {             // std::pair<CIntelligentPointer<CThing>, int>, 12 bytes
    CIntelligentPointer<CThing> first;          // +0x00
    int                         second;         // +0x08
};

SPairIntelligentPointerInt* __fastcall CopyBackward_PairIntelligentPointerInt_0080dff0(const SPairIntelligentPointerInt* first, const SPairIntelligentPointerInt* last, SPairIntelligentPointerInt* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}