// CTCGuard::ReportCrime(CTCVillage& village, long crime, EOpinionDeedType deedType, bool flagVillage) -- report the crime to the village (optionally flagging it), and make the village thing's opinion-of-hero component aware of the deed when it does not already know of it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2C70, length 114 bytes (+14 int3 padding), `ret 10h`.
// Ego_r.pdb: ?ReportCrime@CTCGuard@@AAEXAAVCTCVillage@@JW4EOpinionDeedType@@_N@Z
// Callees: CTCVillage::ReportCrime(long) @ 0x0065CB6B, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
// interface lookup), CTCCreatureOpinionOfHero::KnowsOfDeed(long, EOpinionDeedType, bool) @ 0x00681AE0 and
// InformOfDeed(long, bool) @ 0x00683590.
// Shape: the opinion component is interface 0x4F (bit 15 of CThing +0x28: `test ch,ch ; jns`); the lookup key lives in the
// dead `flagVillage` argument slot. The flag byte is CTCVillage +0x7C (descriptive name).
// `this` is never read: the opinion component is looked up on the VILLAGE's parent thing.
// Layout: CTCBase ParentThing +0x04; CThing component bitset +0x20, interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE };
class CTCCreatureOpinionOfHero {
public:
    bool KnowsOfDeed(long crime, EOpinionDeedType deedType, bool b);   // 0x00681AE0
    void InformOfDeed(long crime, bool b);                              // 0x00683590
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
};
class CTCVillage : public CTCBase {
public:
    unsigned char _pad08[0x7c - 0x08];
    bool CrimeReportedFlag;                     // +0x7c
    void ReportCrime(long crime);               // 0x0065CB6B
};
class CTCGuard : public CTCBase {
public:
    void ReportCrime(CTCVillage& village, long crime, EOpinionDeedType deedType, bool flagVillage);
};

void CTCGuard::ReportCrime(CTCVillage& village, long crime, EOpinionDeedType deedType, bool flagVillage)
{
    village.ReportCrime(crime);
    if (flagVillage)
        village.CrimeReportedFlag = true;
    CTCCreatureOpinionOfHero* opinion;
    if (village.ParentThing->GetInterface(&opinion, 0x4f)) {
        if (!opinion->KnowsOfDeed(crime, deedType, true))
            opinion->InformOfDeed(crime, false);
    }
}
