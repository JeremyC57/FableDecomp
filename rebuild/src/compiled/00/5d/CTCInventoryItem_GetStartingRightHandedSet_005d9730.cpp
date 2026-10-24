// CTCInventoryItem::GetStartingRightHandedSet(long defIndex) -- static: cache the item def for defIndex and return the right-handed set for its Orientation, or the default (orientation-0) set when the def cannot be cached
// Leaf: ?GetStartingRightHandedSet@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D9730, 88 bytes (+8 int3). Ego_r.pdb `?GetStartingRightHandedSet@CTCInventoryItem@@SI?AVCRightHandedSet@@J@Z`
// (static __fastcall: the hidden CRightHandedSet result in ecx, defIndex in edx; `ret`). Instance twin 0x005D92F0.
// Callees: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool), the private static
// CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) @ 0x005D8EF0 (Ego_r `CI`: hidden result in ecx =
// this function's result, orientation in edx). Globals: 0x013B8C18 the cached CInventoryItemDef (engine/CInventoryItemDef.h:
// Orientation +0x54, kept as bytes in the header), 0x0137CF74 the default right-handed set (also the default of
// GetRHSetFromOrientation's switch, whose cases copy their sets with the out-of-line copy 0x0049CF80).
// Shape: speed TU; CRightHandedSet has user constructors (Ego_r), so it is not an aggregate and its hidden result
// pointer rides in ecx (a POD return goes on the stack: 146v88). C3DVector's inline memberwise copy constructor makes
// CRightHandedSet's implicit copy non-trivial (0x0049CF80 is that copy outlined elsewhere); inlined here it copies the
// default set straight into [esi], six dwords rotating ecx/edx/eax. A trivially copyable C3DVector (87v88) or a user
// inline CRightHandedSet copy constructor (91v88) hoist `mov ecx,esi` above the branch and copy through ecx.
#include "engine/CInventoryItemDef.h"
enum EInventoryObjectOrientation { };
class C3DVector {                                       // 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class CRightHandedSet {                                 // 24 bytes
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // Ego_r ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z
};
extern const CRightHandedSet g_DefaultRHSet;            // 0x0137CF74
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static bool __fastcall CacheDef(long defIndex);                                         // 0x005D94B0
    static CRightHandedSet __fastcall GetStartingRightHandedSet(long defIndex);
    static CDefPointer<const CInventoryItemDef> s_CachedItemDef;                          // 0x013B8C18
private:
    static CRightHandedSet __fastcall GetRHSetFromOrientation(EInventoryObjectOrientation orientation);   // 0x005D8EF0
};
CRightHandedSet __fastcall CTCInventoryItem::GetStartingRightHandedSet(long defIndex)
{
    if (CacheDef(defIndex))
        return GetRHSetFromOrientation(reinterpret_cast<const EInventoryObjectOrientation&>(s_CachedItemDef->Orientation));
    return g_DefaultRHSet;
}