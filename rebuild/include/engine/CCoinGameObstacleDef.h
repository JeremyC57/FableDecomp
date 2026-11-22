#ifndef FABLE_ENGINE_CCOINGAMEOBSTACLEDEF_H
#define FABLE_ENGINE_CCOINGAMEOBSTACLEDEF_H
#ifdef FABLE_RETAIL_CCOINGAMEOBSTACLEDEF_PASS26
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"

// Ego_d TPI 0x339BB: public CSubDefClassBase and Radius at 0x28, full size 0x2C.
// Retail allocation, Copy, GetSizeofClass and the FindObstacles radius read corroborate the layout.
class CCoinGameObstacleDef : public CSubDefClassBase {
public:
    CCoinGameObstacleDef();
    virtual ~CCoinGameObstacleDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    virtual void Transfer(CPersistContext&);
    static CDefClassBase* FABLE_FASTCALL Alloc();
    static CCharString FABLE_FASTCALL StatGetTypeName() { return "CCoinGameObstacleDef"; }
    float Radius;
};
FABLE_STATIC_ASSERT_AT(21, sizeof(CCoinGameObstacleDef) == 0x2C);
FABLE_STATIC_ASSERT_AT(22, offsetof(CCoinGameObstacleDef, Radius) == 0x28);
#endif
#endif
