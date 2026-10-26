// COpinionReactionManager::ClearOpinionSource(CThing* thing, bool popOneDef) -- drop the thing's opinion-source component (or, when popOneDef, pop its newest source def and drop the component only once no def is left); a thing that was the exclusive opinion source stops being it
// Leaf: ClearOpinionSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007008C0, length 122 bytes (padded to 128).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCOpinionSource::PopSourceDef @ 0x00771510
//          (manifest "CTCCameraPointScriptedSpline::EditCycleNextKeyCamera" is wrong: it pops the def stack at +0xc/+0x10),
//          CThing::RemoveTC(long) @ 0x004C9840, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Layout: the exclusive opinion source is a CIntelligentPointer at +0x714 (as in the landed 0x00894E60 caller); the
// opinion source keeps its def stack as a vector {first +0xc, last +0x10}.
// Shape: HasTC test then an inline pointer-returning GetTC into a named local (`mov esi,[eax+4]`); the RemoveTC
// call is shared by the "not popping" and the "stack now empty" paths (`!popOneDef || (pop, empty)`).
class CTCBase;
class CTCOpinionSource { public:
    unsigned char _pad00[0xc];
    long* m_defStackFirst;                  // +0x0c
    long* m_defStackLast;                   // +0x10
    bool PopSourceDef();                    // 0x00771510
    bool HasNoSourceDefs() const { return m_defStackFirst == m_defStackLast; }
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
    void RemoveTC(long interfaceType);      // 0x004C9840
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +0x04
    void* GetPItem() const;                 // 0x00A01B50
    void SetPItem(void* item);              // 0x00A01B90
};

enum { TC_OPINION_SOURCE = 0x50 };

class COpinionReactionManager {
public:
    unsigned char           _pad000[0x714];
    CBaseIntelligentPointer ExclusiveOpinionSource;     // +0x714 CIntelligentPointer<CThing>
    void ClearOpinionSource(CThing* thing, bool popOneDef);
};

void COpinionReactionManager::ClearOpinionSource(CThing* thing, bool popOneDef)
{
    if (thing->HasTC(TC_OPINION_SOURCE))
    {
        CTCOpinionSource* source = (CTCOpinionSource*)thing->GetTC(TC_OPINION_SOURCE);
        if (!popOneDef || (source->PopSourceDef(), source->HasNoSourceDefs()))
            thing->RemoveTC(TC_OPINION_SOURCE);
    }
    if (thing == ExclusiveOpinionSource.GetPItem())
        ExclusiveOpinionSource.SetPItem(0);
}