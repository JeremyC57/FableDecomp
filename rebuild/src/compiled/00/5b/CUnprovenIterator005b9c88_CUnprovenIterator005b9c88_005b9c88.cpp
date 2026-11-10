// CUnprovenIterator_005b9c88::CUnprovenIterator_005b9c88() -- default constructor of a one-word class that clears its pointer
// Leaf: ??0CUnprovenIterator_005b9c88@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C88, 6 bytes, thiscall `ret`: mov eax, ecx ; and dword ptr [eax], 0 ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenIterator_005b9c88 {
public:
    CUnprovenIterator_005b9c88();
    void* Node;
};

CUnprovenIterator_005b9c88::CUnprovenIterator_005b9c88() : Node(0)
{
}