#ifndef FABLE_ENGINE_CTCGIFT_H
#define FABLE_ENGINE_CTCGIFT_H
#ifdef FABLE_RETAIL_GIVE_ITEM_ACTION_PASS26
#include "engine/CTCBase.h"
// Original RTTI and both PDBs identify CTCBase and the three const predicates.
class CTCGift : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_GIFT; }
    bool IsFriendly() const;
    bool IsRomantic() const;
    bool IsOffensive() const;
};
#endif
#endif
