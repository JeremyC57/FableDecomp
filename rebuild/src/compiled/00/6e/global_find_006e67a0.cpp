// std::find<CBaseIntelligentPointer*, CBaseObject*> -- STLport `__find`: four-way unrolled scan of an intelligent-pointer range for the element whose held object is `value`
// Leaf: Find_006e67a0
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E67A0, 192 bytes (`ret 8`: the value reference + an unused iterator tag).
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (returns the held CBaseObject*).
// Shape: the algorithm TU is /Gr-style __fastcall (first in ecx, last in edx). A loop over groups of four
//          comparisons (count = (last-first)>>5 for 8-byte elements) then a switch on the 0..3 remaining elements.
//          Each compare goes through the inline operator==(pointer, CBaseObject*): the by-value object argument is
//          read through the reference into a register before the GetPItem call. Manifest label "CBaseIntelligentPointer::Find" is the propagated algorithm name.
struct CBaseObject;

struct CBaseIntelligentPointer {                    // 8 bytes
    CBaseObject* m_pItem;                           // +0x00
    void* m_pNext;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

// the engine's comparison of an intelligent pointer with a raw object pointer (object taken by value)
inline bool operator==(const CBaseIntelligentPointer& pointer, CBaseObject* object) { return pointer.GetPItem() == object; }

struct CIteratorTag { };

CBaseIntelligentPointer* __fastcall Find_006e67a0(CBaseIntelligentPointer* first, CBaseIntelligentPointer* last,
                                                  CBaseObject* const& value, CIteratorTag tag)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        return last;
    }
}