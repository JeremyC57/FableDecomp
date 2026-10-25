// CPersistContext::Transfer(CAnimationSet& value) -- binary-only, unnamed sub-object transfer: CRC-check the empty key, build a CPersistTraits<CAnimationSet> (default value) local, then binary load through value.TransferBinaryIn or binary save through value.TransferBinaryOut; the other modes do nothing
// Leaf: ??$Transfer@VCAnimationSet@@@CPersistContext@@QAEXAAVCAnimationSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047367A, 74 bytes (size-optimised TU, frame pointer, odd entry), thiscall `ret 4`.
// Identity: T = CAnimationSet -- proven; evidence: callers CAppearanceDef::Transfer 0x00473504 (this+0x34 = Ego_r CAppearanceDef::Animation +0x34) and CAnimatingObjectDef::Transfer 0x004ED459 (this+0x28 = Ego_r CAnimatingObjectDef::Animation +0x28); its ctor 0x005DD2E0 is the one the CAnimatingObjectDef ctor 0x004EA1F0 runs and its dtor 0x00461F3B the one the CAnimatingObjectDef deleting dtor 0x004EBA84 runs on +0x28; local 0x20 bytes (Ego_r 0x24).
//   Family G066 (seven byte-identical bodies, one COMDAT per T). Catalogue label "CAnimationSet_TransferBinary" is propagated/behavioural.
//   No Ego_r symbol takes only the value (TLC's def transfers are binary-only and unnamed); the parameter list follows the
//   landed Transfer<unsigned long> 0x00431020 and this chunk's G002 Transfer<enum> rows.
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty literal 0x0122D70E), CAnimationSet::CAnimationSet() 0x005DD2E0,
//   CAnimationSet::TransferBinaryOut(CDataOutputStream&) const 0x005DC070, CAnimationSet::TransferBinaryIn(CMemoryDataInputStream&) 0x005DD440,
//   CAnimationSet::~CAnimationSet() 0x00461F3B (the traits' implicit ctor/dtor and forwarders inline; T's own are out of line).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: the G002 switch (cases 2 and 3 only, binary save laid out first) in a frame-pointer /Os TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CAnimationSet {
public:
    CAnimationSet();                                                       // 0x005DD2E0
    ~CAnimationSet();                                                      // 0x00461F3B
    void TransferBinaryIn(CMemoryDataInputStream& stream);       // 0x005DD440
    void TransferBinaryOut(CDataOutputStream& stream) const;     // 0x005DC070
private:
    unsigned char _body[0x20];                                    // retail sizeof 0x20
};
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value) { value.TransferBinaryIn(stream); }
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) { value.TransferBinaryOut(stream); }
};
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_BINARY_LOAD, MODE_BINARY_SAVE };
    unsigned char           _pad00[0x18];
    EMode                   Mode;                        // +0x18
    bool                    Binary;                      // +0x1c
    unsigned char           _pad1d[3];
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    void CheckCRC(const char* name);                     // 0x00404500
    template <class T> void Transfer(T& value);
};
template <class T> void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
template void CPersistContext::Transfer<CAnimationSet >(CAnimationSet& value);