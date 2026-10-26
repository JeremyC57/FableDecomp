// std::__copy(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, const random_access_iterator_tag&, int*) -- STLport random-access copy of 8-byte intelligent pointers: n = last - first assignments (SetPItem(src.GetPItem())), returns the end of the output
// Flags: /Gr
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA610, 60 bytes, `ret 0Ch` (__fastcall: first in ecx -> esi, last in edx,
//   then result, the iterator-category tag reference and the distance-type pointer on the stack).
// Callees: 0x00A01B50 CBaseIntelligentPointer::GetPItem() const, 0x00A01B90 CBaseIntelligentPointer::SetPItem(CThing*)
//   (the inline CIntelligentPointer::operator=, as in the landed CThingSoundEmitter 0x00836A50 model).
// Callers: five (LTextTreeWalkThrough::BuildTreeArray 0x004ABAFC, a vector erase 0x004AC109, CTCInventoryQuests
//   0x0061A337, 0x0062413A, 0x0077D44B) -- the body is shared by every CIntelligentPointer<T> instantiation (identical
//   code; the element type here is the CThing one).
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
//   return __result;` -- the count in ebx, `result` read straight from its slot on the empty path.
// Label correction: catalogued as `CBaseIntelligentPointer_CopyRange` (right idea); the body is STLport `__copy`.
class CThing;

struct random_access_iterator_tag { random_access_iterator_tag() { } };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                       // +0x04
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

typedef CIntelligentPointer<CThing> TPointer;

TPointer* __fastcall __copy(TPointer* __first, TPointer* __last, TPointer* __result, const random_access_iterator_tag&, int*)
{
    for (int __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}