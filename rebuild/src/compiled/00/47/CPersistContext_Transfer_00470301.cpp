// CPersistContext::Transfer(CSoundMap& value) -- binary-only, unnamed sub-object transfer: CRC-check the empty key, build a CPersistTraits<CSoundMap> (default value) local, then binary load through value.TransferBinaryIn or binary save through value.TransferBinaryOut; the other modes do nothing
// Leaf: ??$Transfer@VCSoundMap@@@CPersistContext@@QAEXAAVCSoundMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470301, 74 bytes (size-optimised TU, frame pointer, odd entry), thiscall `ret 4`.
// Identity: T = CSoundMap -- proven; evidence: callers CEntitySoundDef::Transfer 0x004702C8 (this+0x28 = Ego_r CEntitySoundDef::SoundMap +0x28) and CRegionDef::Transfer 0x004715AB (this+0x50 = Ego_r CRegionDef::SoundMap +0x54, -4); its dtor 0x0045645D is the one the CRegionDef deleting dtor 0x0045D2D1 runs; local 0x20 bytes = two STLport CVectorMaps (Ego_r 0x28).
//   Family G066 (seven byte-identical bodies, one COMDAT per T). Catalogue label "CSoundMap_TransferBinary" is propagated/behavioural.
//   No Ego_r symbol takes only the value (TLC's def transfers are binary-only and unnamed); the parameter list follows the
//   landed Transfer<unsigned long> 0x00431020 and this chunk's G002 Transfer<enum> rows.
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty literal 0x0122D70E), CSoundMap::CSoundMap() 0x0047040E,
//   CSoundMap::TransferBinaryOut(CDataOutputStream&) const 0x0047034B, CSoundMap::TransferBinaryIn(CMemoryDataInputStream&) 0x0047042A,
//   CSoundMap::~CSoundMap() 0x0045645D (the traits' implicit ctor/dtor and forwarders inline; T's own are out of line).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28.
// Shape: the G002 switch (cases 2 and 3 only, binary save laid out first) in a frame-pointer /Os TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CSoundMap {
public:
    CSoundMap();                                                       // 0x0047040E
    ~CSoundMap();                                                      // 0x0045645D
    void TransferBinaryIn(CMemoryDataInputStream& stream);       // 0x0047042A
    void TransferBinaryOut(CDataOutputStream& stream) const;     // 0x0047034B
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
    protected: void CheckCRC(const char* name); public:                     // 0x00404500
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
template void CPersistContext::Transfer<CSoundMap >(CSoundMap& value);