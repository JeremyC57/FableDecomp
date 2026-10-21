// Grade: DIFFER(335v355) -- register allocation: retail spills this to the frame word, keeps the village in ebp and the 0x31 key constant in ebx inside the member loop (both lookup keys in the dead argument slot); ours keeps this in ebx, the village in the argument slot and the 0x31 key in the frame word
// CTCSoundAtmosphereVillage::GetGain(const CCharString&) -- the STREETLIFE layer's gain: the share of the village's living spaces filled by members who are not asleep (recounted every 5 updates), clamped to [0, 1]; 0 for a village with no living spaces, -1 for any other layer
// Leaf: GetGain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5150, 353 bytes (`ret 4`).
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0 ("STREETLIFE" 0x01271DE8; the null-data arm compares with the
//          strcmp intrinsic against "" 0x0122D70E); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (village 0x22,
//          creature mode manager 0x31); CBaseIntelligentPointer::GetPItem 0x00A01B50; CTCCreatureModeManager::GetCurrentMode
//          0x0073A6E0 (manifest "CPixelShader::CountReferences" is a byte twin), CMode::GetModeType (slot 1).
//          Floats: -1.0f 0x0122DEE0, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CTCSoundAtmosphereVillage (0x1C): TimeUntilNextUpdate +0x0C, PDef +0x10, NumLivingSpaces +0x14,
//          NumVillageMembers +0x18 (as the landed OnInitialActivate 0x007F50B0); CTCVillage members list (one sentinel pointer)
//          +0x2c; retail CThing interface bitset +0x20, map +0x44.
// Shape: the village and the member's mode manager are unset locals filled under HasTC (retail reads the dead argument
//          slot that also holds the lookup keys); the gain test is `< 0` then `> 1`.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries), as the landed
// OnInitialActivate 0x007F50B0: VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
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
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThingList {                             // std::list<CIntelligentPointer<CThing> >: one sentinel pointer
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    node* head;
};
struct CTCVillage {
    unsigned char _pad00[0x2c];
    CThingList VillageMembers;                  // +0x2c
};
struct CMode {
    virtual void v00();
    virtual long GetModeType() const;           // slot 1
};
struct CTCCreatureModeManager {                 // the interface-0x31 component
    CMode* GetCurrentMode();                    // 0x0073A6E0
};
struct CTCSoundAtmosphere {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    float TimeUntilNextUpdate;                  // +0x0c
    void* PDef;                                 // +0x10 CDefPointer<const CSoundAtmospheresDef>
};
struct CTCSoundAtmosphereVillage : CTCSoundAtmosphere {
    long NumLivingSpaces;                       // +0x14
    long NumVillageMembers;                     // +0x18
    __forceinline CTCVillage* GetVillage()
    {
        CTCVillage* village;
        if (ParentThing->HasTC(0x22))
            village = (CTCVillage*)ParentThing->GetTC(0x22);
        return village;
    }
    float GetGain(const CCharString& name);
};

float CTCSoundAtmosphereVillage::GetGain(const CCharString& name)
{
    if (name.PStringData == 0) {
        if (strcmp("STREETLIFE", "") != 0)
            return -1.0f;
    } else if (!(name.PStringData->Data == "STREETLIFE"))
        return -1.0f;
    if (NumLivingSpaces == 0)
        return 0.0f;
    if (TimeUntilNextUpdate <= 0.0f) {
        CTCVillage* village = GetVillage();
        NumVillageMembers = 0;
        CTCCreatureModeManager* modes;
        for (CThingList::node* n = village->VillageMembers.head->next; n != village->VillageMembers.head; n = n->next) {
            CThing* member = n->value.GetPItem();
            if (member->HasTC(0x31))
                modes = (CTCCreatureModeManager*)member->GetTC(0x31);
            if (modes->GetCurrentMode()->GetModeType() != 0xd)
                ++NumVillageMembers;
        }
        TimeUntilNextUpdate = 5.0f;
    }
    float gain = (float)NumVillageMembers / NumLivingSpaces;
    if (gain < 0.0f)
        return 0.0f;
    if (gain > 1.0f)
        return 1.0f;
    return gain;
}
