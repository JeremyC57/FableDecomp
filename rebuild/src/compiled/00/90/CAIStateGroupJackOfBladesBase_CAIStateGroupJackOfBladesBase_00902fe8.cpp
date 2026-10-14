#pragma optimize("s", on)      // size TU: the shared CIntelligentPointer<CThing> vtable address is hoisted into ebx
// CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902FE8, length 87 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255 (out of line), CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 three times (inlined CIntelligentPointer constructors: vtable
// 0x01238C6C for CIntelligentPointer<CThing>, 0x0123E7D8 for CIntelligentPointer<CThingCreatureBase>).
// Vtable 0x0128D384 = CAIStateGroup_JackOfBladesBase (rtti-vtables.tsv).
//
// Shape: base ctor, own vtable store, then the members in declaration order (names from
// rebuild/include/engine/CAIStateGroup_JackOfBladesBase.h, whose pointer/vector members are opaque byte
// arrays, so the layout is modelled locally): PSwordEmitter +0x14, PArenaCentre +0x1c, PTargetThing
// +0x24, then the two 12-byte std::vectors PillarMarkers (+0x34) and Pillars (+0x40) zeroed by their
// inline constructors -- six consecutive zero stores, which is what fixes the retail vector at 12 bytes
// (the PDB says 0x10 and puts Pillars at +0x44). The shared CIntelligentPointer<CThing> vtable constant
// is hoisted into ebx by the compiler (only under optimize("s"): the TU is size-optimised). EnemyKnockDownFrame/SpeechCounter (+0x2c/+0x30) stay uninitialised.
struct CThing;
struct CThingCreatureBase;

struct CAIStateGroupBase {
    virtual void v00();
    CAIStateGroupBase();                        // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04 the tracked object (set by the base ctor)
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }      // inlined: base ctor + vtable
};

template<class T> struct CRetailVector {        // retail std::vector<T>: first/last/end, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
};

struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    unsigned char _base[0x10];                  // +0x04 CAIStateGroupBase members (PBrain, priority, name, flags)
    CIntelligentPointer<CThing> PSwordEmitter;  // +0x14
    CIntelligentPointer<CThing> PArenaCentre;   // +0x1c
    CIntelligentPointer<CThingCreatureBase> PTargetThing;    // +0x24
    long EnemyKnockDownFrame;                   // +0x2c (uninitialised here)
    long SpeechCounter;                         // +0x30 (uninitialised here)
    CRetailVector<CIntelligentPointer<CThing> > PillarMarkers;   // +0x34
    CRetailVector<CIntelligentPointer<CThing> > Pillars;         // +0x40 (PDB: +0x44)
    virtual void v00();
    CAIStateGroup_JackOfBladesBase();
};

CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase()
    : CAIStateGroupBase()
    , PSwordEmitter()
    , PArenaCentre()
    , PTargetThing()
    , PillarMarkers()
    , Pillars()
{
}