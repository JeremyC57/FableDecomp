// NCreatureMode::CModeSummonerLightningOrb::CModeSummonerLightningOrb(CThing*, CThing*) -- CModeStandBase(thing), the lightning-orb group filter, PTarget = target, two empty charge-effect pointers
// Leaf: ??0CModeSummonerLightningOrb
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC180, 175 bytes (`ret 8`). Vtable 0x01282BDC (NCreatureMode::CModeSummonerLightningOrb).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90.
// Layout (PDB 0x90): Filter +0x68, PTarget +0x74, PChargeEffect +0x7c, POrbChargeEffect +0x84
// (all CIntelligentPointer<CThing>, vtable 0x01238C6C), Timer +0x8c.
// Shape: Timer is not initialised by this ctor (no store at +0x8c); the three pointers are built in
// declaration order, only the first one through the CThing* ctor.
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

struct CModeSummonerLightningOrb : CModeStandBase {   // vtable 0x01282BDC
    CAnimFilterOfGroup         Filter;           // +0x68
    CIntelligentPointerThing   PTarget;          // +0x74
    CIntelligentPointerThing   PChargeEffect;    // +0x7c
    CIntelligentPointerThing   POrbChargeEffect; // +0x84
    long                       Timer;            // +0x8c
    virtual void v00();
    CModeSummonerLightningOrb(CThing* thing, CThing* target);
};

CModeSummonerLightningOrb::CModeSummonerLightningOrb(CThing* thing, CThing* target)
    : CModeStandBase(thing)
    , Filter("LIGHTNING_ORB")
    , PTarget(target)
{
}

}