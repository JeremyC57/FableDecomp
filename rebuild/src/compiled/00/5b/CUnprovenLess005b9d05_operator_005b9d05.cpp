// CUnprovenLess_005b9d05::operator()(const long& left, const long& right) const -- signed less-than of two word-sized keys (a std::less over long or an enum)
// Leaf: ??RCUnprovenLess_005b9d05@@QBE_NABJ0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9D05, 22 bytes, thiscall `ret 8`: mov eax, dword ptr [esp + 4] ; mov eax, dword ptr [eax] ; mov edx, dword ptr [esp + 8] ; xor ecx, ecx ; cmp eax, dword ptr [edx] ; setl cl ; mov al, cl ; ret 8
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenLess_005b9d05 {
public:
    bool operator()(const long& left, const long& right) const;
};

bool CUnprovenLess_005b9d05::operator()(const long& left, const long& right) const
{
    return left < right;
}