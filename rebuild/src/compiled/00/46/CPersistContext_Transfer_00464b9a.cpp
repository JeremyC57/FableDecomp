// CPersistContext::Transfer(CSimVoice& value) -- binary-only, unnamed sub-object transfer: CRC-check the empty key, build a CPersistTraits<CSimVoice> (default value) local, then binary load through value.TransferBinaryIn or binary save through value.TransferBinaryOut; the other modes do nothing
// Leaf: ??$Transfer@VCSimVoice@@@CPersistContext@@QAEXAAVCSimVoice@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00464B9A, 74 bytes (size-optimised TU, frame pointer, odd entry), thiscall `ret 4`.
// Identity: T = CSimVoice -- proven; evidence: its only caller CSimVoicesDef::Transfer 0x00464B8A passes this+0x3C, the CSimVoice member the landed CSimVoicesDef ctor 0x00455E72 builds with CSimVoice::CSimVoice 0x00455E39 (Ego_r CSimVoicesDef::Entry +0x40, -4 for the parent def); the local is constructed by that same ctor, 0x18 bytes = retail CSimVoice (Ego_r 0x1C with a 16-byte vector).
//   Family G066 (seven byte-identical bodies, one COMDAT per T). Catalogue label "TransferCSimVoice" is propagated/behavioural.
//   No Ego_r symbol takes only the value (TLC's def transfers are binary-only and unnamed); the parameter list follows the
//   landed Transfer<unsigned long> 0x00431020 and this chunk's G002 Transfer<enum> rows.
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty literal 0x0122D70E), CSimVoice::CSimVoice() 0x00455E39,
//   CSimVoice::TransferBinaryOut(CDataOutputStream&) const 0x006808C0, CSimVoice::TransferBinaryIn(CMemoryDataInputStream&) 0x00680950,
//   CSimVoice::~CSimVoice() 0x00455E59 (the traits' implicit ctor/dtor and forwarders inline; T's own are out of line).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: the G002 switch (cases 2 and 3 only, binary save laid out first) in a frame-pointer /Os TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CSimVoice {
public:
    CSimVoice();                                                       // 0x00455E39
    virtual ~CSimVoice();                                                      // 0x00455E59
    void TransferBinaryIn(CMemoryDataInputStream& stream);       // 0x00680950
    void TransferBinaryOut(CDataOutputStream& stream) const;     // 0x006808C0
private:
    unsigned char _body[0x14];                                    // retail sizeof 0x18
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
template void CPersistContext::Transfer<CSimVoice >(CSimVoice& value);