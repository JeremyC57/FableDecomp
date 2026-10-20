// CTCStealth::GetVisibilityMultiplier() const -- how visible the thing is: the stealth def's day / night / twilight multiplier when its interface-0x27 component is in effect (else 1), times the interface-0x68 component's multiplier and the interface-4 component's factor when present
// Leaf: GetVisibilityMultiplier
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747660, 289 bytes (plain `ret`, float in st0).
// Callees: the const interface LowerBound @ 0x004365B0 (interfaces 0x27, 0x68, 4), CTCBase::GetWorld() const @ 0x00686A80
//   (twice), CGameTimeManager::IsDay() const @ 0x006BB2C0, CGameTimeManager::IsNightTime() const @ 0x006BB210, the 0x68
//   component's multiplier getter @ 0x006D6620 (manifest "CTCHeroExperience::GetSoundRadiusMultiplier", unverified). 0.5f
//   from 0x0122F59C.
// Layout: Ego_r.pdb CTCStealth PDef +0x1C (CStealthDef: day multiplier +0x38, night multiplier +0x3C, named after the landed
//   CTCPerceiveThing::GetVisibilityMultiplier 0x007448F0 shape; Ego_r.pdb names them DayTimeLamp/NightTimeLampVisibilityMultiplier); retail CThing interface bitset +0x20 (0x27 = byte +0x24 bit 7,
//   0x68 = word +0x2C bit 8, 4 = byte +0x20 bit 4), map +0x44; the 0x27 component's flag +0x1C, the interface-4 component's
//   factor +0xAC (named descriptively); world game-time manager +0x1C.
// Shape: a float local starting at 1.0f; the 0x27 and interface-4 components come from the out-parameter GetInterface (the
//   component stays in eax; HasTC + GetTC puts it in ecx), the 0x68 one from HasTC + GetTC; the day/night choice is the
//   landed CTCPerceiveThing inline as a `__forceinline` member (the def re-read through PDef each time); the interface-4
//   product is returned directly.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CGameTimeManager {
    bool IsDay() const;                                 // 0x006BB2C0
    bool IsNightTime() const;                           // 0x006BB210
};
struct CWorld {
    unsigned char     _pad00[0x1c];
    CGameTimeManager* GameTimeManager;                  // +0x1c
};
struct CStealthDef {
    unsigned char _pad00[0x38];
    float DayTimeLampVisibilityMultiplier;                  // +0x38
    float NightTimeLampVisibilityMultiplier;                // +0x3c
};
struct CTCStealthModifier27 {                           // interface 0x27 (named descriptively)
    unsigned char _pad00[0x1c];
    bool          AffectsVisibility;                    // +0x1c
};
struct CTCVisibility68 {                                // interface 0x68
    float GetVisibilityMultiplier() const;              // 0x006D6620
};
struct CTCVisibility4 {                                 // interface 4
    unsigned char _pad00[0xac];
    float         VisibilityFactor;                     // +0xac
};
struct CTCStealth {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    const CStealthDef* PDef;                            // +0x1c CDefPointer<const CStealthDef>
    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)
    const CStealthDef* GetDef() const { return PDef; }
    __forceinline float GetTimeOfDayVisibilityMultiplier() const
    {
        if (GetWorld()->GameTimeManager->IsDay())
            return GetDef()->DayTimeLampVisibilityMultiplier;
        if (GetWorld()->GameTimeManager->IsNightTime())
            return GetDef()->NightTimeLampVisibilityMultiplier;
        return (GetDef()->NightTimeLampVisibilityMultiplier + GetDef()->DayTimeLampVisibilityMultiplier) * 0.5f;
    }
    float GetVisibilityMultiplier() const;
};

float CTCStealth::GetVisibilityMultiplier() const
{
    float multiplier = 1.0f;
    CTCStealthModifier27* modifier;
    if (ParentThing->GetInterface(0x27, &modifier) && modifier->AffectsVisibility)
        multiplier = GetTimeOfDayVisibilityMultiplier();
    if (ParentThing->HasTC(0x68))
        multiplier *= ParentThing->GetTC<CTCVisibility68>(0x68)->GetVisibilityMultiplier();
    CTCVisibility4* visibility;
    if (ParentThing->GetInterface(4, &visibility))
        return multiplier * visibility->VisibilityFactor;
    return multiplier;
}
