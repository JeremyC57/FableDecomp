// CTCCreatureItemLevitationBase::~CTCCreatureItemLevitationBase() -- the compiler-generated destructor: destroy the LevitatingItems list (+0x0C, out of line), then ~CTCBase (tail jump)
// Leaf: ??1CTCCreatureItemLevitationBase@@UAE@XZ
// Name (agentNF20, 2026-11-11): Ego_r.pdb / FableWin.pdb ??1CTCCreatureItemLevitationBase@@UAE@XZ; retail callers are the slot-0 ??_G of
//   CTCCreatureItemLevitationBase 0x004E0968, CTCCreatureItemLevitationNymph 0x004E09A1 and CTCCreatureItemLevitationHero 0x004E09E4
//   (rtti-vtables.tsv). Was the descriptive FableDestVal_004de6ed stand-in (inner/outer on an address-named struct): the list destructor
//   is the landed std::list<CThing*>::~list 0x004AB490 and the tail is ??1CTCBase@@UAE@XZ 0x00686830 (engine/CTCBase.h retail branch).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE6ED, 19 bytes: `lea ecx,[esi+0Ch] ; call 0x004AB490 ; mov ecx,esi ; jmp 0x00686830`.
// Layout (Ego_r.pdb names at retail offsets; STLport list = one header pointer): LevitatingItems list<CThing*> +0x0C, NumItems +0x10,
//   LastAddFrame +0x14.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1); the size TU calls the list destructor out of line
//   (declared here as in the landed 0x004AB490 model). DestroyOne makes VC7.1 emit the implicit ??1.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list: one header-node pointer
public:
    void* _M_node;                                      // +0x00
    ~list();                                            // list<CThing*>: 0x004AB490
};
}

class CTCCreatureItemLevitationBase : public CTCBase {
public:
    std::list<CThing*> LevitatingItems;                 // +0x0c
    long               NumItems;                        // +0x10
    long               LastAddFrame;                    // +0x14
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCCreatureItemLevitationBase* object)
{
    object->CTCCreatureItemLevitationBase::~CTCCreatureItemLevitationBase();
}
