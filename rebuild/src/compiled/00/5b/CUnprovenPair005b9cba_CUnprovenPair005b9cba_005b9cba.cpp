// CUnprovenPair_005b9cba::CUnprovenPair_005b9cba(const long& first, const long& second) -- two-member constructor from references (a std::pair<long-sized, long> shape)
// Leaf: ??0CUnprovenPair_005b9cba@@QAE@ABJ0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CBA, 22 bytes, thiscall `ret 8`: mov eax, ecx ; mov ecx, dword ptr [esp + 4] ; mov ecx, dword ptr [ecx] ; mov dword ptr [eax], ecx ; mov ecx, dword ptr [esp + 8] ; mov ecx, dword ptr [ecx] ; mov dword ptr [eax + 4], ecx ; ret 8
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenPair_005b9cba {
public:
    CUnprovenPair_005b9cba(const long& first, const long& second);
    long First;
    long Second;
};

CUnprovenPair_005b9cba::CUnprovenPair_005b9cba(const long& first, const long& second) : First(first), Second(second)
{
}