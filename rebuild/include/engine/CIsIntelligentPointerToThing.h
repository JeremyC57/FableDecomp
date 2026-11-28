#ifndef FABLE_ENGINE_CISINTELLIGENTPOINTERTOTHING_H
#define FABLE_ENGINE_CISINTELLIGENTPOINTERTOTHING_H
#ifdef FABLE_RETAIL_CTCENEMY_ISENEMYOF_PASS30
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"

// Ego_d type 0x3f3f4, with const CThing& member type 0x2c14.
// FableWin 02607270 constructs the reference, 02607FA0 calls the raw-pointer comparison.
class CIsIntelligentPointerToThing {
public:
    explicit CIsIntelligentPointerToThing(const CThing& thing) : Thing(thing) {}
    bool operator()(const CIntelligentPointer<const CThing>& pointer) const {
        return pointer == &Thing;
    }
private:
    const CThing& Thing;
};
#endif // FABLE_RETAIL_CTCENEMY_ISENEMYOF_PASS30
#endif
