// CUnprovenPair_005b9ca7::GetFirst() const -- returns the first word of this (a begin(), an iterator's node or a pointer's target)
// Leaf: ?GetFirst@CUnprovenPair_005b9ca7@@QBEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CA7, 3 bytes, thiscall `ret`: mov eax, dword ptr [ecx] ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenPair_005b9ca7 {
public:
    void* GetFirst() const;
    void* First;
    void* Second;
};

void* CUnprovenPair_005b9ca7::GetFirst() const
{
    return First;
}