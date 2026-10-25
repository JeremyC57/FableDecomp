// std::__copy(CIntelligentPointer<CThingAICreature>* first, CIntelligentPointer<CThingAICreature>* last, CIntelligentPointer<CThingAICreature>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of AI-creature intelligent pointers: n = last - first assignments, returns the end of the output
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AFA10, 60 bytes (+4 int3), `ret 0Ch` (/Gr __fastcall: first in ecx -> esi, last
//   in edx, then result, the iterator-category tag reference and the distance-type pointer on the stack).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (the inline CIntelligentPointer::operator=).
// Identity: byte twin of the landed STLport __copy<CIntelligentPointer<CThing>*> 0x004AA610; this copy belongs to the
//   vector<CIntelligentPointer<CThingAICreature> > TU at 0x007AF8F0.. (callers 0x007AFB5C / 0x007AFC59 = its erase and
//   assignment paths; the TU's element copies store vtable 0x0123E7E0). Catalog label
//   "CopyArray_Forward_CBaseIntelligentPointer" is descriptive.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` -- the count in ebx, `result` read straight from its slot on the empty path.
class CThingAICreature;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThingAICreature* PItem;                             // +0x04
    CThingAICreature* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CThingAICreature* item);               // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CIntelligentPointer<CThingAICreature>* __fastcall __copy<CIntelligentPointer<CThingAICreature>*, CIntelligentPointer<CThingAICreature>*, int>(
    CIntelligentPointer<CThingAICreature>*, CIntelligentPointer<CThingAICreature>*, CIntelligentPointer<CThingAICreature>*,
    const random_access_iterator_tag&, int*);
}