// CTCHeroOpinionDeedLog::RemoveCrimeFromVillage(long handle) -- when the crime village is alive, has its village component and is not flagged, withdraw the crime from the village and clear the logged deed's reported flag
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model (landed 0x00584157 idiom) instead of the GetInterface out-pointer: the component loads straight into ecx
// Leaf: RemoveCrimeFromVillage
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5F80, length 128 bytes (+16 int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inlined interface lookup, interface 0x22 = bit 2 of CThing +0x24), CTCVillage::RemoveCrime(long) @ 0x0065C146, STLport
// map<long, COpinionDeed*>::find(const long&) @ 0x006C77E0 (iterator by hidden pointer).
// Shape: null test of the village pointer, then the GetInterface bit test on a second GetPItem(); find() takes the address of the
// `handle` argument and its result is a named iterator (re-read from the frame for the header compare).
// Layout: retail OpinionDeedMap +0x0c, VillageForCrimes +0x20; CTCVillage flag byte +0x7c (as in CTCGuard::ReportCrime 0x007B2C70);
// COpinionDeed reported-to-village flag +0x22 (descriptive).
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCVillage {
public:
    unsigned char _pad00[0x7c];
    bool CrimeReportedFlag;                     // +0x7c
    void RemoveCrime(long handle);              // 0x0065C146
};
class COpinionDeed { public:
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
    if (VillageForCrimes.Get()) {
        CThing* villageThing = VillageForCrimes.Get();
        if (villageThing->HasTC(TCI_VILLAGE)) {
            CTCVillage* village = ((CTCVillage*)&villageThing->GetTC(TCI_VILLAGE));
            if (!village->CrimeReportedFlag) {
                village->RemoveCrime(handle);
                CDeedMapIterator it = OpinionDeedMap.find(handle);
                if (it._M_node != OpinionDeedMap._M_header)
                    it->second->ReportedToVillage = false;
            }
        }
    }
}