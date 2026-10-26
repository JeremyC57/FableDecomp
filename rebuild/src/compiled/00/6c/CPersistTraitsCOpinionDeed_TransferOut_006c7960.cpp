// CPersistTraits<COpinionDeed*>::TransferOut(CPersistContext* context, CCharString* out, COpinionDeed* const* value) -- text persist write of an opinion deed as "(type,handle,position,frame,FALSE,target,reported,crime,extra)", the frame being the current one while the deed is still happening
// Leaf: ?TransferOut@?$CPersistTraits@PAVCOpinionDeed@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7960, 416 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb COpinionDeed (0x28 bytes) = { EOpinionDeedType DeedType +0, long DeedHandle +4, C3DVector Position +8,
//   long FrameHappened +0x14, CIntelligentPointer<const CThing> PTarget +0x18, bool WasSearchingForWitnesses +0x20,
//   bool IsStillHappening +0x21, bool IsCrimeReportedToVillage +0x22, bool IsCrime +0x23, long ExtraData +0x24 }.
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 ("(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020,
//   "TRUE" 0x0122D6D4, "FALSE" 0x0122D6CC), TransferOutLong @ 0x009E2560 (`__fastcall` context, out, &copy),
//   CPersistTraits<C3DVector>::TransferOut @ 0x009E7F90, CPersistTraits<CIntelligentPointer<const CThing> >::TransferOut
//   @ 0x006C7B00 (the non-const CIntelligentPointer<CThing> traits, RTTI .?AV?$CIntelligentPointer@VCThing@@@@ on vtable
//   0x01238C6C; PTarget itself is the <const CThing> pointer, vtable 0x01237F1C), GFGetCurrentFrame @ 0x0049D870 (returns the global @ 0x013B89BC; manifest "GFGetPreMainMemoryUsed"),
//   CBaseIntelligentPointer() @ 0x00A01B10 / ~CBaseIntelligentPointer @ 0x00A01C10 (the pointer traits' DefaultVal;
//   its vtable stored inline, no store in its generated destructor).
// Shape: the landed text writers (0x0057F78F / 0x00614080): the two traits objects with a DefaultVal live in the frame
//   (the C3DVector one zeroed by its inline constructor, the pointer one built out of line), each long through an inline
//   CPersistTraits<long>::TransferOut (copy into the dead `out` slot), each bool through the inline
//   `if (*value) out += "TRUE"; else out += "FALSE";` (one merged call; the witness flag is always written as a false
//   local, which folds to "FALSE"). `*value` is re-read after every call. No EH frame. Speed-optimised (default) TU.
class CPersistContext;
class CThing;
class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString& operator+=(const char* text);         // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560
long __fastcall GFGetCurrentFrame();                   // 0x0049D870

class C3DVector {
public:
    float X;
    float Y;
    float Z;
    C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {}
};

class CBaseIntelligentPointer {
public:
    void* PItem;                                       // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <CThing> 0x01238C6C, <const CThing> 0x01237F1C
public:
    CIntelligentPointer() {}
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0, OPINION_DEED_TYPE_FORCE_DWORD = 0x7fffffff };

class COpinionDeed {                                   // 0x28 bytes
public:
    EOpinionDeedType DeedType;                         // +0x00
    long             DeedHandle;                       // +0x04
    C3DVector        Position;                         // +0x08
    long             FrameHappened;                    // +0x14
    CIntelligentPointer<const CThing> PTarget;         // +0x18
    bool             WasSearchingForWitnesses;         // +0x20
    bool             IsStillHappening;                 // +0x21
    bool             IsCrimeReportedToVillage;         // +0x22
    bool             IsCrime;                          // +0x23
    long             ExtraData;                        // +0x24
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(context, out, &copy);
    }
};
template <> struct CPersistTraits<bool> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const bool* value) const
    {
        if (*value)
            *out += "TRUE";
        else
            *out += "FALSE";
    }
};
template <> struct CPersistTraits<C3DVector> {
    C3DVector DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, const C3DVector* value);   // 0x009E7F90
};
template <> struct CPersistTraits<CIntelligentPointer<CThing> > {
    CIntelligentPointer<CThing> DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, const CIntelligentPointer<CThing>* value);   // 0x006C7B00
};
template <> struct CPersistTraits<COpinionDeed*> {
    void TransferOut(CPersistContext* context, CCharString* out, COpinionDeed* const* value);
};

void CPersistTraits<COpinionDeed*>::TransferOut(CPersistContext* context, CCharString* out, COpinionDeed* const* value)
{
    CPersistTraits<long> longTraits;
    CPersistTraits<bool> boolTraits;
    CPersistTraits<C3DVector> vectorTraits;
    CPersistTraits<CIntelligentPointer<CThing> > pointerTraits;
    *out += "(";
    longTraits.TransferOut(context, out, (const long*)&(*value)->DeedType);
    *out += ",";
    longTraits.TransferOut(context, out, &(*value)->DeedHandle);
    *out += ",";
    vectorTraits.TransferOut(context, out, &(*value)->Position);
    *out += ",";
    long frame = (*value)->IsStillHappening ? GFGetCurrentFrame() : (*value)->FrameHappened;
    longTraits.TransferOut(context, out, &frame);
    *out += ",";
    bool searching = false;
    boolTraits.TransferOut(context, out, &searching);
    *out += ",";
    pointerTraits.TransferOut(context, out, (const CIntelligentPointer<CThing>*)&(*value)->PTarget);
    *out += ",";
    boolTraits.TransferOut(context, out, &(*value)->IsCrimeReportedToVillage);
    *out += ",";
    boolTraits.TransferOut(context, out, &(*value)->IsCrime);
    *out += ",";
    longTraits.TransferOut(context, out, &(*value)->ExtraData);
    *out += ")";
}