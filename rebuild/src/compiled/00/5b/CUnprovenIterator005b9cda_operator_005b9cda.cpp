// CUnprovenIterator_005b9cda::operator++() -- pre-increment of a list iterator: follows the node's next link (+0)
// Leaf: ??ECUnprovenIterator_005b9cda@@QAEAAV0@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CDA, 9 bytes, thiscall `ret`: mov eax, ecx ; mov ecx, dword ptr [eax] ; mov ecx, dword ptr [ecx] ; mov dword ptr [eax], ecx ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenNode_005b9cda { CUnprovenNode_005b9cda* Next; CUnprovenNode_005b9cda* Prev; };
class CUnprovenIterator_005b9cda {
public:
    CUnprovenIterator_005b9cda& operator++();
    CUnprovenNode_005b9cda* Node;
};

CUnprovenIterator_005b9cda& CUnprovenIterator_005b9cda::operator++()
{
    Node = Node->Next;
    return *this;
}