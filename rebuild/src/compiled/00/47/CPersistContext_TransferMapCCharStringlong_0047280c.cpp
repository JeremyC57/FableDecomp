// CPersistContext::TransferMap<CCharString,long>(map<long,CCharString>& value) -- binary-only, unnamed map transfer: CRC
// check, then read or write the whole map by mode
// Leaf: ??$TransferMap@VCCharString@@J@CPersistContext@@
// Retail: Fable.exe @ 0x0047280C, 53 bytes, `ret 4`; one caller, CGlobalDef::Transfer (call at 0x00472722, ecx = the
// context, the pushed field is CGlobalDef +0xF4 = Ego_r.pdb CGlobalDef::ConversationCameras, map<long,CCharString>).
// Identity: Ego_r.pdb has TransferMap<CCharString,long>(const char*, map<long,CCharString>&[, const CCharString&]); this
// retail body takes only the map (like the unnamed Transfer<float> 0x00431061), so the parameter list is a placeholder.
// The earlier CEngineLightingManager::UpdateShadowScene label was propagated from an unrelated body.
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); Mode (+0x18) 2 = binary load:
// GFSerialiseMapBinaryIn(*PLoadStream (+0x24), value) (0x00472896); 3 = binary save: GFSerialiseMapBinaryOut(*PSaveStream
// (+0x28), value) (0x00472841). Both callees are __fastcall templates (PDB `YIX`).
// Layout: Ego_r.pdb CPersistContext -- Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; no engine/ header exists.
#pragma optimize("s",on)
namespace std {
    template <class T> struct less;
    template <class T> class allocator;
    template <class T1, class T2> struct pair;
    template <class K, class V, class C, class A> class map;
}
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream,
    std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);                 // 0x00472896 for <CCharString,long>
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream,
    const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);           // 0x00472841 for <CCharString,long>

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
    template <class V, class K>
    void TransferMap(std::map<K, V, std::less<K>, std::allocator<std::pair<const K, V> > >& value);
};

template <>
void CPersistContext::TransferMap<CCharString, long>(
    std::map<long, CCharString, std::less<long>, std::allocator<std::pair<const long, CCharString> > >& value)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_BINARY_LOAD:
        GFSerialiseMapBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        GFSerialiseMapBinaryOut(*PSaveStream, value);
        break;
    }
}
