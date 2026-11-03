#ifndef FABLE_ENGINE_THINGLLIST_H
#define FABLE_ENGINE_THINGLLIST_H
class CThing;
// Original struct key U and both pointers from both PDBs.
struct ThingLList {
public:
    CThing * Head;
    CThing * Tail;
};
#endif
