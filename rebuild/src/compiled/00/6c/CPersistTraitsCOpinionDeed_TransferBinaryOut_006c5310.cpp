// CPersistTraits<COpinionDeed*>::TransferBinaryOut(CDataOutputStream*, COpinionDeed* const*) -- write a hero opinion deed: type, handle, position, frame, crime flags, extra data and the target thing's UID
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5310, 201 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0, Write3DVector @ 0x00993FD0, WriteEBOOL @ 0x00993E30,
//   WriteUInt64 @ 0x00993EC0 (manifest "WriteCBYTE"); GFGetCurrentFrame @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed",
//   a propagated label: it returns the frame global 0x013B89BC); CBaseIntelligentPointer ctor @ 0x00A01B10, GetPItem
//   @ 0x00A01B50, dtor @ 0x00A01C10; CIntelligentPointer vtable 0x01238C6C.
// Layout (PDB COpinionDeed, 0x28 bytes): DeedType +0, DeedHandle +4, Position +8, FrameHappened +0x14, PTarget
//   (CIntelligentPointer<const CThing>, 8 bytes) +0x18, WasSearchingForWitnesses +0x20, IsStillHappening +0x21,
//   IsCrimeReportedToVillage +0x22, IsCrime +0x23, ExtraData +0x24. No engine header exists; local model.
// Shape: the frame written is the live frame while the deed is still happening (`IsStillHappening ? GFGetCurrentFrame()
//   : FrameHappened`). The target goes through a local CPersistTraits<CIntelligentPointer<const CThing> > (8 bytes: its
//   default value is an intelligent pointer -- inline ctor = base ctor call + derived vtable store, implicit dtor = base
//   dtor call) whose TransferBinaryOut is inlined (the out-of-line copy is 0x006C52D0): UniqueID (+0x18 of the thing)
//   or 0 for an empty pointer. Speed-optimised (default) TU.
#include "engine/CDataOutputStream.h"

struct C3DVector { float X, Y, Z; };

struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                    // 0x00993E30
    void WriteLong(long value);                     // 0x00993EB0
    void WriteUInt64(unsigned __int64 value);       // 0x00993EC0
    void Write3DVector(const C3DVector& value);     // 0x00993FD0
};

long GFGetCurrentFrame();                           // 0x0049D870

struct CThing {
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UniqueID;                      // +0x18
};

struct CBaseIntelligentPointer {
    virtual void v00();                             // vtable 0x0129C95C
    void* PData;                                    // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() {}
};

struct COpinionDeed {
    long                               DeedType;                    // +0x00 EOpinionDeedType
    long                               DeedHandle;                  // +0x04
    C3DVector                          Position;                    // +0x08
    long                               FrameHappened;               // +0x14
    CIntelligentPointer<const CThing>  PTarget;                     // +0x18
    bool                               WasSearchingForWitnesses;    // +0x20
    bool                               IsStillHappening;            // +0x21
    bool                               IsCrimeReportedToVillage;    // +0x22
    bool                               IsCrime;                     // +0x23
    long                               ExtraData;                   // +0x24
    long GetFrameHappened() const { return IsStillHappening ? GFGetCurrentFrame() : FrameHappened; }
};

template<class T> struct CPersistTraits;
template<class T> struct CPersistTraits<CIntelligentPointer<T> > {
    CIntelligentPointer<T> DefaultVal;
    void TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<T>* value)
    {
        unsigned __int64 uid = 0;
        if (value->GetPItem() != 0)
            uid = value->GetPItem()->UniqueID;
        ((CDataOutputStream_Methods*)stream)->WriteUInt64(uid);
    }
};
template<> struct CPersistTraits<COpinionDeed*> {
    void TransferBinaryOut(CDataOutputStream* stream, COpinionDeed* const* value);
};

void CPersistTraits<COpinionDeed*>::TransferBinaryOut(CDataOutputStream* stream, COpinionDeed* const* value)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    out->WriteLong((*value)->DeedType);
    out->WriteLong((*value)->DeedHandle);
    out->Write3DVector((*value)->Position);
    out->WriteLong((*value)->GetFrameHappened());
    out->WriteEBOOL((*value)->IsCrimeReportedToVillage);
    out->WriteEBOOL((*value)->IsCrime);
    out->WriteLong((*value)->ExtraData);
    CPersistTraits<CIntelligentPointer<const CThing> > traits;
    traits.TransferBinaryOut(stream, &(*value)->PTarget);
}