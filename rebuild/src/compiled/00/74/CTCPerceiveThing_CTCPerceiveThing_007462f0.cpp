// CTCPerceiveThing::~CTCPerceiveThing() -- Release the definition, destroy the owned member, then destroy the component base.
// Leaf: ??1CTCPerceiveThing@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Layout: genuine member chain from PDB; retail PDef+0x34 and PerceivedThing+0xc. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
#include "fable_compiler.h"
#define CTCPerceiveThing CTCPerceiveThing_HeaderLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CThing;class CBaseObjectPointer;
class CTCBase {public:virtual ~CTCBase();CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.
class CIVCountedPointeeBase {public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable long IVRefCount;};
template<class T> class CDefPointer {public:T* Object;FABLE_FORCEINLINE ~CDefPointer(){if(Object){const CIVCountedPointeeBase* base=(const CIVCountedPointeeBase*)Object;if(--base->IVRefCount==0)base->Delete();Object=0;}}};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CEnginePrimitiveThemeChannelDesc {public:long ID;};
enum EOracleMinigameState {};enum EOracleMinigameInput {};
namespace std {template<class T>class allocator{};template<class T,class A=allocator<T> >class list {public:void* _M_node;~list();};}
class CPerceivedThingDef;
class CTCPerceiveThing : public CTCBase {public:
    CIntelligentPointer<const CThing> PerceivedThing;
    float DistanceFromPerceivedThingSquared,AwarenessTimer;long FramesSinceDirectlyPerceivedThing;C3DVector LastPerceivedPos;
    bool Perceiving,LineOfSight,ExtendedLineOfSight,IgnoreFOVLineOfSight,DirectAwareness,Smelt,Heard,SearchingForThingToPerceive;
    CDefPointer<const CPerceivedThingDef> PDef;long UpdateOffset;float SoundRadius,SightRadius,ExtendedSightRadius,SmellRadius,GiveUpChaseRadius;bool PerceptionCombat;long FrameHitByPerceivedThing;
};
void Destroy_CTCPerceiveThing(CTCPerceiveThing* p){p->CTCPerceiveThing::~CTCPerceiveThing();}