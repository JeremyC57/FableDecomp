// NCreatureMode::CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge(CThing*) -- CModeStandBase(thing), the "TAIL_PLUNGE" group filter, empty target / tail / dust-trail pointers, PCreature = the owner
// Leaf: ??0CModeScorpionKingTailPlunge
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D81F0, 180 bytes (`ret 4`). Vtable 0x01285860 = NCreatureMode::CModeScorpionKingTailPlunge
// (rtti-vtables.tsv; the manifest's "CModeJackOfBladesFlash" label is propagated from the 0x8D7E60 twin).
// Callees: CModeStandBase::CModeStandBase(CThing*) 0x00861E00; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Layout (PDB 0xBC, byte-packed: the class is compiled under pack(1), so the pointers sit at odd offsets
// +0xa1/+0xa9/+0xb1 exactly as the retail stores show): RecoveryCount +0x68, DustEffectCounter +0x6c, SpikeTime +0x70,
// SpikeDecisionTime +0x74, Filter +0x78, PCreature +0x84, bRecovering +0x88, TargetPos +0x89, TailPos +0x95,
// PTarget +0xa1 (CIntelligentPointer<CThingCreatureBase>), PTailEmitter +0xa9, PDustTrailEmitter +0xb1.
// Shape: as the Jack-of-Blades flash mode: PCreature is the ctor-body assignment stored last from ebx.
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

template <class T> class CDefPointer { public:    // 4 bytes: the def object (add-ref'd by its owner)
    T* Object;
    CDefPointer() : Object(0) { }
};

namespace NCreatureMode {

#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeStandBase { public:   // vtable 0x01285860
    long                         RecoveryCount;      // +0x68
    long                         DustEffectCounter;  // +0x6c
    long                         SpikeTime;          // +0x70
    long                         SpikeDecisionTime;  // +0x74
    CAnimFilterOfGroup           Filter;             // +0x78
    CThingCreatureBase*          PCreature;          // +0x84
    bool                         bRecovering;        // +0x88
    C3DVector                    TargetPos;          // +0x89
    C3DVector                    TailPos;            // +0x95
    CIntelligentPointerCreature  PTarget;            // +0xa1
    CIntelligentPointerThing     PTailEmitter;       // +0xa9
    CIntelligentPointerThing     PDustTrailEmitter;  // +0xb1
    virtual void v00();
    CModeScorpionKingTailPlunge(CThing* thing);
};
#pragma pack(pop)

CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge(CThing* thing)
    : CModeStandBase(thing)
    , Filter("TAIL_PLUNGE")
{
    PCreature = (CThingCreatureBase*)thing;
}

}