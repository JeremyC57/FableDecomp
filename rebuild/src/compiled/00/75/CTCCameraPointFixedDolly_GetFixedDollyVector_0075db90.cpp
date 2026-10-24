// CTCCameraPointFixedDolly::GetFixedDollyVector() const -- the dolly direction: the principal vector of the parent thing's shape-manager component
// Leaf: ?GetFixedDollyVector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DB90, 104 bytes (+int3 pad to 112), `ret 4` (C3DVector returned
//   through the hidden pointer).
// Identity: Ego_r.pdb ?GetFixedDollyVector@CTCCameraPointFixedDolly@@QBE?AVC3DVector@@XZ (the manifest's
//   "CTCShape_InitializeFromInterface" is a forge autoname). Only caller 0x0087B74E (a camera mode that takes a
//   camera point of type 1 through GetPointType and copies this vector to +0xC0).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, CTCShapeManager::GetPrincipalVector
//   (C3DVector&) const @ 0x00760910 (its bool result is ignored).
// Shape: the inline out-parameter PeekTC<CTCShapeManager> (interface 0x44: bitset word +0x28 bit 0x10, key in its
//   own frame word, `type < it->type` selects end); the out pointer is NOT initialised, so the absent path reads
//   whatever its home slot holds (`mov eax,[esp+14h]`), as every landed GetInterface caller. The vector is a named
//   local filled by GetPrincipalVector and returned through C3DVector's memberwise copy constructor.
// Layout: engine/CTCBase.h (ParentThing +4); CThing interface bitset +0x20 and interface map +0x44 as the landed
//   CTCCameraPointTrack::Validate 0x0075DC00.
#include "engine/CTCBase.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCShapeManager {
public:
    bool GetPrincipalVector(C3DVector& out) const;     // 0x00760910
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> bool PeekTC(long type, const T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCCameraPointFixedDolly : public CTCBase {
public:
    C3DVector GetFixedDollyVector() const;
};

C3DVector CTCCameraPointFixedDolly::GetFixedDollyVector() const
{
    const CTCShapeManager* shapes;
    ParentThing->PeekTC(0x44, &shapes);
    C3DVector v;
    shapes->GetPrincipalVector(v);
    return v;
}