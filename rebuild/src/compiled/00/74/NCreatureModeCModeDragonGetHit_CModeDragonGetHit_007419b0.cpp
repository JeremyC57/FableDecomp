// NCreatureMode::CModeDragonGetHit::CModeDragonGetHit(CThing*) -- CModeStandBase(thing), own vtable, the "GET_HIT" animation-group filter, a null target pointer, Timer = 0
// Leaf: ??0CModeDragonGetHit
// Retail: Fable.exe (TLC, VC7.1) @ 0x007419B0, 135 bytes (`ret 4`). Vtable 0x012683E4 = NCreatureMode::CModeDragonGetHit.
// Callees: CModeStandBase::CModeStandBase(CThing*) @ 0x00861E00 (manifest "CTCCreatureNavigation" is propagated),
// CCharString @ 0x0099EBF0 / 0x0099E4B0 / operator= 0x0099EFB0 / ~ 0x0099EAE0, CRC @ 0x004014A0,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Layout (retail, PDB offsets -4 nowhere here): base 0x68, Filter +0x68 (CAnimFilterOfGroup, vtable 0x01267568),
// PTarget +0x74 (CIntelligentPointer<CThing>, vtable 0x01238C6C), Timer +0x7c.
// Shape: the filter is built from an implicit CCharString conversion of the literal (the temporary is re-addressed
// with lea and destroyed after the filter); the intelligent pointer's base ctor then its vtable store; Timer last.
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
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);
    }
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

struct CModeDragonGetHit : CModeStandBase {   // vtable 0x012683E4
    CAnimFilterOfGroup  Filter;            // +0x68
    CIntelligentPointer<CThing> PTarget;           // +0x74
    long                Timer;             // +0x7c
    virtual void v00();
    CModeDragonGetHit(CThing* thing);
};

CModeDragonGetHit::CModeDragonGetHit(CThing* thing)
    : CModeStandBase(thing)
    , Filter("GET_HIT")
    , Timer(0)
{
}

}