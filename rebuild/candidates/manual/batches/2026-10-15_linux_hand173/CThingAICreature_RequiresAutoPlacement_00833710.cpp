// CThingAICreature::RequiresAutoPlacement() -- a villager whose village's def asks for auto placement
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833710, 147 bytes.
// Callees: CInterfaceMap::LowerBound @ 0x0040F020, CTCVillageMember::GetPVillage @ 0x007EE540 (label
//   "IS_THING_ALIVE<...>" propagated; landed callers 0x007B2CF0 / 0x00849250 name it).
// Layout: retail CThing interface bitset +0x20 (TC 0x23 = CTCVillageMember: word +0x24 bit 3), CVectorMap +0x44;
//   retail CTCVillage keeps its def pointer (CDefPointer, object word) at +0x94 (the PDB's 12-byte lists are 4 bytes in
//   retail, so the PDB offsets do not apply); the def's flag byte read here is at +0xe4 (name unverified); the def ref
//   count is at +4 with virtual Destroy in slot 1.
// Shape: `village && village->GetDef()->Flag` with GetDef returning a CDefPointer BY VALUE (copy ctor `inc [ecx+4]`):
//   the conditionally built temporary gets a construction flag (zeroed at entry, set to 1 on the arm that builds it)
//   tested before its release; the && result is a bool local tested after the release.
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CTCVillageDef : CDefClassBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool RequiresAutoPlacement;                 // +0xe4 (name unverified)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++((CDefClassBase*)Object)->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CTCVillage {
    unsigned char _pad[0x94];
    CDefPointer<const CTCVillageDef> PDef;      // +0x94
    CDefPointer<const CTCVillageDef> GetDef() const { return PDef; }
};
struct CTCVillageMember {
    CTCVillage* GetPVillage() const;            // 0x007EE540
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingAICreature : CThing {
    bool RequiresAutoPlacement();
};

bool CThingAICreature::RequiresAutoPlacement()
{
    if (HasTC(0x23)) {
        CTCVillage* village = GetTC<CTCVillageMember>(0x23)->GetPVillage();
        bool requires = village && village->GetDef()->RequiresAutoPlacement;
        if (requires)
            return true;
    }
    return false;
}
