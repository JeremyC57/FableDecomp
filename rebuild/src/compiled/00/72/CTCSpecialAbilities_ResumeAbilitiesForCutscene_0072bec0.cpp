// CTCSpecialAbilities::ResumeAbilitiesForCutscene() -- after a cutscene, resume each active spell component the hero has (interfaces 0x93, 0x91, 0x9A, 0x88)
// Leaf: ResumeAbilitiesForCutscene
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072BEC0, 245 bytes (plain `ret`).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (four inlined lookups), the components' ResumeForCutscene (manifest labels): CTCPhysicalShieldSpell 0x0079E520
//   (interface 0x93), CTCSummonSpell::ResumeForCutscene 0x0079A6A0 (interface 0x91, landed), CTCGhostSword 0x007C54F0 (0x9A),
//   CTCMultiArrow 0x007CC230 (interface 0x88).
// Layout: retail CThing interface bitset +0x20 (0x93 = word +0x30 bit 19, 0x91 = bit 17, 0x9A = bit 26, 0x88 = bit 8), map
//   +0x44; CTCSpecialAbilities ParentThing +0x04.
// Shape: four `if (HasTC) GetTC()->ResumeForCutscene();` statements, each re-reading ParentThing; the shared key word.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCPhysicalShieldSpell { void ResumeForCutscene(); };        // 0x0079E520 (interface 0x93)
struct CTCSummonSpell { void ResumeForCutscene(); };    // 0x0079A6A0 (interface 0x91)
struct CTCGhostSword { void ResumeForCutscene(); };        // 0x007C54F0 (interface 0x9A)
struct CTCMultiArrow { void ResumeForCutscene(); };        // 0x007CC230 (interface 0x88)
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void ResumeAbilitiesForCutscene();
};

void CTCSpecialAbilities::ResumeAbilitiesForCutscene()
{
    if (ParentThing->HasTC(0x93))
        ParentThing->GetTC<CTCPhysicalShieldSpell>(0x93)->ResumeForCutscene();
    if (ParentThing->HasTC(0x91))
        ParentThing->GetTC<CTCSummonSpell>(0x91)->ResumeForCutscene();
    if (ParentThing->HasTC(0x9a))
        ParentThing->GetTC<CTCGhostSword>(0x9a)->ResumeForCutscene();
    if (ParentThing->HasTC(0x88))
        ParentThing->GetTC<CTCMultiArrow>(0x88)->ResumeForCutscene();
}