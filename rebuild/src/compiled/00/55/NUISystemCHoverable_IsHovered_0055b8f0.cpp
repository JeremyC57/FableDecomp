// NUISystem::CHoverable::IsHovered(C2DBoxI&) const -- test the box's top-left against transformed component bounds.
// Native0055B8F0,163 bytes. Slots122=52EEC0 GetTargetRelativeRenderPosition,123=52ECC0 GetTargetRelativeRenderZoom,
// slot24=531090 GetBoundingBox. Typed member ABI projection exposes each hidden result pointer explicitly; eax returns that same pointer.
// Actual flattened donor CHoverable header retained; this method uses only its vptr, avoiding donor/native member offsets.
// _ftol2 at BFEA70 is emitted by the four truncating integer conversions. Half-open bounds use TLX/TLY only.
#include "engine/CHoverable.h"
namespace NUISystem {
struct C2DVector { float X,Y; C2DVector(){} C2DVector(const C2DVector& p):X(p.X),Y(p.Y){} };
struct C2DBoxF { float TLX,TLY,BRX,BRY; C2DBoxF(){} C2DBoxF(const C2DBoxF& p):TLX(p.TLX),TLY(p.TLY),BRX(p.BRX),BRY(p.BRY){} };
struct C2DBoxI { long TLX,TLY,BRX,BRY; };
struct C2DCoordI { long X,Y; C2DCoordI(long x,long y):X(x),Y(y) {} };
struct CHoverable;
#pragma pointers_to_members(full_generality,single_inheritance)
union HoverSlot {
    void* Address;
    C2DVector* (CHoverable::*Vector)(C2DVector*) const;
    C2DBoxF* (CHoverable::*Bounds)(C2DBoxF*) const;
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(HoverSlot)==4);
#endif
struct CHoverable : ::CHoverable {
    bool IsHovered(C2DBoxI&) const;
};
bool CHoverable::IsHovered(C2DBoxI& mouse) const {
    C2DVector position; (this->*static_cast<const HoverSlot*>(__vftable)[122].Vector)(&position);
    C2DVector zoom; (this->*static_cast<const HoverSlot*>(__vftable)[123].Vector)(&zoom);
    C2DBoxF bounds; (this->*static_cast<const HoverSlot*>(__vftable)[24].Bounds)(&bounds);
    C2DCoordI topLeft(position.X+bounds.TLX,position.Y+bounds.TLY);
    C2DCoordI bottomRight(bounds.BRX*zoom.X+position.X,bounds.BRY*zoom.Y+position.Y);
    long x=mouse.TLX, y=mouse.TLY;
    if(x>=topLeft.X && x<bottomRight.X && y>=topLeft.Y && y<bottomRight.Y) return true;
    return false;
}
}