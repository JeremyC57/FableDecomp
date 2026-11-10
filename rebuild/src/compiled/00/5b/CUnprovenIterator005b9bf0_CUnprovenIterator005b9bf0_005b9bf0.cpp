// CUnprovenIterator_005b9bf0::CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node) -- stores the node pointer (a one-word iterator or handle constructed from a node pointer)
// Leaf: ??0CUnprovenIterator_005b9bf0@@QAE@PAUCUnprovenNode_005b9bf0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9BF0, 11 bytes, thiscall `ret 4`: mov eax, ecx ; mov ecx, dword ptr [esp + 4] ; mov dword ptr [eax], ecx ; ret 4
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenNode_005b9bf0;
class CUnprovenIterator_005b9bf0 {
public:
    CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node);
    CUnprovenNode_005b9bf0* Node;
};

CUnprovenIterator_005b9bf0::CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node) : Node(node)
{
}