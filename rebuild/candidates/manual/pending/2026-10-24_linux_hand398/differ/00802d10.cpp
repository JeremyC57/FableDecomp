// NPlayerTargeting::IsThingInTargetingRangeForTargetType(const CThing& thing, const CThing& target, unsigned long type) -- true when the target lies inside the thing's targeting range for that target type (2D distance, range from the thing's targeting sub-def and its primary-slot weapon)
// Grade: DIFFER(231v231) -- register allocation only: the sub-def holder `targetingDef` loads into esi before the two GetPosition calls (retail loads it after the delta X store, `mov esi,[esp+18h] ; mov edx,esi`), so the thing position lands in edi instead of esi; declaring the holder after the delta (v3) loads it late but swaps the thing pointer to esi
// Leaf: ?IsThingInTargetingRangeForTargetType@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802D10, 231 bytes (+ int3 padding), __fastcall (thing in ecx, target in
//   edx, type on the stack, `ret 4`), bool in al. Caller: 0x006AA996.
// Name: Ego_r.pdb `?IsThingInTargetingRangeForTargetType@NPlayerTargeting@@YI_NABVCThing@@0K@Z` (the catalog label is the
//   bare leaf).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0, CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CDefClassBase::GetSubDef<CTargetingDef>(CDefPointer<const CTargetingDef>*) 0x007EA960 (manifest
//   propagates a CAICreatureWillPowerIndicatorDef instantiation), CThing::GetPosition 0x004C73D0 (twice),
//   GetTargetingRangeForTargetType(unsigned long, const CTargetingDef&, const CThing&, const CThing*) 0x008028D0
//   (__fastcall, file-static ?A0x58ed6966 in Ego_r.pdb); the defs' slot 1 (Destroy) on release.
// Layout (retail CThing): UsingComponents bitset +0x20 (interface 0x46 = CTCCarrying: byte +0x28 bit 6), component
//   vector map +0x44 (8-byte {type, TC} entries, finish +0x48), PDef +0x70; def objects count references at +0x04.
// Shape: the carried weapon pointer is preloaded with 0 (`xor ebx,ebx`) and set only when the thing has the carrying TC
//   (one-select GetTC: `it == end || type < it->Type` picks end); the owner def is read into a raw local, the sub-def out
//   pointer zeroed between that load and its null-tested add-ref, released by hand after GetSubDef; the delta is a
//   12-byte C3DVector temporary (`target - thing`, Z unused: the fourth frame word) whose X/Y squares are compared with
//   range * range (Y term first); the bool survives the null-tested release of the sub-def.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long m_refCount;                                     // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
};
struct CTargetingDef : CDefPointeeBase { };
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CTargetingDef>* out);     // 0x007EA960
};

class CThing;
struct CTCBase { };
struct CTCCarrying : CTCBase {
    CThing* GetThingInPrimarySlot();                     // 0x006A1440
};
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CTCBitset {
    unsigned long _M_w[9];
    __forceinline bool test(long bit) const { if (_M_w[bit >> 5] & (1UL << (bit & 31))) return true; return false; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CTCBitset     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x70 - 0x50];
    CDefPointer<CDefClassBase> PDef;                     // +0x70
    const C3DVector& GetPosition() const;                // 0x004C73D0
    __forceinline bool HasTC(long type) const { return UsingComponents.test(type); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        unsigned long key = type;
        const CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return (T*)it->TC;
    }
};

float __fastcall GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef& def, const CThing& target,
                                                const CThing* carried);   // 0x008028D0

namespace NPlayerTargeting {
bool __fastcall IsThingInTargetingRangeForTargetType(const CThing& thing, const CThing& target, unsigned long type)
{
    const CThing* carried = 0;
    if (thing.HasTC(0x46))
        carried = thing.GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
    {
        CDefClassBase* thingDef = thing.PDef.Object;
        CDefPointer<CTargetingDef> def;
        def.Object = 0;
        if (thingDef)
            ++thingDef->m_refCount;
        thingDef->GetSubDef(&def);
        if (thingDef && --thingDef->m_refCount == 0)
            thingDef->Destroy();
        CTargetingDef* targetingDef = def.Object;
        C3DVector delta = target.GetPosition() - thing.GetPosition();
        float range = GetTargetingRangeForTargetType(type, *targetingDef, target, carried);
        float distanceSquared = delta.Y * delta.Y;
        distanceSquared += delta.X * delta.X;
        bool result = distanceSquared < range * range;
        if (targetingDef && --targetingDef->m_refCount == 0)
            targetingDef->Destroy();
        return result;
    }
}
}
