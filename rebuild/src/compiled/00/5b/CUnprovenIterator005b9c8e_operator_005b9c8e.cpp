// CUnprovenIterator_005b9c8e::operator->() const -- the value of the node it points at (+0x10: past a tree node colour/parent/left/right header)
// Leaf: ??CCUnprovenIterator_005b9c8e@@QBEPAUCUnprovenValue_005b9c8e@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C8E, 6 bytes, thiscall `ret`: mov eax, dword ptr [ecx] ; add eax, 0x10 ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenValue_005b9c8e { long First; long Second; };
struct CUnprovenNode_005b9c8e { CUnprovenNode_005b9c8e* Links[4]; CUnprovenValue_005b9c8e Value; };
class CUnprovenIterator_005b9c8e {
public:
    CUnprovenValue_005b9c8e* operator->() const;
    CUnprovenNode_005b9c8e* Node;
};

CUnprovenValue_005b9c8e* CUnprovenIterator_005b9c8e::operator->() const
{
    return &Node->Value;
}