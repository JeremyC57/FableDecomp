// CCombatAction_GenericResponseToDecapitation::Initialise() -- decapitation reaction: set the animation, decapitate the creature, spin it, base Initialise, then push a knocked-down mode filtered on the animation's group
// Grade: DIFFER(481v481) -- register swap only: retail keeps this in ebx and the lookup temporaries in esi (ours the reverse), plus the scratch-register rotation that follows
// Leaf: ?Initialise@CCombatAction_GenericResponseToDecapitation@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867010, 481 bytes (496 with padding), plain `ret` (virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / CCharString() 0x0099E4B0 /
//   CCharString(const CCharString&) 0x0099EC30 / operator= 0x0099EFB0, CCombatActionBase::SetAnimationName 0x008574D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0xD1 =
//   CTCDecapitation and 0x31 = CTCCreatureModeManager), CTCDecapitation::Decapitate 0x007EF710, creature virtual slot 59
//   (+0xEC), CCombatActionBase::Initialise 0x00857520, the anim entry's extra-data lookup 0x00867F60 (type 0xB, out
//   pointer), CDefStringTable::GetString(long) 0x009D49B0 on the global table 0x013CA828, operator new 0x00BFEA1A,
//   NCreatureMode::CModeBase(CThing*) 0x0073EAC0, CRC 0x004014A0, CTCCreatureModeManager::AddMode 0x0073ACC0;
//   count-block destroy fn 0x007180B0.
// Data: "RESPONSE_GENERIC_REACT_TO_DECAPITATION" 0x012771A8; CModeKnockedDown vtables 0x01267E78 (+0) / 0x01267568 (+0x1C).
// Layout: retail CCombatActionBase (PCreature +0x08, PAnimEntry +0x74); CModeKnockedDown 0x28 bytes: CModeBase 0x1C,
//   anim-filter vptr +0x1C, Group +0x20, GroupCRC +0x24.
// Shape: the group name comes through an inline by-value helper (build, copy, destroy: no NRVO); the by-value counted
//   pointer is built in its argument slot (user copy ctor) and keeps its null test.
void __fastcall Destroy_007180b0(void* object);   // 0x007180B0
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

struct CDefStringTable {
    CCharString GetString(long id);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void SetSpin(float angle, bool immediate);   // slot 59 (+0xEC)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0xb0 - 0x4c];
    float         m_spin;                           // +0xB0
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
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct CTCDecapitation {
    void Decapitate();                              // 0x007EF710
};

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
};
template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};

namespace NCreatureMode {
struct CModeBase {                                  // retail 0x1C bytes
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing* creature);                    // 0x0073EAC0
};
struct __declspec(novtable) CAnimFilter {
    virtual void v00();
};
struct CModeKnockedDown : CModeBase, CAnimFilter {  // vtables 0x01267E78 / 0x01267568
    CCharString   Group;                            // +0x20
    unsigned long GroupCRC;                         // +0x24
    virtual void v00();
    __forceinline CModeKnockedDown(CThing* creature, const CCharString& group)
        : CModeBase(creature)
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};
}

struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<NCreatureMode::CModeBase> mode);   // 0x0073ACC0
};

struct CAnimGroupData {
    unsigned char _pad[8];
    long GroupNameID;                               // +8
};
struct CAnimationEntry {
    void GetExtraData(CAnimGroupData** out);        // 0x00867F60
};

class CCombatActionBase {
public:
    virtual void Initialise();                      // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;              // +0x08
    unsigned char           _pad10[0x74 - 0x10];
    CAnimationEntry*        PAnimEntry;             // +0x74
    void*                   PAnimEntryCount;        // +0x78
    unsigned char           _pad7c[0x114 - 0x7c];
    void SetAnimationName(const CCharString& name); // 0x008574D0
};

class CCombatAction_GenericResponseToDecapitation : public CCombatActionBase {
public:
    virtual void Initialise();
    static CCharString GetGroupName(long id)
    {
        CCharString name = g_DefStringTable.GetString(id);
        return name;
    }
};

void CCombatAction_GenericResponseToDecapitation::Initialise()
{
    {
        CCharString name("RESPONSE_GENERIC_REACT_TO_DECAPITATION");
        SetAnimationName(name);
    }
    CTCDecapitation* decapitation;
    if (PCreature.GetPItem()->GetInterface(&decapitation, 0xd1))
        decapitation->Decapitate();
    CThing* creature = PCreature.GetPItem();
    float spin = PCreature.GetPItem()->m_spin;
    creature->SetSpin(-spin, true);
    CCombatActionBase::Initialise();
    long id;
    {
        CAnimGroupData* data;
        PAnimEntry->GetExtraData(&data);
        id = data->GroupNameID;
    }
    CCharString group = GetGroupName(id);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->AddMode(CCountedPointer<NCreatureMode::CModeBase>(new NCreatureMode::CModeKnockedDown(PCreature.GetPItem(), group)));
}
