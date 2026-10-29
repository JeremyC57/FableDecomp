// CAIStateGroup_OnHeroPush::IsRunnable() -- copy and table-normalize an angry hero-push direction; retail 0x0092b800, 254 bytes. Callees: CVectorMap::LowerBound 0x004365b0, CTCOnHeroPush::IsAngry 0x007bba50. Globals: inverse-square-root table 0x013ce558, three 0x0122ded4, half 0x0122f59c.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

class C3DVector { public:
    float X, Y, Z;
    FABLE_FORCEINLINE C3DVector& Normalise();
};

class CTCOnHeroPush { public:
    void* __vftable;
    fable_u32 Unknown04;
    fable_u32 Unknown08;
    float DistPushed;
    C3DVector PushDir;
    fable_i32 FrameSincePushed;
    bool IsAngry() const;                                      // 0x007bba50
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry* First;
    Entry* Last;
    const Entry* LowerBound(const ETCInterfaceType& type) const;                 // 0x004365b0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

// CThingAICreature's generated header leaves this component-map region opaque.
// The adapter names the two native regions without inventing a second creature layout.
struct CThingAICreatureInterfaceAccess : CThingAICreature {
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, fable_i32 type) {
        fable_u32* presentBits = (fable_u32*)((char*)this + 0x20);
        if (!(presentBits[type >> 5] & (1u << (type & 31))))
            return false;
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = (CInterfaceMap*)((char*)this + 0x44);
        const CInterfaceMap::Entry* it = map->LowerBound(key);
        if (it == map->Last || type < it->first)
            it = map->Last;
        *out = (T*)((void*)it->second);
        return true;
    }
};

extern fable_u32 g_InvSqrtMantissaTable[128];              // 0x013ce558
extern float g_three;                                      // 0x0122ded4
extern float g_half;                                       // 0x0122f59c
extern float g_angryPushDistance;                          // 0x01383744

FABLE_FORCEINLINE C3DVector& C3DVector::Normalise()
{
    float x = X, y = Y, z = Z;
    float lengthSquared = z * z;
    lengthSquared = lengthSquared + x * x;
    lengthSquared = lengthSquared + y * y;
    fable_u32 bits = *(fable_u32*)&lengthSquared;
    fable_u32 estimateBits = (((0x17cu - ((bits >> 23) & 0xffu)) << 22) & 0xff800000u) |
        (g_InvSqrtMantissaTable[(bits >> 17) & 0x7fu] << 15);
    float r = *(float*)&estimateBits;
    r = (g_three - r * r * lengthSquared) * r;
    r = r * g_half;
    X = X * r;
    Y = Y * r;
    Z = Z * r;
    return *this;
}

struct CAIStateGroup_OnHeroPush : CAIStateGroupBase {
    C3DVector PushDir;                                     // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_OnHeroPush::IsRunnable()
{
    CThingAICreatureInterfaceAccess* creature = (CThingAICreatureInterfaceAccess*)PBrain->Creature;
    CTCOnHeroPush* push = 0;
    if (creature->GetInterface(&push, 0xc4)) {
        if (push->IsAngry()) {
            PushDir = push->PushDir;
            PushDir.Normalise();
            return true;
        }
    }
    return false;
}

#if defined(_MSC_VER)
typedef char fable_abi_assert_group_brain[(offsetof(CAIStateGroupBase, PBrain) == 0x04) ? 1 : -1];
typedef char fable_abi_assert_brain_creature[(offsetof(CAIBrain, Creature) == 0x18) ? 1 : -1];
typedef char fable_abi_assert_creature_size[(sizeof(CThingAICreature) == 0x1d8) ? 1 : -1];
typedef char fable_abi_assert_pushdir[(offsetof(CAIStateGroup_OnHeroPush, PushDir) == 0x14) ? 1 : -1];
typedef char fable_abi_assert_vector[(sizeof(C3DVector) == 0x0c) ? 1 : -1];
typedef char fable_abi_assert_component_pushdir[(offsetof(CTCOnHeroPush, PushDir) == 0x10) ? 1 : -1];
#endif