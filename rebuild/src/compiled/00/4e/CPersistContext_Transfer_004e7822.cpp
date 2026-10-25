// CPersistContext::Transfer(CTextureMorphs& value) -- binary-only, unnamed sub-object transfer: CRC-check the empty key, build a CPersistTraits<CTextureMorphs> (default value) local, then binary load through value.TransferBinaryIn or binary save through value.TransferBinaryOut; the other modes do nothing
// Leaf: ??$Transfer@VCTextureMorphs@@@CPersistContext@@QAEXAAVCTextureMorphs@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7822, 74 bytes (size-optimised TU, frame pointer, odd entry), thiscall `ret 4`.
// Identity: T = CTextureMorphs -- proven; evidence: its only caller CHeroMorphDef::Transfer 0x004E77E9 passes this+0x28 = Ego_r CHeroMorphDef::TextureMorphs (+0x28; then SkeletalMorphs +0x3C-4 = +0x38, ParticleMorphs +0x50-8 = +0x48, IdleParticleMorphs +0x64-12 = +0x58, one STLport vector each); local 0x10 bytes = one CVectorMap.
//   Family G066 (seven byte-identical bodies, one COMDAT per T). Catalogue label "CExpressionSet::CExpressionSet::TransferHelper" is propagated/behavioural.
//   No Ego_r symbol takes only the value (TLC's def transfers are binary-only and unnamed); the parameter list follows the
//   landed Transfer<unsigned long> 0x00431020 and this chunk's G002 Transfer<enum> rows.
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty literal 0x0122D70E), CTextureMorphs::CTextureMorphs() 0x004E786C,
//   CTextureMorphs::TransferBinaryOut(CDataOutputStream&) const 0x0081ACE0, CTextureMorphs::TransferBinaryIn(CMemoryDataInputStream&) 0x0081B300,
//   CTextureMorphs::~CTextureMorphs() 0x004E787C (the traits' implicit ctor/dtor and forwarders inline; T's own are out of line).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: the G002 switch (cases 2 and 3 only, binary save laid out first) in a frame-pointer /Os TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CTextureMorphs {
public:
    CTextureMorphs();                                                       // 0x004E786C
    ~CTextureMorphs();                                                      // 0x004E787C
    void TransferBinaryIn(CMemoryDataInputStream& stream);       // 0x0081B300
    void TransferBinaryOut(CDataOutputStream& stream) const;     // 0x0081ACE0
private:
    unsigned char _body[0x10];                                    // retail sizeof 0x10
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
template void CPersistContext::Transfer<CTextureMorphs >(CTextureMorphs& value);