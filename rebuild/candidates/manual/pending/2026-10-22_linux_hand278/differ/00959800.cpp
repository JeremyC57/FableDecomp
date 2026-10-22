// Grade: DIFFER(625v595) -- retail stacks customers/nodes/area upward and reuses the customers vector's words for the CIntelligentPointer temp while caching its start in edi for the late free; ours orders the slots the other way and reloads
// CAIStateGroup_HoverWanderAroundThing::IsRunnable() -- runnable while the wander-around thing is alive; otherwise gather the track nodes and potential customers in a 20x20 rough area around the creature and adopt a random customer (else a random track node) as the new thing; false when neither exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00959800, length 595 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, _ftol @ 0x00BFEA70,
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CThingSearchTools rough-area searches @ 0x00959380 (track nodes) /
// 0x009595C0 (potential customers), CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890 (`__fastcall`),
// GFRandom @ 0x004CDAE0 (`__fastcall`), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor
// @ 0x00A01C10, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CAIStateGroup_HoverWanderAroundThing PThing +0x1c; retail STLport vectors are 12 bytes.
#include <stdlib.h>
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    void* vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
extern void* const CIntelligentPointer_CThing_vftable;   // 0x0125C29C
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { *(void* const**)this = &CIntelligentPointer_CThing_vftable; SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CThingList {                             // STLport vector<CThing*>, retail 12 bytes
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingList() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CRoughArea {
    long X0, Y0, X1, Y1;
    CRoughArea(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
struct CThingFilter { };
struct CThingSearchTools {
    void GetTrackNodesInRoughArea(const CRoughArea& area, const CThingFilter& filter, CThingList* out);   // 0x00959380
    void GetCustomersInRoughArea(const CRoughArea& area, const CThingFilter& filter, CThingList* out);    // 0x009595C0
};
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
inline unsigned long WorldRandomRange(unsigned long* seed, unsigned long range)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    if (range == 0)
        return 0;
    return *seed % range;
}
struct CWorld {
    unsigned char _pad0[0x20];
    CThingSearchTools* SearchTools;             // +0x20
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
};
struct CAIStateGroup_HoverWanderAroundThing : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    CIntelligentPointer<CThing> PThing;         // +0x1c
    bool IsRunnable();
};

bool CAIStateGroup_HoverWanderAroundThing::IsRunnable()
{
    CIntelligentPointer<CThing>& thing = PThing;
    if (IS_THING_ALIVE(thing))
        return true;
    CThingList customers;
    CThingList nodes;
    CThingFilter filter;
    {
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        CRoughArea area(x - 10, y - 10, x + 10, y + 10);
        CWorld* world = GetPWorld();
        world->SearchTools->GetTrackNodesInRoughArea(area, filter, &nodes);
    }
    {
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        CRoughArea area(x - 10, y - 10, x + 10, y + 10);
        CWorld* world = GetPWorld();
        world->SearchTools->GetCustomersInRoughArea(area, filter, &customers);
    }
    if (!customers.empty()) {
        CThing* pick = customers._M_start[WorldRandomRange(GetPWorld()->GetPRandomSeed(), customers.size())];
        thing = CIntelligentPointer<CThing>(pick);
        return true;
    }
    if (!nodes.empty()) {
        CThing* pick = nodes._M_start[GFRandom(nodes.size(), GetPWorld()->GetPRandomSeed())];
        thing = CIntelligentPointer<CThing>(pick);
        return true;
    }
    return false;
}
