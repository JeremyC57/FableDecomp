// CUnprovenPair_005b9ce3::CUnprovenPair_005b9ce3(const long& first, const bool& second) -- two-member constructor from references (a std::pair<long-sized, bool> shape)
// Leaf: ??0CUnprovenPair_005b9ce3@@QAE@ABJAB_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CE3, 22 bytes, thiscall `ret 8`: mov eax, ecx ; mov ecx, dword ptr [esp + 4] ; mov ecx, dword ptr [ecx] ; mov dword ptr [eax], ecx ; mov ecx, dword ptr [esp + 8] ; mov cl, byte ptr [ecx] ; mov byte ptr [eax + 4], cl ; ret 8
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenPair_005b9ce3 {
public:
    CUnprovenPair_005b9ce3(const long& first, const bool& second);
    long First;
    bool Second;
};

CUnprovenPair_005b9ce3::CUnprovenPair_005b9ce3(const long& first, const bool& second) : First(first), Second(second)
{
}