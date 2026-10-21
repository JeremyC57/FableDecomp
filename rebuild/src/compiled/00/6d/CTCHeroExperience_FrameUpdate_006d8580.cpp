// CTCHeroExperience::FrameUpdate() -- per-frame upkeep: unless the thing manager is paused, count down the multiplier attenuation timer or decay the experience multiplier param (clamped at 0) and fire OnComboMultiplierDecrease when the pickup multiplier drops; age out pickups; remember the param; clear the whole attack history once its last entry is more than three seconds old
// Leaf: FrameUpdate
//
// Retail 0x006D8580, 330 bytes (+6 int3 padding, plain `ret`). Vtable 0x0125F6C4 slot 9 (the catalog's "vfunc_9"; slot 9 of the
// CTCBase vtable is FrameUpdate).
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (twice) and CWorld's const GetThingManager (virtual slot 4, 0x0049E1C0: returns
// +0x50), CTCHeroExperience::GetActiveComboMultiplierData() const @ 0x006D72D0, _ftol2 @ 0x00BFEA70, OnComboMultiplierDecrease
// @ 0x006D7E70, free @ 0x00BFEA14 (the erased pickup node), CTCBase::GetWorld() @ 0x00686A70 (result unused) and the static
// `__fastcall` CWorld::GetWorldFrame @ 0x0049D870; g_FramesPerSecond @ 0x01375550; float pools 0.0f @ 0x0122DEDC, -3.0f
// @ 0x0125F734, double 1.0 @ 0x01237F00 (the inline GetExperiencePickupMultiplierForParam adds 1.0, not 0.5:
// the landed 0x006D6570 / 0x006D64B0 models spell 0.5, which the relocation mask hides).
// Shape: the landed CTCHeroExperience model (GetRoughExperienceLevel 0x006D64B0): the pickup multiplier is the inline
// `(long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5)` clamped to 1, taken before and after the decay;
// the clamp is an inline `ClampToZero(float&)` (written inline after the `-=`, VC keeps the value with `fst`; through the
// reference parameter it stores and reloads); the paused test goes through a const `this` so the const GetWorld/GetThingManager
// overloads are called (overloaded virtuals are laid out in reverse declaration order); STLport list erase inlined (unlink, free,
// continue at next); the last attack is reached as the header's prev node (`*--end()`).
// Layout: retail CTCHeroExperience (12-byte vectors, 4-byte lists): ExperienceMultiplierParam +0x30, LastExperienceMultiplierParam
// +0x34, ExperienceAttenuationTimer +0x38, Pickups +0x40, AttackHistory +0x50, PDef +0x54; retail CThingManager pause flags +0x54
// NonScriptedEntitesPauseMode / +0x55 AllEntitesPauseMode (PDB +0x68/+0x69 less two 16->12 byte vectors).
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

struct CThingManager {
    unsigned char _pad00[0x54];
    bool NonScriptedEntitesPauseMode;               // +0x54
    bool AllEntitesPauseMode;                       // +0x55
};
struct CWorld {
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual const CThingManager* GetThingManager() const;  // slot 4 (0x0049E1C0) -- overloads are laid out in reverse order
    virtual CThingManager* GetThingManager();       // slot 3 (0x0049E1B0)
    static long __fastcall GetWorldFrame();         // 0x0049D870
};
extern long g_FramesPerSecond;                      // 0x01375550

template <class T> struct CListNode {
    CListNode* _M_next;
    CListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {             // STLport list<T>: header-node pointer only
    CListNode<T>* _M_node;
    CListNode<T>* begin() const { return _M_node->_M_next; }
    CListNode<T>* end() const { return _M_node; }
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& back() const { return _M_node->_M_prev->_M_data; }
    CListNode<T>* erase(CListNode<T>* pos)
    {
        CListNode<T>* next = pos->_M_next;
        CListNode<T>* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(pos);
        return next;
    }
};
struct SPickup { long FramesLeft; long Amount; };   // pair<long,long>
struct SAttack { long Attribute; long Frame; };     // pair<EDamageAttribute,long>
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CComboMultiplierData {
    unsigned char ComboMultiplier[0xc];
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0c
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};
struct CHeroExperienceDef {
    unsigned char _base[0x64];
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
};

inline void ClampToZero(float& value) { if (value < 0.0f) value = 0.0f; }
class CTCHeroExperience {
public:
    virtual void v00();
    void* ParentThing;                                      // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    CRetailVector<long> StatExperiencePoints;               // +0x18
    CRetailVector<long> TrainableStatLevels;                // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3c
    CRetailList<SPickup> Pickups;                           // +0x40
    CRetailVector<long> ExperienceSpentOnStats;             // +0x44
    CRetailList<SAttack> AttackHistory;                     // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    CWorld* GetWorld();                                     // 0x00686A70
    const CWorld* GetWorld() const;                         // 0x00686A80
    const CComboMultiplierData& GetActiveComboMultiplierData() const;   // 0x006D72D0
    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 1.0);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    void OnComboMultiplierDecrease();                       // 0x006D7E70
    virtual void FrameUpdate();
};

void CTCHeroExperience::FrameUpdate()
{
    const CTCHeroExperience* self = this;
    if (!self->GetWorld()->GetThingManager()->NonScriptedEntitesPauseMode &&
        !self->GetWorld()->GetThingManager()->AllEntitesPauseMode) {
        long oldMultiplier = GetExperiencePickUpMultiplier();
        if (ExperienceAttenuationTimer > 0) {
            --ExperienceAttenuationTimer;
        } else {
            ExperienceMultiplierParam -= GetActiveComboMultiplierData().ExperienceMultiplierParamDecayDeltaPerSecond / g_FramesPerSecond;
            ClampToZero(ExperienceMultiplierParam);
        }
        if (GetExperiencePickUpMultiplier() < oldMultiplier)
            OnComboMultiplierDecrease();
    }
    for (CListNode<SPickup>* node = Pickups.begin(); node != Pickups.end(); ) {
        if (--node->_M_data.FramesLeft == 0)
            node = Pickups.erase(node);
        else
            node = node->_M_next;
    }
    LastExperienceMultiplierParam = ExperienceMultiplierParam;
    long frame = GetWorld()->GetWorldFrame();
    if (!AttackHistory.empty()) {
        CListNode<SAttack>* last = AttackHistory.end()->_M_prev;
        if (last->_M_data.Attribute != -1 && frame > last->_M_data.Frame - (long)(g_FramesPerSecond * -3.0f)) {
            for (CListNode<SAttack>* node = AttackHistory.begin(); node != AttackHistory.end(); node = node->_M_next) {
                node->_M_data.Attribute = -1;
                node->_M_data.Frame = 0;
            }
        }
    }
}