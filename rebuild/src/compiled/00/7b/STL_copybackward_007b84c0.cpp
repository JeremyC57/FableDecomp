// std::__copy_backward(CIntelligentPointer<CThingObject>* first, CIntelligentPointer<CThingObject>* last, CIntelligentPointer<CThingObject>* result, const random_access_iterator_tag&, int*) -- STLport random-access backward copy of thing-object intelligent pointers: n = last - first assignments from the back, returns the new start of the output
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B84C0, 62 bytes, `ret 0Ch` (/Gr __fastcall: first in ecx, last in edx
//   -> esi, then result, the iterator-category tag reference and the distance-type pointer on the stack).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (the inline CIntelligentPointer::operator=).
// Identity: byte twin of 0x007AFA50 (the CThingAICreature instantiation) in the CTCTavernTable TU: the
//   vector<CIntelligentPointer<CThingObject> > (TU vtable 0x0125A858; callers 0x007B966C / 0x007B9BD3). Catalog label
//   "CopyArray_Backward_CBaseIntelligentPointer" is descriptive.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   -- both pointers pre-decremented before the GetPItem/SetPItem pair, the count in ebx.
class CThingObject;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}

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
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace std {
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template CIntelligentPointer<CThingObject>* __fastcall __copy_backward<CIntelligentPointer<CThingObject>*, CIntelligentPointer<CThingObject>*, int>(
    CIntelligentPointer<CThingObject>*, CIntelligentPointer<CThingObject>*, CIntelligentPointer<CThingObject>*,
    const random_access_iterator_tag&, int*);
}