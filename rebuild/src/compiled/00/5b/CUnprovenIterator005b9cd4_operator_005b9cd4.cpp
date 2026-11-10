// CUnprovenIterator_005b9cd4::operator->() const -- the value of the node it points at (+0x8: past a list node next/prev header)
// Leaf: ??CCUnprovenIterator_005b9cd4@@QBEPAUCUnprovenValue_005b9cd4@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CD4, 6 bytes, thiscall `ret`: mov eax, dword ptr [ecx] ; add eax, 8 ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenValue_005b9cd4 { long First; long Second; };
struct CUnprovenNode_005b9cd4 { CUnprovenNode_005b9cd4* Links[2]; CUnprovenValue_005b9cd4 Value; };
class CUnprovenIterator_005b9cd4 {
public:
    CUnprovenValue_005b9cd4* operator->() const;
    CUnprovenNode_005b9cd4* Node;
};

CUnprovenValue_005b9cd4* CUnprovenIterator_005b9cd4::operator->() const
{
    return &Node->Value;
}