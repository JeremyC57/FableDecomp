// CDefendingCombatantInfo::CCombatWheel::AddRing(const CCombatTypeDef& def) -- append a new combat ring (index = current ring count) built from the combat type, and return a reference to it
// Leaf: AddRing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EC7D0, length 118 bytes.
// Label correction: the manifest's "CDefendingCombatantInfo::RemoveCombatType" is wrong; Ego_r.pdb has
// `?AddRing@CCombatWheel@CDefendingCombatantInfo@@QAEAAVCCombatRing...` and the body pushes a ring and returns back().
// Callees: CCombatRing::CCombatRing(long index, const CCombatTypeDef&) @ 0x006EC380, vector<CCombatRing>::push_back(const
// CCombatRing&) @ 0x006F4930 (manifest "CDefendingCombatantInfo_PushBackProng"), CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10 (each prong's POccupier), free @ 0x00BFEA14 (STLport malloc allocator).
// Layout (Ego_r.pdb, retail 12-byte STLport vectors): CCombatWheel = {vector<CCombatRing> CombatRings}; CCombatRing (28
// bytes) = RotationAngle, DeltaRotation, Index, RingThickness, vector<CProng> Prongs +0x10; CProng (28 bytes) = Blocked +0,
// Prong (C3DVector) +4, ProngBlockageTimer +0x10, POccupier (CIntelligentPointer<CThingCreatureBase>) +0x14.
// Shape: the ring count is the signed (last - first) / 28 (reciprocal multiply) taken into a named `long` local first (the
// def argument is then read after the division; inline in the call it is loaded before the prologue); the temporary
// ring's implicit destructor inlines STLport's ~vector<CProng>: _Destroy(first, last) with by-value bounds (the end cached
// in ebx, the start re-read), then a null-tested free; back() is `end - 1`.
extern "C" void __cdecl free(void* block);

class CThingCreatureBase;
class CCombatTypeDef;
class C3DVector { public: float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                               // +4
    ~CBaseIntelligentPointer();                  // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
};

struct CProng {                                  // CDefendingCombatantInfo::CProng, 28 bytes
    bool                Blocked;                 // +0x00
    C3DVector           Prong;                   // +0x04
    long                ProngBlockageTimer;      // +0x10
    CIntelligentPointer POccupier;               // +0x14
};

static inline void _Destroy(CProng* first, CProng* last)   // STLport range destroy: the bounds are by-value locals
{
    for (; first != last; ++first)
        first->~CProng();
}

struct CProngVector {                            // STLport vector<CProng>
    CProng* _M_start;
    CProng* _M_finish;
    CProng* _M_end_of_storage;
    ~CProngVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CCombatRing {                             // CDefendingCombatantInfo::CCombatRing, 28 bytes
    float        RotationAngle;                  // +0x00
    float        DeltaRotation;                  // +0x04
    long         Index;                          // +0x08
    float        RingThickness;                  // +0x0c
    CProngVector Prongs;                         // +0x10
    CCombatRing(long index, const CCombatTypeDef& def);   // 0x006EC380
};

struct CCombatRingVector {                       // STLport vector<CCombatRing>
    CCombatRing* _M_start;
    CCombatRing* _M_finish;
    CCombatRing* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCombatRing& back() { return *(_M_finish - 1); }
    void push_back(const CCombatRing& ring);     // 0x006F4930
};

struct CCombatWheel {                            // CDefendingCombatantInfo::CCombatWheel
    CCombatRingVector CombatRings;               // +0x00
    CCombatRing& AddRing(const CCombatTypeDef& def);
};

CCombatRing& CCombatWheel::AddRing(const CCombatTypeDef& def)
{
    long index = CombatRings.size();
    CombatRings.push_back(CCombatRing(index, def));
    return CombatRings.back();
}