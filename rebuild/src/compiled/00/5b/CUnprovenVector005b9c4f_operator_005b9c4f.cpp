// CUnprovenVector_005b9c4f::operator[](unsigned int index) -- the element at index of a vector of 12-byte elements (its start pointer at +0)
// Leaf: ??ACUnprovenVector_005b9c4f@@QAEAAUCUnprovenElement12_005b9c4f@@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C4F, 15 bytes, thiscall `ret 4`: mov eax, dword ptr [esp + 4] ; mov ecx, dword ptr [ecx] ; lea eax, [eax + eax*2] ; lea eax, [ecx + eax*4] ; ret 4
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenElement12_005b9c4f { long Words[3]; };
class CUnprovenVector_005b9c4f {
public:
    CUnprovenElement12_005b9c4f& operator[](unsigned int index);
    CUnprovenElement12_005b9c4f* Start;
    CUnprovenElement12_005b9c4f* Finish;
    CUnprovenElement12_005b9c4f* EndOfStorage;
};

CUnprovenElement12_005b9c4f& CUnprovenVector_005b9c4f::operator[](unsigned int index)
{
    return Start[index];
}