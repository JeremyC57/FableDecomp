// NCreatureMode::CModeDrained::CModeDrained(CThing*, CThing*) -- CModeStandBase(thing), the "DRAINED" group filter, PDrainer = drainer, DrainerMode = the type of the drainer's current creature mode
// Leaf: ??0CModeDrained
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868220, 214 bytes (`ret 8`). Vtable 0x01268234 = NCreatureMode::CModeDrained.
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90;
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020; CTCCreatureModeManager::GetCurrentMode 0x0073A6E0
// (manifest label "CPixelShader::CountReferences" is propagated); CModeBase::GetModeType is vtable slot 1.
// Layout (retail, as the landed one-argument overload 0x007413C0): Filter +0x68, PDrainer +0x74, DrainerMode +0x7c.
// CThing: interface bitset +0x20 (type 0x31 = CTCCreatureModeManager, bit 17 of the second word), interface map +0x44.
// Shape: the inline out-parameter GetInterface (the not-found arm leaves the out pointer untouched, so retail
// reads the shared key/out frame slot); the mode type comes from a virtual call on GetCurrentMode().
struct CThing;
struct CThingCreatureBase;
struct CBaseObject;

struct CCharStringData {
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
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

struct C3DVector { float X, Y, Z; };

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

template <class T> struct CDefPointer {    // 4 bytes: the def object (add-ref'd by its owner)
    T* Object;
    CDefPointer() : Object(0) { }
};

class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
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

struct CModeBase {
    virtual void v00();
    virtual unsigned long GetModeType();   // slot 1
};

struct CTCCreatureModeManager {            // interface type 0x31
    CModeBase* GetCurrentMode();           // 0x0073A6E0
};

namespace NCreatureMode {

struct CModeDrained : CModeStandBase {     // vtable 0x01268234
    CAnimFilterOfGroup        Filter;      // +0x68
    CIntelligentPointerThing  PDrainer;    // +0x74
    unsigned long             DrainerMode; // +0x7c
    virtual void v00();
    CModeDrained(CThing* thing, CThing* drainer);
};

CModeDrained::CModeDrained(CThing* thing, CThing* drainer)
    : CModeStandBase(thing)
    , Filter("DRAINED")
    , PDrainer(drainer)
{
    CTCCreatureModeManager* modes;
    drainer->GetInterface(&modes, 0x31);
    DrainerMode = modes->GetCurrentMode()->GetModeType();
}

}
