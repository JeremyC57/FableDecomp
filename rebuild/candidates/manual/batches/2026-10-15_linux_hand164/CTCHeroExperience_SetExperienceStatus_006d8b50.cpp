// CTCHeroExperience::SetExperienceStatus(const vector<long>& status) -- restore the experience totals, the per-stat experience points and the trainable stat levels from a flat status vector, then refresh the abilities of all 10 stats and signal the level change
// Leaf: SetExperienceStatus
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8B50, length 159 bytes (+1 int3 padding), `ret 4`.
// Ego_r.pdb: ?SetExperienceStatus@CTCHeroExperience@@QAEXABV?$vector@JV?$allocator@J@std@@@std@@@Z
// Callees: CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) @ 0x006D8170 (stats 0..9, false),
// CTCHeroExperience::OnStatLevelChange(bool) @ 0x006D83B0 (false).
// Shape: status[0]/[1] are the totals; one running index (starting at 2, stepped after `++i` in each for header) feeds both copy loops (VC turns it into `+8` and a
// scaled byte offset); STLport `*(begin() + n)` access with the sizes re-read each pass.
// Layout: retail TotalExperiencePoints +0x10, ExperiencePointsAvailableToSpend +0x14, StatExperiencePoints +0x18,
// TrainableStatLevels +0x24 (12-byte vectors).
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NONE };
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CThing;
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*             ParentThing;            // +0x04
    unsigned char       _base08[0x4];
    long                FishingLevel;           // +0x0c
    long                TotalExperiencePoints;  // +0x10
    long                ExperiencePointsAvailableToSpend;   // +0x14
    CRetailVector<long> StatExperiencePoints;   // +0x18
    CRetailVector<long> TrainableStatLevels;    // +0x24
    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool b);   // 0x006D8170
    void OnStatLevelChange(bool b);                                     // 0x006D83B0
    void SetExperienceStatus(const CRetailVector<long>& status);
};

void CTCHeroExperience::SetExperienceStatus(const CRetailVector<long>& status)
{
    TotalExperiencePoints = status[0];
    ExperiencePointsAvailableToSpend = status[1];
    unsigned int index = 2;
    for (unsigned int i = 0; i < StatExperiencePoints.size(); ++i, ++index)
        StatExperiencePoints[i] = status[index];
    for (unsigned int i = 0; i < TrainableStatLevels.size(); ++i, ++index)
        TrainableStatLevels[i] = status[index];
    for (long stat = 0; stat < 10; ++stat)
        UpdateAbilitiesForStat((EHeroTrainableStatType)stat, false);
    OnStatLevelChange(false);
}
