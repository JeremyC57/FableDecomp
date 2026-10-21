// Grade: DIFFER(498v498) -- loop registers swapped (retail crime in edi and the L"\n" literal in esi; ours esi/edi) and the time-left text temporary in the dead region slot [ebp+8] where retail uses the dead fine slot [ebp+10h]
// CTCHeroStats::GetCrimeReport(const CCharString& region, CWideString& report, bool includeFine) -- append one line per
// outstanding crime type in `region` (the crime's name, its count and the deed's fine) and, when asked, the total fine
// and the minutes left before the crimes are forgotten
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057DC22, 498 bytes, `ret 0Ch`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
};
extern CCharString g_CrimeNameSymbols[10];                  // 0x013B8B10
extern CCharString g_CrimeFineSymbol;                       // 0x013B8B38
extern CCharString g_CrimeTimeLeftSymbol;                   // 0x013B8B3C
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30 (wchar_t is a typedef here)
bool __fastcall ConstructString(CWideString* text, long value);              // 0x009C7B70
bool __fastcall ConstructString(CWideString* text, long value0, long value1);  // 0x009C7B80
float __cdecl CeilFloat(float value);                       // 0x0057A2D0 (math.h inline float ceil(float), out of line)

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol); // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                    // +0x14
};
extern CMainGameComponent* g_MainGameComponent;             // 0x013B86A0

struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool HasTC(long type) { return present.test(type); }
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

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x74 - 0x08];
    long CrimeFineAmount;                                   // +0x74 (PDB +0x88)
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
struct CTCHeroOpinionDeedLog {
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

struct MirroredCriminalStatus {
    long  Crimes[10];                                       // +0x00
};
struct CCrimeStatusMap {                                    // std::map<CCharString, MirroredCriminalStatus>
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    MirroredCriminalStatus& operator[](const CCharString& key);  // 0x005812DF
};

struct CTCHeroStats {
    void*           __vftable;                              // +0x00
    CThing*         ParentThing;                            // +0x04 CTCBase
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;                      // +0x0C
    void UpdateOutstandingCrimes(const CCharString& region);   // 0x0057D9FD
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);   // 0x0057DAF5
    void GetCrimeReport(const CCharString& region, CWideString& report, bool includeFine);
};

void CTCHeroStats::GetCrimeReport(const CCharString& region, CWideString& report, bool includeFine)
{
    UpdateOutstandingCrimes(region);
    CTCHeroOpinionDeedLog* deedLog = 0;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)parent->GetTC(0x51);
    CWideString text;
    for (long crime = 0; crime < 10; ++crime) {
        if (OutstandingCrimes[region].Crimes[crime] != 0) {
            CDataBank* bank = g_MainGameComponent->TextBank;
            text = bank->GetTextBySymbol(g_CrimeNameSymbols[crime]);
            ConstructString(&text, OutstandingCrimes[region].Crimes[crime], deedLog->GetDeedEffectsDef(crime)->CrimeFineAmount);
            report += text + L"\n";
        }
    }
    if (includeFine) {
        long fine;
        long framesLeft;
        GetOutstandingFineAndFramesLeft(region, fine, framesLeft);
        CDataBank* bank = g_MainGameComponent->TextBank;
        text = bank->GetTextBySymbol(g_CrimeFineSymbol);
        ConstructString(&text, fine);
        report += text + L"\n";
        if (fine != 0) {
            CDataBank* timeBank = g_MainGameComponent->TextBank;
            text = timeBank->GetTextBySymbol(g_CrimeTimeLeftSymbol);
            ConstructString(&text, (long)CeilFloat((float)framesLeft * (1.0f / 900.0f)));
            report += text + L"\n";
        }
    }
}
