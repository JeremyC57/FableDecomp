// CUnprovenPair_005b9c61::GetSecond() const -- returns the second word of this (a vector's end())
// Leaf: ?GetSecond@CUnprovenPair_005b9c61@@QBEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C61, 4 bytes, thiscall `ret`: mov eax, dword ptr [ecx + 4] ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenPair_005b9c61 {
public:
    void* GetSecond() const;
    void* First;
    void* Second;
};

void* CUnprovenPair_005b9c61::GetSecond() const
{
    return Second;
}