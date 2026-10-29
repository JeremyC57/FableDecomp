// COpinionReactionManager::LogReaction(EOpinionReactionType reaction, const CThing* creature, bool fromScript) -- tell the reaction's normal and high frequency traits (when the def has them) that the creature reacted; a non-script reaction also stamps the creature's opinion-of-hero component with the current world frame
// Leaf: LogReaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700940, length 141 bytes (padded to 144).
// Callees: CReactionFrequencyTraits virtual slot 1 (+4) LogReaction(const CThing*, bool) through
//          PDef->FrequencyTraits[reaction] / FrequencyTraitsHigh[reaction]; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//          @ 0x0040F020; CTCBase::GetWorld @ 0x00686A70 (result unused); CWorld::GetWorldFrame (static) @ 0x0049D870.
// Layout: PDef (CDefPointer word) +0; COpinionReactionManagerDef::FrequencyTraits / FrequencyTraitsHigh are retail
// +0x324 / +0x328 (PDB +0x354 / +0x358); CTCCreatureOpinionOfHero (TC type 0x4f = bit 15 of presence word 2)
// LastOpinionReactionFrame +0xc.
// Shape: the opinion-of-hero lookup is the unchecked accessor (`CTC* tc; if (HasTC(k)) tc = GetTC(k); return tc;`):
// the absent arm reads the never-written local from the lookup key's frame word (the dead `reaction` slot);
// each traits pointer is a named local (tested and used from one load);
// `tc->GetWorld()->GetWorldFrame()` keeps the GetWorld call although GetWorldFrame is static.
class CThing;
class CWorld { public:
    static long __fastcall GetFrame();            // 0x0049D870
};
class CTCBase { public:
    virtual void v00();
    CWorld* GetWorld();                     // 0x00686A70
};
class CTCCreatureOpinionOfHero : public CTCBase { public:
    unsigned char _pad04[8];
    long LastOpinionReactionFrame;          // +0x0c
};
struct CInterfaceMap {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 bitset of the interface types present
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCCreatureOpinionOfHero* GetOpinionOfHeroTC()
    {
        CTCCreatureOpinionOfHero* tc;
        if (HasTC(0x4f))
            tc = (CTCCreatureOpinionOfHero*)GetTC(0x4f);
        return tc;
    }
};
class CReactionFrequencyTraits { public:
    virtual void v00();
    virtual void LogReaction(const CThing* creature, bool fromScript);   // slot 1
};
class COpinionReactionManagerDef { public:
    unsigned char              _pad000[0x324];
    CReactionFrequencyTraits** FrequencyTraits;         // +0x324 (PDB +0x354)
    CReactionFrequencyTraits** FrequencyTraitsHigh;     // +0x328 (PDB +0x358)
};

class COpinionReactionManager {
public:
    const COpinionReactionManagerDef* PDef;     // +0x00 CDefPointer<const COpinionReactionManagerDef>
    void LogReaction(long reaction, CThing* creature, bool fromScript);
};

void COpinionReactionManager::LogReaction(long reaction, CThing* creature, bool fromScript)
{
    CReactionFrequencyTraits* traits = PDef->FrequencyTraits[reaction];
    if (traits)
        traits->LogReaction(creature, fromScript);
    CReactionFrequencyTraits* highTraits = PDef->FrequencyTraitsHigh[reaction];
    if (highTraits)
        highTraits->LogReaction(creature, fromScript);
    if (!fromScript)
    {
        CTCCreatureOpinionOfHero* opinion = creature->GetOpinionOfHeroTC();
        opinion->LastOpinionReactionFrame = opinion->GetWorld()->GetFrame();
    }
}