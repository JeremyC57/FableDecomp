// 008ef200 Coin::Coin
// Leaf: ??0Coin@@QAE@MMPAVCThing@@_N@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_COIN_OWNER_TYPE_PASS26
#define FABLE_RETAIL_MASS_INLINE_DTOR_PASS26
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/Coin.h"

// Descriptive: same-TU Coin::UpdateSound reads this unsigned sound sentinel.
extern const unsigned long g_InvalidSoundHandle;

Mass::Mass()
{
    Pos = C2DVector(0.0f, 0.0f);
    Vel = C2DVector(0.0f, 0.0f);
    APos = 0.0f;
    AVel = 0.0f;
}

Coin::Coin(float radius, float idensity, CThing* thing, bool owned)
{
    Owned = owned;
    Thing = thing;
    Radius = radius;
    IMass = (float)(idensity / (6.283185307 * Radius * Radius));
    AIMass = (float)(2.5 * IMass / (Radius * Radius));
    Sound = g_InvalidSoundHandle;
}