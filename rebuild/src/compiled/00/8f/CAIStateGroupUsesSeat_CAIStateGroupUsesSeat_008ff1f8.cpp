// CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat() -- base, own vtable, the seat pointer, no seat position yet.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FF1F8, length 51 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (PSeat at +0x14, vtable 0x0125A858 = CIntelligentPointer<CThingObject>). Own vtable 0x0128B53C
// (rtti-vtables: CAIStateGroup_UsesSeat; the manifest label "OpinionReactionRoot ctor" is propagated).
// Shape: base constructor, vtable, the member constructor, then SeatPosIndex (+0x20) = -1 (`or ..,-1`) -- an
// init-list member declared after PSeat, so its store follows the member constructor (the vtable store cannot sink
// below a member constructor call) -- then the body: GotSeatPos (+0x1c), Sitting (+0x1d) and SittingOnBench
// (+0x30) cleared from one `xor al,al` (as body assignments VC schedules the -1 store after them). Member names and
// offsets from struct_layouts_egor.tsv (CAIStateGroup_UsesSeat, 52 bytes); SeatPos (+0x24) is left alone.
// Pragma: `#pragma optimize("s", on)` (the `or [mem],-1` store is the size-optimised -1 initialiser).
#pragma optimize("s", on)
class CThing;

class CBaseObject;
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

class CThingObject;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0125A858
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {   // vtable 0x0128B53C
    CIntelligentPointer<CThingObject> PSeat;            // +0x14
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    float SeatPos_X, SeatPos_Y, SeatPos_Z;      // +0x24 C3DVector
    bool SittingOnBench;                        // +0x30
    CAIStateGroup_UsesSeat();
};

CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat()
    : CAIStateGroupBase()
    , PSeat()
    , SeatPosIndex(-1)
{
    GotSeatPos = false;
    Sitting = false;
    SittingOnBench = false;
}