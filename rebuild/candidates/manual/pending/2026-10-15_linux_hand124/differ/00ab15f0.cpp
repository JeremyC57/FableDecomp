// CBoneInfo::FindChildIndex(const CCharString& name) -- linear search of a skeleton-builder bone's children for the child with the given name; returns its index or -1
// Grade: DIFFER(109v109) -- register allocation only under /Oa: retail reuses eax for the child pointer chain (`mov eax,[ebx] ; mov eax,[eax]`) and the recomputed count, edx for the length compare; the lane takes edx / ecx. Without /Oa the lane re-reads the name block and begin() every iteration (102v109), so /Oa (no aliasing: the name block hoisted into esi, begin() strength-reduced into ebx) is the module flag
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB15F0, length 109 bytes (ret 4; caller 0x00AB1942).
// Callee: Compare(const char*, const char*) @ 0x00411570 (__fastcall, 0 when equal) inside the inlined
// CCharString operator== (same data block, or both present with equal lengths and Compare == 0; the landed
// CMapBrushList::GetScriptedMapBrush 0x0081E9F0 carries the same inline operator).
// Layout (Ego_r.pdb CBoneInfo of the animation builder): Name +0x00, Index +0x04, Children +0x0c (a CArray of
// CCountedPointer<CBoneInfo>: first +0x0c, last +0x10 in retail; 8-byte elements {object, count block}).
// Shape: the element count `(last - first) >> 3` is recomputed after every miss (the out-of-line Compare may write
// memory) while the element pointer is strength-reduced into ebx; the searched name's data block is loaded once
// (esi) and sits on the LEFT of the comparison (`Compare(name, child)` = ecx/edx); unsigned index and count
// (`jbe`/`jb`), the -1 tail is the fall-through exit.
struct CCharStringData {
    const char*   PData;                      // +0x00
    unsigned long StringLength;               // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->StringLength == b->StringLength)
            return Compare(a->PData, b->PData) == 0;
        return false;
    }
};

class CBoneInfo;
struct CCountedPointerBone {                  // CCountedPointer<CBoneInfo>
    CBoneInfo* m_pObject;                     // +0x00
    void*      m_pCount;                      // +0x04
    CBoneInfo* operator->() const { return m_pObject; }
};
struct CArrayBones {                          // CArray<CCountedPointer<CBoneInfo> >, retail 12 bytes
    CCountedPointerBone* m_pFirst;            // +0x00
    CCountedPointerBone* m_pLast;             // +0x04
    CCountedPointerBone* m_pEnd;              // +0x08
    unsigned long Size() const { return m_pLast - m_pFirst; }
    CCountedPointerBone& operator[](unsigned long index) { return m_pFirst[index]; }
};

class CBoneInfo {
public:
    CCharString   Name;                       // +0x00
    unsigned long Index;                      // +0x04
    unsigned long OriginalNoChildren;         // +0x08
    CArrayBones   Children;                   // +0x0c
    long FindChildIndex(const CCharString& name);
};

long CBoneInfo::FindChildIndex(const CCharString& name)
{
    for (unsigned long i = 0; i < Children.Size(); ++i) {
        if (name == Children[i]->Name)
            return i;
    }
    return -1;
}
