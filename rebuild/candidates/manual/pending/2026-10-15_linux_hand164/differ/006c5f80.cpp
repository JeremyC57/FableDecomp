// CTCHeroOpinionDeedLog::RemoveCrimeFromVillage(long handle) -- when the crime village is alive, has its village component and is not flagged, withdraw the crime from the village and clear the logged deed's reported flag
// Grade: DIFFER(132v130) -- lane gap: the looked-up village component used as `this` is loaded straight into ecx by retail (`mov ecx,[eax+4] ; mov al,[ecx+7Ch]` then RemoveCrime on ecx); ours goes through eax and copies, which reshuffles the find() argument setup (tried HasTC + a check-free GetTC accessor)
// Leaf: RemoveCrimeFromVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5F80, length 128 bytes (+16 int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inlined interface lookup, interface 0x22 = bit 2 of CThing +0x24), CTCVillage::RemoveCrime(long) @ 0x0065C146, STLport
// map<long, COpinionDeed*>::find(const long&) @ 0x006C77E0 (iterator by hidden pointer).
// Shape: null test of the village pointer, then the GetInterface bit test on a second GetPItem(); find() takes the address of the
// `handle` argument and its result is a named iterator (re-read from the frame for the header compare).
// Layout: retail OpinionDeedMap +0x0c, VillageForCrimes +0x20; CTCVillage flag byte +0x7c (as in CTCGuard::ReportCrime 0x007B2C70);
// COpinionDeed reported-to-village flag +0x22 (descriptive).
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
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
class CTCVillage {
public:
    unsigned char _pad00[0x7c];
    bool CrimeReportedFlag;                     // +0x7c
    void RemoveCrime(long handle);              // 0x0065C146
};
struct COpinionDeed {
    unsigned char _pad00[0x22];
    bool          ReportedToVillage;            // +0x22
};
struct SDeedNode {
    long       _M_color;
    SDeedNode* _M_parent;
    SDeedNode* _M_left;
    SDeedNode* _M_right;
    long          first;                        // +0x10
    COpinionDeed* second;                       // +0x14
};
struct CDeedMapIterator {                       // STLport _Rb_tree_iterator (returned by hidden pointer)
    SDeedNode* _M_node;
    CDeedMapIterator(SDeedNode* n) : _M_node(n) {}
    SDeedNode* operator->() const { return _M_node; }
};
struct COpinionDeedMap {                        // STLport map<long, COpinionDeed*> (12 bytes)
    SDeedNode*   _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    CDeedMapIterator find(const long& key);     // 0x006C77E0
};
class CTCBaseRoot {
public:
    virtual ~CTCBaseRoot();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroOpinionDeedLog : public CTCBaseRoot {
public:
    COpinionDeedMap OpinionDeedMap;             // +0x0c
    void*           OpinionDeedsSearchingForWitnesses;   // +0x18
    long            NextHandle;                 // +0x1c
    CIntelligentPointer<CThing> VillageForCrimes;        // +0x20
    void RemoveCrimeFromVillage(long handle);
};

void CTCHeroOpinionDeedLog::RemoveCrimeFromVillage(long handle)
{
    if (VillageForCrimes.GetPItem()) {
        CTCVillage* village;
        if (VillageForCrimes.GetPItem()->GetInterface(&village, 0x22)) {
            if (!village->CrimeReportedFlag) {
                village->RemoveCrime(handle);
                CDeedMapIterator it = OpinionDeedMap.find(handle);
                if (it._M_node != OpinionDeedMap._M_header)
                    it->second->ReportedToVillage = false;
            }
        }
    }
}
