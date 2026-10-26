// Behaviour test for 00ab15f0: src/00ab15f0.cpp copied verbatim; returns the index of the first child bone whose name
// equals the given one (same block, or same length and text), else -1.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBoneInfo::FindChildIndex(const CCharString& name) -- linear search of a skeleton-builder bone's children for the child with the given name; returns its index or -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB15F0, length 109 bytes (ret 4; caller 0x00AB1942).
// Callee: Compare(const char*, const char*) @ 0x00411570 (__fastcall, 0 when equal) inside the inlined
// CCharString operator== (same data block, or both present with equal lengths and Compare == 0; the landed
// CMapBrushList::GetScriptedMapBrush 0x0081E9F0 carries the same inline operator).
// Layout (Ego_r.pdb CBoneInfo of the animation builder): Name +0x00, Index +0x04, Children +0x0c (a CArray of
// CCountedPointer<CBoneInfo>: first +0x0c, last +0x10 in retail; 8-byte elements {object, count block}).
// Shape: Compare gets its landed body as a same-TU `auto_inline(off)` stand-in. VC7.1 then knows the call writes no
//   memory, so the searched name's block is loaded once (esi) and the element pointer is strength-reduced into ebx (no
//   /Oa needed). The element count `(last - first) >> 3` is still recomputed after every miss. The inline CCharString ==
//   is the MsgIsKilledBy 0x008D04E0 form (identity, null tests and length in one condition, then `Compare(...) ? false :
//   true`), which gives retail's eax reuse for the child chain. The name sits on the LEFT (`Compare(name, child)` =
//   ecx/edx); unsigned index and count (`jbe`/`jb`); the -1 tail is the fall-through exit.
class CCharStringData { public:
    const char*   PData;                      // +0x00
    unsigned long StringLength;               // +0x04
};
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body, same-TU stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->StringLength == other.PStringData->StringLength)
            return Compare(PStringData->PData, other.PStringData->PData) ? false : true;
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
// ---- end source ----
static CCharStringData Make(const char* t) { CCharStringData d; d.PData = t; d.StringLength = (unsigned long)strlen(t); return d; }
int main()
{
    CCharStringData hip = Make("Hip"), knee = Make("Knee"), knee2 = Make("Knee"), kneX = Make("KneX"), foot = Make("Foot");
    CBoneInfo parent, c0, c1, c2;
    memset(&parent, 0, sizeof(parent)); memset(&c0, 0, sizeof(c0)); memset(&c1, 0, sizeof(c1)); memset(&c2, 0, sizeof(c2));
    c0.Name.PStringData = &hip; c1.Name.PStringData = &knee; c2.Name.PStringData = 0;
    CCountedPointerBone kids[3] = { { &c0, 0 }, { &c1, 0 }, { &c2, 0 } };
    parent.Children.m_pFirst = kids; parent.Children.m_pLast = kids + 3; parent.Children.m_pEnd = kids + 3;
    CCharString q;
    q.PStringData = &knee2; CHECK(parent.FindChildIndex(q) == 1);      // equal text, different block
    q.PStringData = &hip;   CHECK(parent.FindChildIndex(q) == 0);      // same block
    q.PStringData = &kneX;  CHECK(parent.FindChildIndex(q) == -1);     // same length, different text
    q.PStringData = &foot;  CHECK(parent.FindChildIndex(q) == -1);
    q.PStringData = 0;      CHECK(parent.FindChildIndex(q) == 2);      // null block matches the null-named child
    parent.Children.m_pLast = kids; CHECK(parent.FindChildIndex(q) == -1);   // no children
    if (fails == 0) printf("00ab15f0_TEST PASS\n"); else printf("00ab15f0_TEST FAIL\n");
    return fails;
}