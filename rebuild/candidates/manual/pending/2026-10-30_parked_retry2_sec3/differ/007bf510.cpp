// Grade: DIFFER(581v635) -- 189 difflines (base 583v635/236); __forceinline memberwise C3DVector copy ctor (the position/normal copies become mov pairs). Left: frame 0x34 vs 0x18 -- retail keeps the hit thing in ebp and homes the hit-list bool, the find iterator tag and the 0x4c key in the dead thing/frame parameter slots and folds back() into [eax-30h] displacements.
// CTCShotDefinitionBase::OnCollision(long frame, const C3DVector& pos, const C3DVector& normal, CThing* thing) -- ignore things already hit; otherwise record the collision nearest the weapon-trail tip (or, with no trail points, the shooter's position and an up normal)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF510, 640 bytes, `ret 0x10` (esp frame).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   STLport __find<CIntelligentPointer<CThing>*> 0x007C2980 (fastcall, 8-byte elements, unrolled by 4),
//   CVectorMap::LowerBound 0x0040F020, CTCWeaponTrail::HasWeaponTrailPoints 0x0076E4A0 / point getter 0x0076E5D0 (catalog
//   "_Umove<EWeaponClass*>", propagated), CIntelligentPointer<const CThing>(const CThing*) 0x004A96F0 (out of line),
//   CThing::GetPosition 0x004C73D0, CRT free 0x00BFEA14.
// Layout: shot def hit list (vector<CIntelligentPointer<CThing>>) begin +0x14 / end +0x18, collision position +0x70, normal
//   +0x7c, collision thing pointer +0x88, collision frame +0x90; CThing flags byte +0x91 (bit 0 skips the hit-list test);
//   weapon-trail point stride 0x30 with the position first.
// Leaf: OnCollision
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z; __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                 // vtable 0x01238C6C
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct CThing;
template <> struct CIntelligentPointer<const CThing> : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(const CThing* item);            // 0x004A96F0 (out of line)
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
typedef CIntelligentPointer<const CThing> CThingPtr;
struct random_access_iterator_tag { };
CIntelligentPointer<CThing>* __fastcall StdVector_Find(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last,
                                                       const CIntelligentPointer<CThing>& value,
                                                       const random_access_iterator_tag&);   // 0x007C2980

struct STrailPoint { C3DVector Pos; unsigned char _rest[0x30 - 0x0c]; };
struct CTrailPointVector {
    STrailPoint* _M_start;
    STrailPoint* _M_finish;
    STrailPoint* _M_end_of_storage;
    CTrailPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CTrailPointVector() { if (_M_start) free(_M_start); }
    STrailPoint& back() { return _M_finish[-1]; }
};
struct CTCWeaponTrail {
    bool HasWeaponTrailPoints();                        // 0x0076E4A0
    bool GetWeaponTrailPoints(CTrailPointVector* out);  // 0x0076E5D0
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return ((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

struct CTCShotDefinitionBase {
    void*          __vftable;                           // +0x00
    CThing*        ParentThing;                         // +0x04
    unsigned char  _pad08[0x14 - 0x08];
    CIntelligentPointer<CThing>* HitStart;              // +0x14
    CIntelligentPointer<CThing>* HitFinish;                           // +0x18
    unsigned char  _pad1c[0x70 - 0x1c];
    C3DVector      CollisionPos;                        // +0x70
    C3DVector      CollisionNormal;                     // +0x7c
    CThingPtr      PCollisionThing;                     // +0x88
    long           CollisionFrame;                      // +0x90
    void OnCollision(long frame, const C3DVector& pos, const C3DVector& normal, CThing* thing);
};

void CTCShotDefinitionBase::OnCollision(long frame, const C3DVector& pos, const C3DVector& normal, CThing* thing)
{
    if (thing && !(thing->Flags91 & 1) && StdVector_Find(HitStart, HitFinish, CIntelligentPointer<CThing>(thing), random_access_iterator_tag()) != HitFinish)
        return;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x4c)) {
        CTCWeaponTrail* trail = (CTCWeaponTrail*)parent->GetTC(0x4c);
        CTrailPointVector points;
        if (trail->HasWeaponTrailPoints()) {
            if (trail->GetWeaponTrailPoints(&points)) {
                C3DVector tip = points.back().Pos;
                if (CollisionFrame == 0 || (pos - tip).GetLengthSquared() < (CollisionPos - tip).GetLengthSquared()) {
                    CollisionPos = pos;
                    CollisionNormal = normal;
                    PCollisionThing = CThingPtr(thing);
                    CollisionFrame = frame;
                }
            }
        } else {
            PCollisionThing = CThingPtr(0);
            CollisionFrame = frame;
            CollisionPos = parent->GetPosition();
            CollisionNormal = C3DVector(0.0f, 0.0f, 1.0f);
        }
    }
}
