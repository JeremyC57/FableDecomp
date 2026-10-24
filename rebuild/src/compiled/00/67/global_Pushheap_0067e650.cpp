// _global::_Push_heap(CDegradableInfo*, int, int, CDegradableInfo, CIsHealth1GTHealth2) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067E650, 105 bytes, `ret 0x24`.  No callees.
// Element: CDegradableDef's degradation stages (PDB CDegradableInfo, 28 bytes), sorted from CDegradableDef::Transfer (0x0067E290) with CDegradableDef::CIsHealth1GTHealth2.
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 28-byte record by value and the empty 4-byte comparator on the stack (4 + 28 + 4 = 0x24).
// The record is trivially copyable, so both element moves are `rep movsd` x7.
// Predicate: `fld [parent] ; fcomp [value] ; test ah,41h ; jne` = `parent.HealthPercentage > value.HealthPercentage`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N0067e650 {
struct CEngineGraphic {   // PDB: 14 bytes of data, 16 with the dword alignment
    unsigned long BankIndex;      // +0x0
    float AnimStep;               // +0x4
    float RenderSizeX;            // +0x8
    bool AdditiveAlpha;           // +0xc
    unsigned char Type;           // +0xd
};
struct CDegradableInfo {
    float HealthPercentage;       // +0x0
    CEngineGraphic EngineGraphic; // +0x4 (16 bytes)
    long SmashParticleEmitter;    // +0x14
    bool BlocksNavigation;        // +0x18
};

struct CIsHealth1GTHealth2 {
    char m_tag;
    bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
};

void __fastcall _Push_heap(CDegradableInfo* first, int hole, int top, CDegradableInfo value, CIsHealth1GTHealth2 comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}