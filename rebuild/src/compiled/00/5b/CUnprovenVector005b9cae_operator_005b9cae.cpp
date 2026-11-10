// CUnprovenVector_005b9cae::operator[](unsigned int index) -- the element at index of a vector of 4-byte elements (its start pointer at +0)
// Leaf: ??ACUnprovenVector_005b9cae@@QAEAAUCUnprovenElement4_005b9cae@@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CAE, 12 bytes, thiscall `ret 4`: mov eax, dword ptr [ecx] ; mov ecx, dword ptr [esp + 4] ; lea eax, [eax + ecx*4] ; ret 4
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenElement4_005b9cae { long Words[1]; };
class CUnprovenVector_005b9cae {
public:
    CUnprovenElement4_005b9cae& operator[](unsigned int index);
    CUnprovenElement4_005b9cae* Start;
    CUnprovenElement4_005b9cae* Finish;
    CUnprovenElement4_005b9cae* EndOfStorage;
};

CUnprovenElement4_005b9cae& CUnprovenVector_005b9cae::operator[](unsigned int index)
{
    return Start[index];
}