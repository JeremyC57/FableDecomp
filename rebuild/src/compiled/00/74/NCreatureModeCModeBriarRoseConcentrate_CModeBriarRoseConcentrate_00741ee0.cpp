// NCreatureMode::CModeBriarRoseConcentrate::CModeBriarRoseConcentrate(CThing*) -- mode ctor: "POWER_UP" group filter, Timer 0, empty target / imposter list / def / two effect pointers
// Leaf: ??0CModeBriarRoseConcentrate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741EE0, 208 bytes (`ret 4`). Vtable 0x01268548 (NCreatureMode::CModeBriarRoseConcentrate).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Layout (PDB CModeBriarRoseConcentrate, 0xA8; retail 0xA4 because the STLport vector is 12 bytes, not the PDB's 16):
// Filter +0x68 (CAnimFilterOfGroup, vtable 0x01267568), Timer +0x74, PTarget +0x78
// (CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8), Imposters +0x80 (vector<CIntelligentPointer<CThing>>),
// Imposter +0x8c (bool), PDef +0x90 (CDefPointer<const CBriarRoseDef>), PBodyEffect +0x94, PSwordEffect +0x9c
// (CIntelligentPointer<CThing>, vtable 0x01238C6C).
// Shape: the filter is built from an implicit CCharString conversion temporary of the literal; Timer is a
// plain initialiser (stored before the PTarget base ctor); the vector is the STLport _Vector_base +
// _STLP_alloc_proxy model (three zero words from the shared ebx); Imposter / PDef interleave with the next
// member's `lea`.
class CThing;
class CThingCreatureBase;
class CBriarRoseDef;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
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
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

struct CBaseIntelligentPointer {           // vptr + item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};

template <class T> struct CAllocator {     // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {    // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};

template <class T> class CDefPointer { public:    // 4 bytes: the def object
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

class CModeBriarRoseConcentrate : public CModeStandBase { public:   // vtable 0x01268548
    CAnimFilterOfGroup                  Filter;       // +0x68
    long                                Timer;        // +0x74
    CIntelligentPointerCreature         PTarget;      // +0x78
    CVector<CIntelligentPointerThing>   Imposters;    // +0x80
    bool                                Imposter;     // +0x8c
    CDefPointer<const CBriarRoseDef>    PDef;         // +0x90
    CIntelligentPointerThing            PBodyEffect;  // +0x94
    CIntelligentPointerThing            PSwordEffect; // +0x9c
    virtual void v00();
    CModeBriarRoseConcentrate(CThing* thing);
};

CModeBriarRoseConcentrate::CModeBriarRoseConcentrate(CThing* thing)
    : CModeStandBase(thing)
    , Filter("POWER_UP")                // literal @ 0x01268578 (masked)
    , Timer(0)
    , Imposter(false)
{
}

}