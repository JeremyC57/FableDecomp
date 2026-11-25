#pragma optimize("s",on)
// Leaf: ?_Destroy_range@@YIXPAUAnimPair@@0PAUAllocPair@@@Z
// _Destroy_range<allocator<pair<unsigned long, CAnimationEntry>>>
// retail 0x005c2a8e: loop over [first,last) stride 0x2c calling ~pair() (thiscall),
// pop edi/esi, ret 4. The element (std::pair<unsigned long, CAnimationEntry>) is 44
// bytes (0x2c) and its destructor is the out-of-line thiscall at 0x5c2739.

struct AnimPair {
    unsigned long first;          // key
    unsigned long second[10];     // CAnimationEntry payload -> total 44 (0x2c)
};

struct AllocPair {};

// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005C2739 ??1CCategory@NInventory@@QAE@XZ (was ?Dtor@AnimPair@@QAEXXZ)
namespace NInventory { class CCategory { public: ~CCategory(); }; }
void __fastcall _Destroy_range(AnimPair* first, AnimPair* last, AllocPair* al)
{
    for (; first != last; ++first)
        reinterpret_cast<NInventory::CCategory*>(first)->NInventory::CCategory::~CCategory();
}