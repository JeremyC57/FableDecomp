// CQuestManager::GetFullSectionNameForThing(const CCharString& sectionName, CThing* thing) -- the section name with the thing's day/night-only suffix ("%DayOnly" / "%NightOnly") appended when it has a CTCDayOrNightOnlySupport, else the name itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1450, length 152 bytes (padded to 160), `ret 0Ch` (hidden result + 2).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (component kind 0x7c), CCharString::
// CCharString(const char*, long) @ 0x0099EBF0 ("%" @ 0x01239434), CTCDayOrNightOnlySupport::GetSectionNameSuffix
// @ 0x00702AC0 (landed), operator+(const CCharString&, const CCharString&) @ 0x0099F570 (`__fastcall`: hidden result in
// ecx, left in edx, right on the stack), CCharString::~CCharString @ 0x0099EAE0 (x2), CCharString(const CCharString&)
// @ 0x0099EC30.
// Layout: the thing's component mask words at +0x20 (kind 0x7c = word +0x2c, bit 28), the component map at +0x44.
// Shape: the presence test is the inline masked HasTC, the lookup the __forceinline map find (its key temporary and the
// "%" conversion temporary both land in the thing argument's dead slot); `return name + tc->GetSectionNameSuffix("%");`
// destroys the suffix then the "%" temporary.
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation; key by address)
    __forceinline CTCBase_Retail* Find(long kind) const               // the inlined map `find`: end entry when absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class CTCDayOrNightOnlySupport { public:
    CCharString GetSectionNameSuffix(const CCharString& name) const;   // 0x00702AC0
};
class CQuestManager { public:
    CCharString GetFullSectionNameForThing(const CCharString& sectionName, CThing_Retail* thing);
};
CCharString CQuestManager::GetFullSectionNameForThing(const CCharString& sectionName, CThing_Retail* thing)
{
    if (thing->HasTC(0x7c)) {
        CTCDayOrNightOnlySupport* support = (CTCDayOrNightOnlySupport*)GetTC(thing, 0x7c);
        return sectionName + support->GetSectionNameSuffix("%");
    }
    return sectionName;
}