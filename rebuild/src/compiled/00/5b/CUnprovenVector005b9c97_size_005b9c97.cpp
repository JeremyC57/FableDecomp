// CUnprovenVector_005b9c97::size() const -- the element count of a vector of 12-byte elements (finish - start, a signed divide by 12)
// Leaf: ?size@CUnprovenVector_005b9c97@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C97, 12 bytes, thiscall `ret`: mov eax, dword ptr [ecx + 4] ; sub eax, dword ptr [ecx] ; push 0xc ; cdq ; pop ecx ; idiv ecx ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenElement12_005b9c97 { long Words[3]; };
class CUnprovenVector_005b9c97 {
public:
    unsigned int size() const;
    CUnprovenElement12_005b9c97* Start;
    CUnprovenElement12_005b9c97* Finish;
    CUnprovenElement12_005b9c97* EndOfStorage;
};

unsigned int CUnprovenVector_005b9c97::size() const
{
    return Finish - Start;
}