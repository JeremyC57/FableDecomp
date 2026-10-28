// CPersistContext::Transfer<CFireEffectCreationSet>(CFireEffectCreationSet& value) -- binary-only, unnamed transfer of a
// fire-effect creation set: CRC check, a default traits value, then read or write the ContainmentMap by mode
// Leaf: ??$Transfer@VCFireEffectCreationSet@@@CPersistContext@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E59E1, 88 bytes, `leave ; ret 4`; one caller, CFlammableDef::Transfer (call at
// 0x004E59D7, ecx = the context, the pushed field is CFlammableDef +0x38 = Ego_r.pdb CFlammableDef::EffectCreationSet).
// Identity: Ego_r.pdb has Transfer<CFireEffectCreationSet>(const char*, CFireEffectCreationSet&[, const&]) and the
// CPersistTraits<CFireEffectCreationSet> TransferBinaryIn/Out pair; this retail body takes only the value (like the
// unnamed Transfer<float> 0x00431061), so the parameter list is a placeholder. The old label
// CFlammableDef::TransferHelper had `this` and the argument swapped (ecx is the context: CheckCRC(""), Mode +0x18,
// PLoadStream +0x24, PSaveStream +0x28).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); the traits value is a default
// CFireEffectCreationSet (constructor 0x004E0A05; Ego_r.pdb layout: an 8-byte base, then
// map<CDefString,long,CCompareDefString<long> > ContainmentMap +8); Mode 2: GFSerialiseMapBinaryIn(*PLoadStream,
// value.ContainmentMap) (0x004E5A76, PDB-exact template), Mode 3: GFSerialiseMapBinaryOut(*PSaveStream,
// value.ContainmentMap) (0x004E5A39); then the traits value's destructor inlines: the map (0x004DD470) and the base
// (0x0099A410).
// Shape: the frame-pointer module (`leave`), the mode decoded with `dec ; dec ; je ; dec ; jne`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMemoryDataInputStream;
class CDataOutputStream;
class CDefString;
template <class T> class CCompareDefString;

namespace std {
template <class T> class allocator;
template <class T1, class T2> struct pair;
template <class K, class V, class C, class A> class map {
public:
    unsigned char _body[12];                            // STLport _Rb_tree: header, node count, comparator
    ~map();                                             // 0x004DD470 for the ContainmentMap type
};
}
typedef std::map<CDefString, long, CCompareDefString<long>, std::allocator<std::pair<const CDefString, long> > >
    TContainmentMap;

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream,
    std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);                 // 0x004E5A76 for <long,CDefString>
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream,
    const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);           // 0x004E5A39 for <long,CDefString>

class CInitBaseClass {                                  // the 8-byte base of CFireEffectCreationSet
public:
    unsigned char _body[8];
    ~CInitBaseClass();                                  // 0x0099A410
};

class CFireEffectCreationSet : public CInitBaseClass {
public:
    TContainmentMap ContainmentMap;                     // +0x08
    CFireEffectCreationSet();                           // 0x004E0A05
};

template <class T> class CPersistTraits;
template <> class CPersistTraits<CFireEffectCreationSet> {
public:
    CFireEffectCreationSet Value;
    void TransferBinaryIn(CMemoryDataInputStream& stream, CFireEffectCreationSet& value)
    {
        GFSerialiseMapBinaryIn(stream, value.ContainmentMap);
    }
    void TransferBinaryOut(CDataOutputStream& stream, const CFireEffectCreationSet& value)
    {
        GFSerialiseMapBinaryOut(stream, value.ContainmentMap);
    }
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

template <>
void CPersistContext::Transfer<CFireEffectCreationSet>(CFireEffectCreationSet& value)
{
    CheckCRC("");
    CPersistTraits<CFireEffectCreationSet> traits;
    switch (Mode) {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
