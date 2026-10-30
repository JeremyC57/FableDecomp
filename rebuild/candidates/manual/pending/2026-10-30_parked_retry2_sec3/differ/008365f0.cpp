// std::for_each(const pair<unsigned long, CAnimationEntry>* first, const pair<unsigned long, CAnimationEntry>* last, CDeadCreatureAnimPreloader f) -- run the dead-creature preloader over an animation set: every entry named STANDARD_DEAD gets its bank animation loading started; returns the functor
// Leaf: ??$for_each@
// Grade: DIFFER(119v119) -- wave 2 sec3: PDB const signatures applied (operator() QBE, ctor (const CMeshDataBank&), PeekAnimation QBE PBV..K, GetStringAsCharArray QBE, CDefString::GetAsCharArray); residue unchanged: the strcmp-zero register (retail eax, ours edx) and the early-exit functor copy (retail edx, ours ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008365F0, 119 bytes, __fastcall (/Gr TU: hidden result in ecx, first in edx, last
//   and the by-value functor on the stack), `ret 8`.
// Identity: Ego_r.pdb `std::_For_each<const pair<unsigned long, CAnimationEntry>*, CDeadCreatureAnimPreloader>` (the
//   Dinkumware spelling; retail is STLport `for_each`); the only caller is CThingDeadCreature::PreloadDeadAnimation
//   0x00835BD0 (landed, which models this body as a stand-in). The manifest label
//   "CTCInventoryAbilities_PreloadDeadAnimation" is propagated.
// Callees: CDefStringTable::GetStringAsCharArray(long) 0x009D49F0 on the global table 0x013CA828; CMeshDataBank::
//   PeekAnimation(long) 0x00A242C0 (landed; `ret 4`); C3DAnimationInfo::BeginLoading(bool) const 0x00A26730 (landed);
//   literal "STANDARD_DEAD" 0x01272AF0.
// Layout: Ego_r.pdb CAnimationEntry (BankIndex +0, AnimName +4 CDefString, GroupName +8, Components +0xc) behind the
//   pair's key: element stride 0x14; CDeadCreatureAnimPreloader = { CMeshDataBank* MeshBank } (4 bytes).
// Shape: SGI/STLport `for (; first != last; ++first) f(*first); return f;`; the functor's operator() is inline:
//   `strcmp(name, "STANDARD_DEAD") == 0` through the strcmp intrinsic (`repe cmpsb`, its zero eax reused as the
//   BeginLoading `false` pushed before the PeekAnimation call -- arguments right to left); the hidden result pointer is
//   spilled to the `push ecx` slot and the functor copied through it at the end.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

class CDefStringTable { public:
    const char* GetStringAsCharArray(long tablePos) const;   // 0x009D49F0 (PDB QBE)
};
extern CDefStringTable g_DefStringTable;                 // 0x013CA828

class CDefString { public:
    long TablePos;
    const char* GetAsCharArray() const { return g_DefStringTable.GetStringAsCharArray(TablePos); }
};

class C3DAnimationInfo { public:
    void BeginLoading(bool wait) const;                  // 0x00A26730
};

class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long bankIndex) const;   // 0x00A242C0 (PDB QBEPBV..K)
};

class CAnimationEntry { public:                                 // Ego_r.pdb, 16 bytes
    long       BankIndex;                                // +0x00
    CDefString AnimName;                                 // +0x04
    CDefString GroupName;                                // +0x08
    void*      Components;                               // +0x0c CIVCountedPointer<CAnimComponentMap>
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for (; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}

class CDeadCreatureAnimPreloader {                       // Ego_r.pdb, 4 bytes
public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& bank) : MeshBank(&bank) { }
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry) const
    {
        if (strcmp(entry.second.AnimName.GetAsCharArray(), "STANDARD_DEAD") == 0)
            MeshBank->PeekAnimation(entry.second.BankIndex)->BeginLoading(false);
    }
private:
    const CMeshDataBank* MeshBank;
};

template CDeadCreatureAnimPreloader __fastcall std::for_each(const std::pair<unsigned long, CAnimationEntry>* __first,
                                                             const std::pair<unsigned long, CAnimationEntry>* __last,
                                                             CDeadCreatureAnimPreloader __f);
