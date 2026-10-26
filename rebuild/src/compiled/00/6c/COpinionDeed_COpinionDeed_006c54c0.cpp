// COpinionDeed::COpinionDeed(EOpinionDeedType, long handle, const C3DVector& position, const CThing* target, bool happening, bool isCrime, long extraData) -- record a deed: type, handle, where, the world frame it happened on, the target, the witness/happening flags (both from one argument), not yet reported, crime flag and extra data
// Leaf: ??0COpinionDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C54C0, length 109 bytes.
// Callees: GetWorldFrame @ 0x0049D870 (returns the global @ 0x013B89BC; manifest "GFGetPreMainMemoryUsed" is wrong),
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtables: CIntelligentPointer<const CThing> 0x01237F1C.
// Layout: PDB COpinionDeed (0x28 bytes): DeedType +0, DeedHandle +4, Position +8, FrameHappened +0x14,
// PTarget +0x18 (CIntelligentPointer, 8 bytes), WasSearchingForWitnesses +0x20, IsStillHappening +0x21,
// IsCrimeReportedToVillage +0x22, IsCrime +0x23, ExtraData +0x24.
// Shape: the position copy goes through two alternating registers (memberwise user copy constructor of
// C3DVector); the intelligent pointer is built by its inline (T*) constructor: out-of-line base ctor, own
// vtable store, SetPItem(target). The same bool argument initialises both +0x20 and +0x21.
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(const void* item);               // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual void v00();                            // own vtable (0x01237F1C)
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

long GetWorldFrame_0049d870();                     // 0x0049D870

class COpinionDeed { public:
    long                              DeedType;                   // +0x00 EOpinionDeedType
    long                              DeedHandle;                 // +0x04
    C3DVector                         Position;                   // +0x08
    long                              FrameHappened;              // +0x14
    CIntelligentPointer<const CThing> PTarget;                    // +0x18
    bool                              WasSearchingForWitnesses;   // +0x20
    bool                              IsStillHappening;           // +0x21
    bool                              IsCrimeReportedToVillage;   // +0x22
    bool                              IsCrime;                    // +0x23
    long                              ExtraData;                  // +0x24
    COpinionDeed(long type, long handle, const C3DVector& position, const CThing* target,
                 bool happening, bool isCrime, long extraData);
};

COpinionDeed::COpinionDeed(long type, long handle, const C3DVector& position, const CThing* target,
                           bool happening, bool isCrime, long extraData)
    : DeedType(type), DeedHandle(handle), Position(position), FrameHappened(GetWorldFrame_0049d870()),
      PTarget(target), WasSearchingForWitnesses(happening), IsStillHappening(happening),
      IsCrimeReportedToVillage(false), IsCrime(isCrime), ExtraData(extraData)
{
}