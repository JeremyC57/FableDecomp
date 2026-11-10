// CUnprovenIterator_005b9d47::operator->() const -- the value of the node it points at (+0x10: past a tree node colour/parent/left/right header)
// Leaf: ??CCUnprovenIterator_005b9d47@@QBEPAUCUnprovenValue_005b9d47@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9D47, 6 bytes, thiscall `ret`: mov eax, dword ptr [ecx] ; add eax, 0x10 ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenValue_005b9d47 { long First; long Second; };
struct CUnprovenNode_005b9d47 { CUnprovenNode_005b9d47* Links[4]; CUnprovenValue_005b9d47 Value; };
class CUnprovenIterator_005b9d47 {
public:
    CUnprovenValue_005b9d47* operator->() const;
    CUnprovenNode_005b9d47* Node;
};

CUnprovenValue_005b9d47* CUnprovenIterator_005b9d47::operator->() const
{
    return &Node->Value;
}