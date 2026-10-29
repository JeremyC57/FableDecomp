// NCreatureMode::CModeSummonerSummon::CModeSummonerSummon(CThing*, CThing*) -- CModeStandBase(thing), the "SUMMON" group filter, PTarget = target, empty CreatePositions, Timer/NumCreated 0
// Leaf: ??0CModeSummonerSummon
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC0B0, 172 bytes (`ret 8`). Vtable 0x01282BA0 (NCreatureMode::CModeSummonerSummon).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90.
// Layout (PDB 0x94; retail 0x90 because the STLport vector is 12 bytes, not 16): Filter +0x68, PTarget +0x74,
// CreatePositions +0x7c (vector<C3DVector>), Timer +0x88, NumCreated +0x8c.
// Shape: the filter is built from an implicit CCharString conversion temporary of the literal; the target goes
// through the CIntelligentPointer<CThing>(CThing*) ctor (base ctor, vptr, SetPItem); the vector's three words and
// the two longs are five zero stores from the one register the CRC test zeroed.
class CThing;
class CThingCreatureBase;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
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

class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0: CRC

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

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
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

class C3DVector { public: float X, Y, Z; };

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

struct CModeSummonerSummon : CModeStandBase {   // vtable 0x01282BA0
    CAnimFilterOfGroup         Filter;           // +0x68
    CIntelligentPointerThing   PTarget;          // +0x74
    CVector<C3DVector>         CreatePositions;  // +0x7c
    long                       Timer;            // +0x88
    long                       NumCreated;       // +0x8c
    virtual void v00();
    CModeSummonerSummon(CThing* thing, CThing* target);
};

CModeSummonerSummon::CModeSummonerSummon(CThing* thing, CThing* target)
    : CModeStandBase(thing)
    , Filter("SUMMON")
    , PTarget(target)
    , Timer(0)
    , NumCreated(0)
{
}

}