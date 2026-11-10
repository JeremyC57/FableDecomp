// CUnprovenIterator_005b9bfb::operator==(const CUnprovenIterator_005b9bfb& other) const -- whether two one-word iterators hold the same node
// Leaf: ??8CUnprovenIterator_005b9bfb@@QBE_NABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9BFB, 16 bytes, thiscall `ret 4`: mov eax, dword ptr [esp + 4] ; mov eax, dword ptr [eax] ; sub eax, dword ptr [ecx] ; neg eax ; sbb eax, eax ; inc eax ; ret 4
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenIterator_005b9bfb {
public:
    bool operator==(const CUnprovenIterator_005b9bfb& other) const;
    void* Node;
};

bool CUnprovenIterator_005b9bfb::operator==(const CUnprovenIterator_005b9bfb& other) const
{
    return other.Node == Node;
}