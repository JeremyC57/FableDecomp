// CHeroExperienceDef::~CHeroExperienceDef() -- destroy the attack-combo vector and the seven maps out of line, free the three long vectors, then the inlined CDefPointeeBase destructor stores its vtable and tail-calls ~CResource
// Leaf: ??1CHeroExperienceDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EBB1C, length 135 bytes (size-optimised module: `pop ecx` after free).
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller
// is the scalar deleting destructor @ 0x004EBB00, slot 0 of CHeroExperienceDef's vtable 0x0124390C.
// Callees: vector<CAttackHistoryCombo> dtor @ 0x004E81C1, map<EDamageAttribute,CHeroStatIncreaseData> dtor @ 0x004E81FB,
// map<long,CDefString> dtor @ 0x004DD21C (x5), map<long,CComboMultiplierData> dtor @ 0x004E81E4 (all catalogued as the
// propagated "CActiveFile::OnReadFinished"), free @ 0x00BFEA14 (x3), CResource::~CResource @ 0x009FC550 (tail jump).
// Layout: Ego_r.pdb CHeroExperienceDef with the retail 12-byte STLport containers (PDB 16): ExperienceLevelThresholds
// +0x28, HeroLevelTitles +0x34, FallbackMultipliers +0x40, SoundIntensityThresholds +0x4c, ComboMultiplierMap +0x58,
// two floats +0x64/+0x68, ComboMultiplierIncrease +0x6c, ComboMultiplierLock +0x78, ComboMultiplierLost +0x84,
// ComboMultiplierRunningHitsIncrease +0x90, StatIncreasePerHitType +0x9c, AttackHistoryCombos +0xa8 (retail 0xb4).
// Shape: members destroyed in reverse order (the map/combo-vector destructors out of line, the vector<long> destructor
// inline `if (start) free(start)`), the most-derived class `__declspec(novtable)` (no own vtable store), then the
// inlined CDefPointeeBase destructor's store (0x01230BA0) and the tail jump; `#pragma optimize("s", on)`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CResource_Calls {                           // engine/CResource.h: the root of the def hierarchy
    virtual void v00();
    ~CResource_Calls();                            // 0x009FC550
};
class CDefPointeeBase : public CResource_Calls { public:         // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct CLongVector {                               // retail vector<long>: start / finish / end_of_storage
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    ~CLongVector() { if (_M_start) free(_M_start); }
};
struct CLongDefStringMap {                         // map<long,CDefString>
    unsigned char _body[0x0c];
    ~CLongDefStringMap();                          // 0x004DD21C
};
struct CComboMultiplierMap {                       // map<long,CComboMultiplierData>
    unsigned char _body[0x0c];
    ~CComboMultiplierMap();                        // 0x004E81E4
};
struct CStatIncreaseMap {                          // map<EDamageAttribute,CHeroStatIncreaseData>
    unsigned char _body[0x0c];
    ~CStatIncreaseMap();                           // 0x004E81FB
};
struct CAttackHistoryComboVector {                 // vector<CAttackHistoryCombo>
    unsigned char _body[0x0c];
    ~CAttackHistoryComboVector();                  // 0x004E81C1
};

struct __declspec(novtable) CHeroExperienceDef : CDefPointeeBase {   // vtable 0x0124390C, retail 0xb4 bytes
    unsigned char             _base[0x28 - 0x04];
    CLongVector               ExperienceLevelThresholds;            // +0x28
    CLongVector               HeroLevelTitles;                      // +0x34
    CLongVector               FallbackMultipliers;                  // +0x40
    CLongDefStringMap         SoundIntensityThresholds;             // +0x4c
    CComboMultiplierMap       ComboMultiplierMap;                   // +0x58
    float                     ExperienceMultiplierParamPerUnitComboMultiplier;   // +0x64
    float                     ExperienceMultiplierRegionChangeMultiplier;        // +0x68
    CLongDefStringMap         ComboMultiplierIncrease;              // +0x6c
    CLongDefStringMap         ComboMultiplierLock;                  // +0x78
    CLongDefStringMap         ComboMultiplierLost;                  // +0x84
    CLongDefStringMap         ComboMultiplierRunningHitsIncrease;   // +0x90
    CStatIncreaseMap          StatIncreasePerHitType;               // +0x9c
    CAttackHistoryComboVector AttackHistoryCombos;                  // +0xa8
    virtual void v00();
    ~CHeroExperienceDef();
};

CHeroExperienceDef::~CHeroExperienceDef()
{
}