// CPersistContext::Transfer<CSpecialEffectsStringMap>(CSpecialEffectsStringMap& value) -- binary persist of the special-effects name map: CRC-check the empty key, then load (mode 2) or save (mode 3) its containment map, with a CPersistTraits<CSpecialEffectsStringMap> local alive across the switch
// Leaf: ??$Transfer@VCSpecialEffectsStringMap@@@CPersistContext@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045C8D3, 74 bytes, `ret 4`; y-off + size TU (frame pointer, `leave`), the def module of
//         its only caller CSpecialEffectsDef::Transfer 0x0045C8C3 (landed; it passes SpecialEffectsMap at +0x28).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (key "" @ 0x0122D70E); CSpecialEffectsStringMap::CSpecialEffectsStringMap()
//          @ 0x004533C5 (the default map: _Rb_tree ctor 0x004518B7 with the empty compare/allocator temporaries; catalogued as
//          the propagated CDisplayManager::CopyBackBufferToTexture); map<unsigned long, long>::~map @ 0x004518E1 (clear + free
//          the header; catalogued as a list dtor twin); GFSerialiseMapBinaryOut<unsigned long, long> @ 0x0045C91D and
//          GFSerialiseMapBinaryIn<unsigned long, long> @ 0x0045C95A (both __fastcall (stream, map*); catalogued as
//          WriteState / GFSerialiseMapBinaryIn<long,long>, byte-identical instantiations).
// Identity: Ego_r.pdb names ??$Transfer@VCSpecialEffectsStringMap@@@CPersistContext@@QAEXPBDAAVCSpecialEffectsStringMap@@@Z
//   (name set only; the retail body takes just the value and checks the empty key); PDB CPersistTraits<CSpecialEffectsStringMap>
//   is { CSpecialEffectsStringMap DefaultVal; } and CSpecialEffectsStringMap { map<unsigned long, long> ContainmentMap; } -- the
//   12-byte retail STLport map {header, node count, compare} is the 12-byte frame.
// Layout: CPersistContext Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; local models (no engine headers for these).
// Shape: the same two-case binary switch as the CPhysicalPrimitiveInit instance 0x00458833; the traits' binary transfers
//   are inline and hand the stream (ecx) and the value (edx) to the __fastcall map serialisers.
#pragma optimize("y", off)
#pragma optimize("s", on)   // the def module is size-optimised

class CDataOutputStream;
class CMemoryDataInputStream;

struct CULongLongMap {                  // std::map<unsigned long, long> (STLport: header node, node count, empty compare)
    void*         _M_header;            // +0x00
    unsigned long _M_node_count;        // +0x04
    unsigned long _M_key_compare;       // +0x08
    ~CULongLongMap();                   // 0x004518E1
};

void __fastcall GFSerialiseMapBinaryOut_ULongLong(CDataOutputStream* stream, const CULongLongMap* map);   // 0x0045C91D
void __fastcall GFSerialiseMapBinaryIn_ULongLong(CMemoryDataInputStream* stream, CULongLongMap* map);     // 0x0045C95A

class CSpecialEffectsStringMap {
public:
    CULongLongMap ContainmentMap;       // +0x00
    CSpecialEffectsStringMap();         // 0x004533C5
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CSpecialEffectsStringMap> {
    CSpecialEffectsStringMap DefaultVal;                                                        // +0x00
    void TransferBinaryOut(CDataOutputStream* stream, const CSpecialEffectsStringMap* value)
    {
        GFSerialiseMapBinaryOut_ULongLong(stream, &value->ContainmentMap);
    }
    void TransferBinaryIn(CMemoryDataInputStream* stream, CSpecialEffectsStringMap* value)
    {
        GFSerialiseMapBinaryIn_ULongLong(stream, &value->ContainmentMap);
    }
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                           // +0x18 (2 = binary load, 3 = binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    protected: void CheckCRC(const char* name); public:                        // 0x00404500
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        break;
    }
}

template void CPersistContext::Transfer<CSpecialEffectsStringMap>(CSpecialEffectsStringMap& value);