// CTCAvoidRegionExit::Update (vtable slot 9) -- every 5 frames: rebuild the shared region lists once per frame, clear the owner's avoid distance (TC 0xAA), drop the owner from the distance map, then among the live region-exit things (TC 0x64) pick the region whose exit the owner is moving past, and among the live avoiders pick the nearest; store the distance in the owner's TC and the map
// Grade: DIFFER(1232v1277) -- first full draft (static init and per-frame refresh, 5-frame throttle, map erase, region pick by the exit's front/back half-planes, nearest avoider, distance store); retail keeps this in ebp (ours ebx), the HasTC 0xAA as a dead stack bool, and its x87 sqrt/compare scheduling differs
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E9460, 1280 bytes, `ret`.
#include <math.h>
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct SFrame { unsigned char _pad[0xc]; C2DVector Forward; };
struct CThingController {
    virtual void s000(); virtual void s004(); virtual void s008(); virtual void s00c(); virtual void s010(); virtual void s014(); virtual void s018(); virtual void s01c();
    virtual void s020(); virtual void s024(); virtual void s028(); virtual void s02c(); virtual void s030(); virtual void s034(); virtual void s038(); virtual void s03c();
    virtual void s040(); virtual void s044(); virtual void s048(); virtual void s04c(); virtual void s050(); virtual void s054(); virtual void s058(); virtual void s05c();
    virtual void s060(); virtual void s064(); virtual void s068(); virtual void s06c(); virtual void s070(); virtual void s074(); virtual void s078(); virtual void s07c();
    virtual void s080(); virtual void s084(); virtual void s088(); virtual void s08c(); virtual void s090(); virtual void s094(); virtual void s098(); virtual void s09c();
    virtual void s0a0(); virtual void s0a4(); virtual void s0a8(); virtual void s0ac(); virtual void s0b0(); virtual void s0b4(); virtual void s0b8(); virtual void s0bc();
    virtual void s0c0(); virtual void s0c4(); virtual void s0c8(); virtual void s0cc(); virtual void s0d0(); virtual void s0d4(); virtual void s0d8(); virtual void s0dc();
    virtual void s0e0(); virtual void s0e4(); virtual void s0e8(); virtual void s0ec(); virtual void s0f0(); virtual void s0f4(); virtual void s0f8(); virtual void s0fc();
    virtual void s100(); virtual void s104(); virtual void s108(); virtual void s10c(); virtual void s110(); virtual void s114(); virtual void s118(); virtual void s11c();
    virtual const SFrame* GetFrame(void* scratch);      // +0x120
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingController* Controller;                       // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
    void Assign(const CBaseIntelligentPointer& other);  // 0x004A9760
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct SThingNode { SThingNode* Next; SThingNode* Prev; CBaseIntelligentPointer Thing; };
struct CThingDistanceMap {
    struct SIterator { void* Node; };
    SIterator* equal_range(SIterator* out, CThing* const& key);   // 0x007E9AE0
    void erase(SIterator first, SIterator last);        // 0x007E99F0
    float& operator[](CThing* const& key);              // 0x007EA1A0
};
struct C2DVectorOps { static const C2DVector& Normalise(const C2DVector& v); };   // 0x0049CE00
struct CTCRegionExit { unsigned char _pad[0x14]; float Radius; };
struct CTCAvoider { unsigned char _pad[0x7c]; float Distance; };
extern bool g_AvoidRegionInitialised;                   // 0x013BAEF0
extern unsigned long g_AvoidRegionFrame;                // 0x013BAEF4
extern SThingNode* g_RegionExits;                       // 0x013BAEF8
extern SThingNode* g_Avoiders;                          // 0x013BAEFC
extern CThingDistanceMap g_AvoidDistances;              // 0x013BAF00
unsigned long GetFrameNumber();                         // 0x0049D870

class CTCAvoidRegionExit {
public:
    void* __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer NearestAvoider;                 // +0x0c
    CIntelligentPointer Region;                         // +0x14
    unsigned long NextUpdate;                           // +0x1c
    void RefreshLists();                                // 0x007E9250
    void Reset();                                       // 0x007E92D0
    void Update();
};

void CTCAvoidRegionExit::Update()
{
    if (!g_AvoidRegionInitialised) {
        g_AvoidRegionInitialised = true;
        Reset();
    }
    if (g_AvoidRegionFrame != GetFrameNumber()) {
        g_AvoidRegionFrame = GetFrameNumber();
        RefreshLists();
    }
    if (NextUpdate > GetFrameNumber())
        return;
    NextUpdate = GetFrameNumber() + 5;
    CTCAvoider* avoider = 0;
    CThing* owner = ParentThing;
    if (owner->m_presentBits[5] & 0x400) {
        avoider = owner->GetTC<CTCAvoider>(0xaa);
        avoider->Distance = 0;
    }
    CThing* key = ParentThing;
    CThingDistanceMap::SIterator range[2];
    g_AvoidDistances.equal_range(range, key);
    g_AvoidDistances.erase(range[0], range[1]);
    long count = 0;
    for (SThingNode* n = g_RegionExits->Next; n != g_RegionExits; n = n->Next)
        ++count;
    if (count == 0)
        return;
    count = 0;
    for (SThingNode* n = g_Avoiders->Next; n != g_Avoiders; n = n->Next)
        ++count;
    if (count == 0)
        return;
    float nearest = 0.0f;
    Region = CIntelligentPointer(0);
    NearestAvoider = CIntelligentPointer(0);
    for (SThingNode* node = g_RegionExits->Next; node != g_RegionExits; node = node->Next) {
        CBaseIntelligentPointer* exit = &node->Thing;
        if (!exit->GetPItem() || exit->GetPItem()->IsDead()) {
            Reset();
            return;
        }
        if (!(exit->GetPItem()->m_presentBits[3] & 0x10))
            continue;
        float radius = exit->GetPItem()->GetTC<CTCRegionExit>(0x64)->Radius;
        const C3DVector& e = exit->GetPItem()->GetPosition();
        const C3DVector& o = ParentThing->GetPosition();
        float dx = o.X - e.X, dy = o.Y - e.Y, dz = o.Z - e.Z;
        float dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (!(radius + radius > dist))
            continue;
        unsigned char scratch[0x1c];
        const C2DVector& fwd = C2DVectorOps::Normalise(exit->GetPItem()->Controller->GetFrame(scratch)->Forward);
        C2DVector offset;
        offset.X = radius * fwd.X;
        offset.Y = radius * fwd.Y;
        const C3DVector& ep = exit->GetPItem()->GetPosition();
        C2DVector front;
        front.X = offset.X + ep.X;
        front.Y = offset.Y + ep.Y;
        const C3DVector& ep2 = exit->GetPItem()->GetPosition();
        C2DVector back;
        back.X = ep2.X - offset.X;
        back.Y = ep2.Y - offset.Y;
        const C3DVector& op = ParentThing->GetPosition();
        if ((op.X - back.X) * offset.X + (op.Y - back.Y) * offset.Y > 0.0f &&
            !((op.X - front.X) * offset.X + (op.Y - front.Y) * offset.Y > 0.0f))
            Region.Assign(*exit);
    }
    if (!Region.GetPItem())
        return;
    const SFrame* frame = Region.GetPItem()->Controller->GetFrame(0);
    C2DVector fwd = frame->Forward;
    const C3DVector& rp = Region.GetPItem()->GetPosition();
    const C3DVector& op = ParentThing->GetPosition();
    if (!((rp.X - op.X) * fwd.X + (rp.Y - op.Y) * fwd.Y > 0.0f))
        return;
    for (SThingNode* node = g_Avoiders->Next; node != g_Avoiders; node = node->Next) {
        CBaseIntelligentPointer* other = &node->Thing;
        if (!other->GetPItem() || other->GetPItem()->IsDead()) {
            Reset();
            return;
        }
        CThing* thing = other->GetPItem();
        const C3DVector& a = Region.GetPItem()->GetPosition();
        const C3DVector& b = thing->GetPosition();
        float dx = b.X - a.X, dy = b.Y - a.Y, dz = b.Z - a.Z;
        float distSq = dx * dx + dy * dy + dz * dz;
        if (!NearestAvoider.GetPItem() || !(nearest > distSq)) {
            NearestAvoider.SetPItem(other->GetPItem());
            nearest = distSq;
        }
    }
    nearest = (float)sqrt(nearest);
    if (avoider)
        avoider->Distance = nearest;
    CThing* self = ParentThing;
    g_AvoidDistances[self] = nearest;
}
