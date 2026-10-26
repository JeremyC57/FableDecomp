// CTCHeroStats::GetTradeSkill() const -- the trade-skill factor for the hero's trainable stat 5 level (the table's last entry past its end, 0 without experience)
// Retry 10b: the stats def comes from an inline const getter (`GetDefinitionManager()->GetHeroStatsDef()`, B-4/B-5 accessor idiom), which gives retail's interface select (`mov eax,esi ; mov ecx,[eax+4]`); the plain member read selects into esi.
// Leaf: GetTradeSkill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057C239, 173 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: std::bitset::test @ 0x00410DE0 (thing interface presence, +0x20), the CONST
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (+0x44), CTCHeroExperience::
//          GetTrainableStatLevel(long) @ 0x006D6520 (reads TrainableStatLevels[stat], retail +0x24; manifest label
//          "CTCInventoryAbilities::GetAbilityLevel" is propagated), CTCBase::GetDefinitionManager() @ 0x006869D0
//          (+0xDC its CHeroStatsDef).
// Shape: the parent thing is reached as `const CThing*` (a const method), so the HasTC + GetTC lookup is the
// const template over the const LowerBound (the component lands straight in ecx); the experience component (interface 0x68) is checked and the absent
// arm shares the `return 0.0f` tail (0x0122DEDC is 0.0f). The def table (+0xF0, a 12-byte float vector) is read
// through the def pointer in eax with three separate size() evaluations: `level < size()`, the `size() != 0`
// guard and `size() - 1` in the clamp.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) const { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel;
        if (it == map->last)
            sel = map->last;
        else if (it->type <= wanted)
            sel = it;
        else
            sel = map->last;
        return (T*)sel->component;
    }
};

struct CFloatVector {                                // retail std::vector<float>: first, last, end
    float* m_pFirst;
    float* m_pLast;
    float* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const float& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0xF0];
    CFloatVector  TradeSkillForGuileLevel;                 // +0xF0
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC
    const CHeroStatsDef* GetHeroStatsDef() const { return PHeroStatsDef; }
};

class CTCHeroExperience { public:
    unsigned long GetTrainableStatLevel(long stat) const;  // 0x006D6520
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCHeroStats : public CTCBase { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase

    float GetTradeSkill() const;
};

float CTCHeroStats::GetTradeSkill() const
{
    const CThing* thing = ParentThing;
    if (thing->HasTC(0x68)) {
        unsigned long level = thing->GetTC<const CTCHeroExperience>(0x68)->GetTrainableStatLevel(5);
        const CHeroStatsDef* def = GetDefinitionManager().GetHeroStatsDef();
        if (level < def->TradeSkillForGuileLevel.size())
            return def->TradeSkillForGuileLevel[level];
        if (def->TradeSkillForGuileLevel.size() > 0)
            return def->TradeSkillForGuileLevel[def->TradeSkillForGuileLevel.size() - 1];
    }
    return 0.0f;
}