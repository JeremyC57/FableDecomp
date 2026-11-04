// CEngineInternalPrimitiveSpriteTrail::~CEngineInternalPrimitiveSpriteTrail() -- Release owned sequence storage and destroy the base.
// Leaf: ??1CEngineInternalPrimitiveSpriteTrail@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own deleting-dtor00bcc320 and full RTTI12a6a4c; actual inherited hierarchy and original Trail+104; base vtable12354d0.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CEnginePersistentPrimitiveBase CEnginePersistentPrimitiveBase_HeaderLayout
#include "engine/CEnginePersistentPrimitiveBase.h"
#undef CEnginePersistentPrimitiveBase
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBase_HeaderLayout
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "engine/CArray.h"
// Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.
class CIVCountedPointeeBase {public:virtual __forceinline ~CIVCountedPointeeBase(){}virtual void Delete()const;unsigned long IVRefCount;};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase {public:long Type;};
class CEngineSceneGridCell;
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase {public:
unsigned long EnginePrimitiveFlags;float AlphaMipBias;struct CEngineFadeDistance{float FadeBeginDistance,MaxDrawDistance;} FadeDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;
struct CEngineSceneGridPos{signed char Level;unsigned char X,Y;} GridPos;unsigned char Alpha,AlphaRef,PreloadStat;unsigned short Pad;int InterpolationSet;unsigned long RenderLayerMask;unsigned short StateFlags,LastFrameRendered;CEngineInternalPrimitiveBase* NextPrimitive;CEngineInternalPrimitiveBase** RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;CEngineInternalPrimitiveBase** RefLayerMask;
};
class C3DVector {public:float X,Y,Z;};
class CEngineInternalPrimitiveSpriteTrail:public CEngineInternalPrimitiveBase {public:class CSpriteTrailEntry {public:C3DVector Position;float Width;};unsigned long MaxLength,GraphicIndex,GraphicFrame;float SkipPoints,Width;unsigned int HeadColour,MidColour,TailColour;CArray<CSpriteTrailEntry> Trail;long NextPos;unsigned short Flags;int BlendOp,BlendMode;float WorldMat[12];};


void DestroyCandidate(CEngineInternalPrimitiveSpriteTrail* p){p->CEngineInternalPrimitiveSpriteTrail::~CEngineInternalPrimitiveSpriteTrail();}