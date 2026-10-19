// NCreatureMode::CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing*) -- CModeStandBase(thing), the "VULNERABLE" group filter, empty target, obstruction list and eye / sting emitters, PCreature = the owner
// Leaf: ??0CModeScorpionKingVulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8120, 201 bytes (`ret 4`). Vtable 0x0128582C (NCreatureMode::CModeScorpionKingVulnerable).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Layout (PDB 0xBC; retail 4 bytes smaller from the 12-byte STLport vector): RecoveryCount +0x68, Filter +0x6c,
// PCreature +0x78, bRecovering +0x7c, PTarget +0x7d, Obstructions +0x88, PEyeEmitter +0x94, PStingEmitter +0x9c,
// FrameCount +0xa4, EffectsDone +0xa8, TailEffectIndex +0xac, CollisionRadius +0xb0, TailCurlFinished +0xb4.
// The intelligent pointers are byte-aligned (their classes are declared under pack(1) in this TU, as the
// odd +0x7d offset shows) while the vector keeps its 4-byte alignment (+0x88).
// Shape: PCreature is the ctor-body assignment stored last from ebp; the vector's zero words are immediates
// (the CRC null test used `test ecx,ecx`, no zero register).
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

#pragma pack(push, 1)
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

#pragma pack(pop)

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

namespace NCreatureMode {

struct CModeScorpionKingVulnerable : CModeStandBase {   // vtable 0x0128582C
    long                               RecoveryCount;     // +0x68
    CAnimFilterOfGroup                 Filter;            // +0x6c
    CThingCreatureBase*                PCreature;         // +0x78
    bool                               bRecovering;       // +0x7c
    CIntelligentPointerCreature        PTarget;           // +0x7d
    CVector<CIntelligentPointerThing>  Obstructions;      // +0x88
    CIntelligentPointerThing           PEyeEmitter;       // +0x94
    CIntelligentPointerThing           PStingEmitter;     // +0x9c
    long                               FrameCount;        // +0xa4
    bool                               EffectsDone;       // +0xa8
    long                               TailEffectIndex;   // +0xac
    float                              CollisionRadius;   // +0xb0
    bool                               TailCurlFinished;  // +0xb4
    virtual void v00();
    CModeScorpionKingVulnerable(CThing* thing);
};

CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing* thing)
    : CModeStandBase(thing)
    , Filter("VULNERABLE")
{
    PCreature = (CThingCreatureBase*)thing;
}

}
