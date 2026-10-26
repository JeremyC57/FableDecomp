// NCreatureMode::CModeSleepBed::CModeSleepBed(CThing*) -- CModeBase(thing), own vtable, the "SLEEP_BED" filter, SleepAnimName copied from the default sleep-animation global, a null remove-action pointer
// Leaf: ??0CModeSleepBed
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7CD0 (`ret 4`). Vtable 0x01275760 = NCreatureMode::CModeSleepBed.
// Callees: CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CCharString(const char*, long) @ 0x0099EBF0, CCharString() @ 0x0099E4B0,
// operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, CRC @ 0x004014A0, CCharString(const CCharString&) @ 0x0099EC30.
// Global: the default sleep animation name (a CCharString) @ 0x013BAFD4.
// Layout: Ego_r.pdb CModeSleepBed (0x34): Filter +0x1c, SleepAnimName +0x28, PRemoveAction +0x2c (as landed 0x0084B7C0,
// the three-argument overload). Shape: one-argument overload of that constructor; the counted pointer's inline ctor
// zeroes both words through the register VC already zeroed for the filter's null test.
class CThing;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (default argument: the literal converts implicitly)
    CCharString(const CCharString& other); // 0x0099EC30
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);   // one shared null test (CSE of the two guards)
    }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, sizeof 0x1c (vtable 0x012673CC)
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing* thing);              // 0x0073EAC0
};

class CCreatureActionBase;
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

extern CCharString g_defaultSleepAnimName;   // 0x013BAFD4

namespace NCreatureMode {

class CModeSleepBed : public CModeBase { public:         // vtable 0x01275760
    CAnimFilterOfGroup                    Filter;          // +0x1c
    CCharString                           SleepAnimName;   // +0x28
    CCountedPointer<CCreatureActionBase>  PRemoveAction;   // +0x2c
    virtual void v00();
    CModeSleepBed(CThing* thing);
};

CModeSleepBed::CModeSleepBed(CThing* thing)
    : CModeBase(thing)
    , Filter("SLEEP_BED")
    , SleepAnimName(g_defaultSleepAnimName)
{
}

}