#ifndef FABLE_ENGINE_CCOMBATACTIONBASE_H
#define FABLE_ENGINE_CCOMBATACTIONBASE_H
#include "engine/CCreatureActionBase.h"
// Ego_r0x37dee; native base is0x114 after STLport list/vector reductions.
class CCombatActionBase : public CCreatureActionBase {
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
protected: virtual CCreatureActionBase* Clone() const;
protected: unsigned char _pad_0xa8[0x6c];
};
FABLE_STATIC_ASSERT(sizeof(CCombatActionBase)==0x114);
#endif
