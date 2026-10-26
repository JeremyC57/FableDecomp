// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing* thing) -- mode ctor: null target, "ICICLE_ASSUALT" group filter, null def, two empty vectors
// Leaf: ??0CModeIceTrollGroundSmashIcicleAssault
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743BE0, 170 bytes (`ret 4`). Vtable 0x01268774 (NCreatureMode::CModeIceTrollGroundSmashIcicleAssault).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10;
// CCharString(const char*, long) 0x0099EBF0, CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0.
// Layout (PDB 0xA4; retail 0x9C, the two STLport vectors are 12 bytes): PTarget +0x68 (CIntelligentPointer<CThing>,
// vtable 0x01238C6C), Filter +0x70 (CAnimFilterOfGroup, vtable 0x01267568), Timer +0x7c, PDef +0x80
// (CDefPointer<const CSnowTrollDef>), Icicles +0x84 (vector<CIcicleData>), ViablePoints +0x90 (vector<C3DVector>).
// Shape: init list in declaration order; Timer is left uninitialised (no store to +0x7c); the def pointer and
// both _Vector_base + _STLP_alloc_proxy vectors share the zero register edi (seven zero words).
class CThing;
class CThingCreatureBase;
class CSnowTrollDef;
class C3DVector { public: float X, Y, Z; };
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
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
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

struct CModeIceTrollGroundSmashIcicleAssault : CModeStandBase {   // vtable 0x01268774
    class CIcicleData { public: unsigned char _data[0x24]; };   // PEffect, PTrap (intelligent pointers), EffectTimer, TrapTimer, Position
    CIntelligentPointer<CThing>          PTarget;       // +0x68
    CAnimFilterOfGroup                Filter;        // +0x70
    long                              Timer;         // +0x7c
    CDefPointer<const CSnowTrollDef>  PDef;          // +0x80
    CVector<CIcicleData>              Icicles;       // +0x84
    CVector<C3DVector>                ViablePoints;  // +0x90
    virtual void v00();
    CModeIceTrollGroundSmashIcicleAssault(CThing* thing);
};

CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing* thing)
    : CModeStandBase(thing)
    , Filter("ICICLE_ASSUALT")          // literal @ 0x01268760 (masked; retail spelling)
{
}

}