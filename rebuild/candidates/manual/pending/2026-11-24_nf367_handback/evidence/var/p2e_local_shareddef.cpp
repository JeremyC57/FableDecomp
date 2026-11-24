// MEASUREMENT ONLY (NF367 P2e): 0x00749A70's local CTCNoise model (implicit destructor, the five retail overrides) on the shared
// engine/CTCNoiseDef.h with 0x00749810's def opt-ins plus FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR; ctor text = 0x00749810's.
// Leaf: ??1CTCNoise@@UAE@XZ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CTCBase.h"
#include "engine/CTCNoiseDef.h"

class CThing;
class CBaseObject;
class CBaseObjectPointer;

#include "engine/CIntelligentPointer.h"

enum ETCInterfaceType;

class CTCNoise : public CTCBase {                           // vtable 0x01268A74
public:
    CTCNoise(CThing& thing);                                // ??0CTCNoise@@QAE@AAVCThing@@@Z
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual void OnCreate();
    virtual void FrameUpdate();
    virtual bool HasFrameUpdate() const;
    CTCNoiseDef                       Definition;           // +0x0C
    long                              LifeTimer;            // +0x40
    float                             DistanceCanBeHeardFrom;   // +0x44
    CIntelligentPointer<const CThing> Creator;              // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CTCNoise) == 0x50);

CTCNoise::CTCNoise(CThing& owner)
    : CTCBase(owner), LifeTimer(0), DistanceCanBeHeardFrom(0.0f)
{
}
