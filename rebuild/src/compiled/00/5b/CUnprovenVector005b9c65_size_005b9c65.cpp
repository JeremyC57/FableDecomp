// CUnprovenVector_005b9c65::size() const -- the element count of a vector of 24-byte elements (finish - start, a signed divide by 24)
// Leaf: ?size@CUnprovenVector_005b9c65@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9C65, 12 bytes, thiscall `ret`: mov eax, dword ptr [ecx + 4] ; sub eax, dword ptr [ecx] ; push 0x18 ; cdq ; pop ecx ; idiv ecx ; ret
// Identity: one of the 47 out-of-line helpers retail packs after DrawPreparePrimitivesForRendering (0x005B9BF0..0x005B9D8C) with no call, jmp or absolute reference anywhere in Fable.exe (whole-image rel32 + dword scan), so its owner type is unproven; the type is the descriptive CUnproven placeholder (EXTRA_RULES.md), its shape is the bytes.
#pragma optimize("s", on)
struct CUnprovenElement24_005b9c65 { long Words[6]; };
class CUnprovenVector_005b9c65 {
public:
    unsigned int size() const;
    CUnprovenElement24_005b9c65* Start;
    CUnprovenElement24_005b9c65* Finish;
    CUnprovenElement24_005b9c65* EndOfStorage;
};

unsigned int CUnprovenVector_005b9c65::size() const
{
    return Finish - Start;
}