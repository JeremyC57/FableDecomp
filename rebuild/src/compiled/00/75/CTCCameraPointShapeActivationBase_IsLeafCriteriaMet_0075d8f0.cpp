// CTCCameraPointShapeActivationBase::IsLeafCriteriaMet() -- true when camera points are globally enabled and the parent thing's interface-0x44 component reports its flag at +0x1c
// Leaf: IsLeafCriteriaMet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D8F0, length 70 bytes. Vtable 0x0123AEDC slot 29 (shared by six
// camera-point vtables).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (landed GetInterface family).
// Globals: the enable byte @ 0x013BAD7C (a bool; `mov al,[g] ; test al,al`).
// Identity: slot 29 is CTCCameraPointBuilding::IsLeafCriteriaMet in the manifest; "vfunc_29" is the
// placeholder. Layout: engine/CTCBase.h (ParentThing +4); CThing's interface bitset at +0x20 and the
// sorted interface map at +0x44 as in the landed GetInterface sources.
// Shape: the thing's inline GetInterface(type, &out) (landed 0x007504E0 shape): the bitset word is
// tested raw (type 0x44 -> word 2 at +0x28, bit 4: `test byte ptr [eax+28h],10h`), the key lives in
// the `push ecx` frame slot, the map pointer is a named local (esi), a miss yields the end entry, and
// the component comes back in eax. The template must be `__forceinline` (else it is emitted out of
// line and called). The `if (g) { if (Get) return flag; } return false;` nesting gives the single
// `xor al,al` tail.
#include "engine/CTCBase.h"

extern bool g_cameraPointsEnabled;                 // 0x013BAD7C

struct CTCComponent;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCameraShape {                             // the interface-0x44 component (class not established)
    unsigned char _pad[0x1c];
    bool          m_isActive;                       // +0x1c
};

struct CTCCameraPointShapeActivationBase : CTCBase {
    bool IsLeafCriteriaMet();
};

bool CTCCameraPointShapeActivationBase::IsLeafCriteriaMet()
{
    if (g_cameraPointsEnabled) {
        CTCCameraShape* shape;
        if (ParentThing->GetInterface(0x44, &shape))
            return shape->m_isActive;
    }
    return false;
}