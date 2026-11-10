// CUnprovenEmpty_005b9cd3::~CUnprovenEmpty_005b9cd3() -- empty destructor (an empty class such as an allocator or functor)
// Leaf: ??1CUnprovenEmpty_005b9cd3@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9CD3, 1 bytes, thiscall `ret`: ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
class CUnprovenEmpty_005b9cd3 {
public:
    ~CUnprovenEmpty_005b9cd3();
};

CUnprovenEmpty_005b9cd3::~CUnprovenEmpty_005b9cd3()
{
}