// CPersistContext::TransferMapBinary<ETableSprites,long>(std::map<ETableSprites,long>& value) -- binary-only, unnamed transfer of the UI def's sprite map: CRC check, then read it from the load stream or write it to the save stream by mode
// Leaf: ??$TransferMapBinary@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006327A0, 55 bytes (+ int3 padding), `ret 4` (the map is the only argument).
// Identity: the only caller is the CUIDef def-Transfer 0x00631C60 (call at 0x00631CD8) on +0x64 = PDB CUIDef::Sprites, a
// map<NUISystem::ETableSprites, long>. Ego_r.pdb's TransferMap<K,V> takes (const char*, map&); this retail instance takes
// only the map and handles only the binary modes, so the name is descriptive (as the landed TransferVectorBinary
// 0x004331F9). The catalog label "CPersistContext_TransferList_CrimeOpinion" is propagated and wrong.
// Callees: CPersistContext::CheckCRC 0x00404500 (the pooled empty literal 0x0122D70E), GFSerialiseMapBinaryOut 0x006327E0
// (mode 3) and GFSerialiseMapBinaryIn 0x00632830 (mode 2), both __fastcall (stream, map).
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; as the landed 0x004331F9 model).
// Shape: speed TU -- a two-case switch on Mode lowers to `sub eax,2 ; je ; dec eax ; jne`, the mode-3 body laid out first;
// each arm loads the map argument into edx and ends in its own `pop esi ; ret 4`.
class CMemoryDataInputStream;
class CDataOutputStream;

namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_0 };
}

namespace std {
template <class T> struct less { };
template <class K, class V, class C = less<K> > class map {
public:
    void* _M_header;                                     // +0x00
    unsigned long _M_node_count;                         // +0x04
    C _M_key_compare;                                    // +0x08
};
}

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& value);   // 0x00632830
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<K, V, C>& value);   // 0x006327E0

class CPersistContext {
public:
    void*                   PDefinitionManager;          // +0x00
    void*                   PSymbolMap;                  // +0x04
    void*                   PParser;                     // +0x08
    void*                   ErrorCallback;               // +0x0c
    void*                   PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    long                    Mode;                        // +0x18 EMode: 2 binary load, 3 binary save
    bool                    Binary;                      // +0x1c
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    void CheckCRC(const char* name);                     // 0x00404500
    template <class K, class V, class C> void TransferMapBinary(std::map<K, V, C>& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 2:
            GFSerialiseMapBinaryIn(*PLoadStream, value);
            break;
        case 3:
            GFSerialiseMapBinaryOut(*PSaveStream, value);
            break;
        }
    }
};

template void CPersistContext::TransferMapBinary<NUISystem::ETableSprites, long, std::less<NUISystem::ETableSprites> >(
    std::map<NUISystem::ETableSprites, long, std::less<NUISystem::ETableSprites> >&);