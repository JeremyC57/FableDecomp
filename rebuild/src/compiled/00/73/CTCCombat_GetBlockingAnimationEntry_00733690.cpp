// CTCCombat::GetBlockingAnimationEntry() -- a random "BLOCK_IDLE" combat animation of the parent creature, or its plain BLOCK_IDLE animation when it has no combat variants
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733690, 184 bytes (plain `ret`, 0x10-byte frame: the name string and the vector).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CThingCreatureBase::GetPossibleCombatAnimations
// (const CCharString&, vector<const CAnimationEntry*>&) const @ 0x006668A0 (manifest "Animation_FilterAndCompile"; Ego_r.pdb name),
// CTCBase::GetPRandomSeed() @ 0x00686DA0, GFROR13 @ 0x00497890 (`__fastcall`), CThingCreatureBase::GetAnimFromName(const
// CCharString&, long) @ 0x00662A00, free @ 0x00BFEA14 (the vector storage). Literal "BLOCK_IDLE" @ 0x012670DC.
// Layout: engine/CTCCombat.h ParentCreature +0x24 (before any container, so the PDB offset holds in retail).
// Shape: a local STLport vector (zeroed after the call's pushes) released with `if (first) free(first)`; the random pick is
// the landed WorldRandomRange shape (seed advanced, GFROR13, `% n` guarded by n != 0) indexing *(begin() + r); the
// no-variants arm asks for animation 0 by name.
#include "engine/CTCCombat.h"
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAnimationEntry;
template <class T> struct CRetailVector {               // STLport vector, 12 bytes
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
    ~CRetailVector() { if (first) free(first); }
    iterator begin() { return iterator(first); }
    bool empty() const { return first == last; }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
class CThingCreatureBase { public:
    bool GetPossibleCombatAnimations(const CCharString& name, CRetailVector<const CAnimationEntry*>& anims) const;   // 0x006668A0
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};
struct CTCCombat_Methods : CTCCombat {
    unsigned long* GetPRandomSeed();                    // 0x00686DA0 (CTCBase)
    const CAnimationEntry* GetBlockingAnimationEntry();
};

const CAnimationEntry* CTCCombat_Methods::GetBlockingAnimationEntry()
{
    CCharString name("BLOCK_IDLE", -1);
    CRetailVector<const CAnimationEntry*> anims;
    ParentCreature->GetPossibleCombatAnimations(name, anims);
    const CAnimationEntry* entry;
    if (!anims.empty()) {
        unsigned long count = anims.size();
        unsigned long* seed = GetPRandomSeed();
        entry = anims[WorldRandomRange(*seed, count)];
    } else {
        entry = ParentCreature->GetAnimFromName(name, 0);
    }
    return entry;
}