// CAIStateGroup_CreatureAdvanceInCover::GetCoverThingList(CThingVector&) -- collect unoccupied cover closer to the enemy than the present cover (or creature).
// Retail 0x00978FD0, 287 bytes excluding padding. Shared AdvanceInCover/UseCoverBase/Base/Brain headers audited.
// UseCoverBase exposes PCoverThing at the retail unaligned +0x1f; inherited intelligent enemy handle at +0x14 is opaque in its generated header.
// Search 0x009793D0 audited: calls the four bound predicates, then appends matching things to the supplied vector. No predicate or callback is omitted.
#include <math.h>
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class C3DVector { public: float X,Y,Z; };
class C2DVector { public: float X,Y; C2DVector(const C3DVector& p):X(p.X),Y(p.Y){} };
struct CThingRetail { const C3DVector& GetPosition() const; }; // 0x004C73D0
struct CIntelligentPointerRetail { CThingRetail* GetPItem() const; }; // 0x00A01B50 (opaque handle methods)
struct CThingVector { CThingRetail** first; CThingRetail** last; CThingRetail** limit; unsigned char allocators[4]; };
class CThingFilter_CanBeUsedAsCover { public: CThingFilter_CanBeUsedAsCover() {} bool operator()(CThingRetail*) const; }; // 0x006A7AB0
class CThingFilter_IsOccupiableOccupied { public: CThingFilter_IsOccupiableOccupied() {} bool operator()(CThingRetail*) const; }; // 0x006A8A30
struct CNotOccupied { CThingFilter_IsOccupiableOccupied predicate; CNotOccupied() {} };
class CThingFilter_IsNot { public: CThingRetail* Thing; CThingFilter_IsNot(CThingRetail* t):Thing(t){} bool operator()(CThingRetail*) const; }; // 0x006A76A0
class CThingFilter_In2DRange { public: C3DVector Pos; float Range; CThingFilter_In2DRange(const C3DVector& p,float r):Pos(p),Range(r){} bool operator()(CThingRetail*) const; }; // 0x006A76B0
struct CFilterBind4 {
 const CThingFilter_CanBeUsedAsCover* F1; const CNotOccupied* F2; const CThingFilter_IsNot* F3; const CThingFilter_In2DRange* F4;
 CFilterBind4(const CThingFilter_CanBeUsedAsCover& a,const CNotOccupied& b,const CThingFilter_IsNot& c,const CThingFilter_In2DRange& d):F1(&a),F2(&b),F3(&c),F4(&d){}
};
class C2DBoxI { public: long minX,minY,maxX,maxY; };
struct C2DBox { float minX,minY,maxX,maxY;
 C2DBox(const C2DVector& p,float radius) { minX=p.X-radius; minY=p.Y-radius; maxX=p.X+radius; maxY=p.Y+radius; }
 C2DBoxI ToInteger() const; // 0x00A55A30, audited four float-to-long conversions
};
struct CThingSearchToolsRetail { void FindInRoughArea(const C2DBoxI&,const CFilterBind4&,CThingVector&); }; // 0x009793D0
struct AdvanceInCoverMethods : CAIStateGroup_CreatureAdvanceInCover {
 __forceinline CIntelligentPointerRetail& Cover() { return *(CIntelligentPointerRetail*)((CAIStateGroup_CreatureUseCoverBase*)this)->PCoverThing; }
 __forceinline CIntelligentPointerRetail& Enemy() { return *(CIntelligentPointerRetail*)((CAIStateGroup_CreatureUseCoverBase*)this)->_pad_0x14; }
 CThingSearchToolsRetail* PeekThingSearchTools(); // 0x008FD000
 void GetCoverThingList(CThingVector&);
};
void AdvanceInCoverMethods::GetCoverThingList(CThingVector& output)
{
 CThingRetail* current=Cover().GetPItem();
 if(!current) current=(CThingRetail*)((CAIStateGroupBase*)this)->PBrain->Creature;
 const C3DVector& enemy=Enemy().GetPItem()->GetPosition();
 const C3DVector& start=current->GetPosition();
 float dx=start.X-enemy.X,dy=start.Y-enemy.Y;
 float radius=(float)sqrt(dx*dx+dy*dy);
 C2DVector center(Enemy().GetPItem()->GetPosition());
 PeekThingSearchTools()->FindInRoughArea(C2DBox(center,radius).ToInteger(),CFilterBind4(CThingFilter_CanBeUsedAsCover(),CNotOccupied(),CThingFilter_IsNot(current),CThingFilter_In2DRange(Enemy().GetPItem()->GetPosition(),radius)),output);
}