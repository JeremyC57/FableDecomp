// CEngineInternalPrimitiveWeaponTrail::Update(const CEnginePrimitive&, unsigned long) -- copy the weapon-trail primitive state and rebuild the two vertex tracks and creation-time array
// Native identity: RTTI vtable 0x012A6B24 slot 5 points here; retail class construction/destruction at 00BCD600/00BCDE40 confirms a 0x48 CEngineInternalPrimitiveBase prefix, 12-byte vector headers at +0x5c/+0x68, and the 0x7c native extent. The generated internal-trail donor layout incorrectly expands CArray fields to 16 bytes; this view follows retail accesses and the destructor.
// Input view: the function's CEnginePrimitive reference has a weapon-trail extension. Its one-word list head and following members match retail reads (+0x0c..+0x2c); CWeaponTrailEntry Top/Bottom/CreationTime and list-node value offset +8 match Ego_r.pdb. No weapon-trail input header exists.
// The base header's __vftable member is the real stored engine vptr. This update entry is modeled as a nonvirtual body method so no incomplete/synthetic vtable is emitted.
// Callees: vector<CNativeVector12<C3DVector>>::resize (12-byte element helper, retail 00BCDE70), vector<C3DVector>::resize (0075C040), vector<float>::resize (00B442770), CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults (00B8FD40).

#include "engine/CEngineInternalPrimitiveBase.h"

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
};

struct CWeaponTrailEntry {
    C3DVector Top;
    C3DVector Bottom;
    float CreationTime;
};

struct CWeaponTrailListNode {
    CWeaponTrailListNode* Next;
    CWeaponTrailListNode* Previous;
    CWeaponTrailEntry Entry;
};

struct CWeaponTrailListHead {
    CWeaponTrailListNode* Head;
};

class CEnginePrimitive { public:
    unsigned long Type;
    unsigned long InterpolationSet;
    unsigned long ThemeChannelDesc;
};

struct CEnginePrimitiveWeaponTrailUpdateView : CEnginePrimitive {
    CWeaponTrailListHead Trail;
    unsigned long GraphicIndex;
    unsigned long GraphicFrame;
    unsigned long TrailType;
    float SkipPoints;
    float FadeDistance;
    unsigned char Alpha;
    unsigned char _pad_0x25[3];
    float CurrentTime;
    float FallOffTime;
};

template <class T>
struct CNativeVector12 {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;

    CNativeVector12() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void resize(unsigned long newSize, const T& initialValue = T());
    T& operator[](unsigned long index) { return _M_start[index]; }
};

class CEngineInternalPrimitiveWeaponTrail : public CEngineInternalPrimitiveBase { public:
    unsigned long TrailType;
    unsigned long GraphicIndex;
    unsigned long GraphicFrame;
    unsigned char Alpha;
    float SkipPoints;
    CNativeVector12<CNativeVector12<C3DVector> > Trail;
    CNativeVector12<float> TrailElementCreationTime;
    float CurrentTime;
    float FallOffTime;

    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);
    void Update(const CEnginePrimitive& primitive, unsigned long primitiveFlags);
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWeaponTrailEntry) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CWeaponTrailListNode, Entry) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, Trail) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, GraphicIndex) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, GraphicFrame) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, TrailType) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, SkipPoints) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, FadeDistance) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, Alpha) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, CurrentTime) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveWeaponTrailUpdateView, FallOffTime) == 0x2c);
FABLE_STATIC_ASSERT(sizeof(CNativeVector12<C3DVector>) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, TrailType) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, GraphicIndex) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, GraphicFrame) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, Alpha) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, SkipPoints) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, Trail) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, TrailElementCreationTime) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, CurrentTime) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveWeaponTrail, FallOffTime) == 0x78);
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveWeaponTrail) == 0x7c);
#endif

void CEngineInternalPrimitiveWeaponTrail::Update(const CEnginePrimitive& primitive, unsigned long primitiveFlags)
{
    const CEnginePrimitiveWeaponTrailUpdateView& source =
        (const CEnginePrimitiveWeaponTrailUpdateView&)primitive;
    EnginePrimitiveFlags = primitiveFlags;
    TrailType = source.TrailType;
    GraphicIndex = source.GraphicIndex;
    GraphicFrame = source.GraphicFrame;
    Alpha = source.Alpha;
    SkipPoints = source.SkipPoints;
    CurrentTime = source.CurrentTime;
    FallOffTime = source.FallOffTime;

    unsigned long count = 0;
    const CWeaponTrailListNode* node;
    for (node = source.Trail.Head->Next;
         node != source.Trail.Head;
         node = node->Next)
        ++count;

    CNativeVector12<C3DVector> emptyTrack;
    Trail.resize(2, emptyTrack);
    Trail[0].resize(count);
    Trail[1].resize(count);
    TrailElementCreationTime.resize(count, 0.0f);

    unsigned long index = 0;
    for (node = source.Trail.Head->Next;
         node != source.Trail.Head;
         node = node->Next, ++index) {
        Trail[0][index] = node->Entry.Top;
        Trail[1][index] = node->Entry.Bottom;
        TrailElementCreationTime[index] = node->Entry.CreationTime;
    }

    SetFadeDistanceWithGamePrimitiveDefaults(source.FadeDistance);
}