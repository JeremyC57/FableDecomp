// CAIStateGroup_SitInPub::Update() -- sit down on the seat, then idle while the idle action repeats, re-rolling the
// Leaf: ?Update@CAIStateGroup_SitInPub@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// next fidget frame (half the global interval plus a random part of it) each time it passes; when done, leave the
// pub building's customer list (component 0x7B) and terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094B740, length 246 bytes (+ int3 padding). Vtable slot 12 of
// CAIStateGroup_SitInPub (0x0128BF04).
// Callees: CAIStateGroup_UsesSeat::MoveToSeatAndSitDown(EMovementType) @ 0x009387C0, CWorld::GetFrame @ 0x0049D870,
// AIAction_DoNothing @ 0x0097E200, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the seat pointer, result unused,
// and PBuilding x3), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 0x7B, bit 27 of +0x2c), the pub component's RemoveCustomer(CThing*)
// @ 0x007FFDB0, Terminate @ 0x008FCF40. Global: the fidget interval long @ 0x013BB730.
// Shape: the random step is the landed GetRandomObject one with the `range ? seed % range : 0` guard; the interval
// halves with the signed `cdq ; sub ; sar 1`; the interval global is read before the seed store (retail loads it
// first; the natural store-then-read order matches too under --flags /Oa); the building component uses the
// `if (HasTC) { T* p; GetInterfaceV(&p) }` form (eax then ecx) on a named building local.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BB730 TIME_TO_LEAVE_BETWEEN_SPEAKING (anonymous namespace, const; initializer 0x01221CC0), was g_pubFidgetInterval.
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
public:
    void Terminate();   // 0x008FCF40
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
namespace { extern const long TIME_TO_LEAVE_BETWEEN_SPEAKING; }                // 0x013BB730
struct CTCPub {                                 // interface 0x7B on the pub building
    void RemoveCustomer(CThing* customer);      // 0x007FFDB0
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CBaseIntelligentPointer PSeat;              // +0x14 CIntelligentPointer<CThing>
    unsigned char _pad1c[0x34 - 0x1c];
    bool MoveToSeatAndSitDown(long movement);   // 0x009387C0
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_SitInPub : CAIStateGroup_UsesSeat {
    CBaseIntelligentPointer PBuilding;          // +0x34 CIntelligentPointer<CThing>
    virtual void Update();
};

void CAIStateGroup_SitInPub::Update()
{
    if (MoveToSeatAndSitDown(3)) {
        long nextFidget = CWorld::GetFrame();
        if (AIAction_DoNothing() == 4) {
            do {
                long now = CWorld::GetFrame();
                if (now > nextFidget) {
                    ((CThing*)PSeat.GetPItem());
                    unsigned long* seed = GetPWorldSeed();
                    unsigned long next = *seed * 0x24A1U + 0x24DFU;
                    *seed = next;
                    unsigned long rotated = GFROR13(next);
                    long interval = TIME_TO_LEAVE_BETWEEN_SPEAKING;
                    *seed = rotated;
                    unsigned long r = interval ? rotated % interval : 0;
                    nextFidget = interval / 2 + r + now;
                }
            } while (AIAction_DoNothing() == 4);
        }
    }
    if (IS_THING_ALIVE(PBuilding)) {
        CThing* building = ((CThing*)PBuilding.GetPItem());
        if (building->HasTC(TCI_GATHER_POINT)) {
            CTCPub* pub;
            building->GetTC(TCI_GATHER_POINT, &pub);
            pub->RemoveCustomer(PBrain->PCreature);
        }
    }
    Terminate();
}