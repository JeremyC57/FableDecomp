// _global::GetDoorTypeSymbolMap() -- the hero-centre door-type symbol map, loaded once from "Data\Defs\" + "thing_def.h" on first use
// Leaf: GetDoorTypeSymbolMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F510, 81 bytes (+ int3 padding to 96), static __fastcall with no
//   arguments, plain `ret`.
// Caller: CTCHeroCentreDoorMarker::OnSerialise @ 0x0077F650 (landed; translates an old save's door-type name through the
//   map with CSymbolMap::Lookup 0x009E5170).
// Callees: GetDefsDirectory() @ 0x0041A080 (L"Data\Defs\", __fastcall with the hidden result in ecx; catalogued as the
//   propagated CUserProfileManager::GetAutoSaveFileName), operator+(const CWideString&, const unsigned short*) @
//   0x0099BF30 (__fastcall, hidden result in ecx, left operand in edx, the literal on the stack), CSymbolMap::
//   LoadFromFile(const CWideString&) @ 0x009E5C90, ~CWideString @ 0x0099B510 (x2).
// Data: the loaded flag byte @ 0x013BADB0, the CSymbolMap @ 0x013BADB4 (a file-scope object built by its own static
//   initialiser: the flag is a plain bool, not a VC7.1 local-static guard bit), L"thing_def.h" @ 0x0126B01C.
// Shape: right-to-left argument evaluation -- operator+'s literal is pushed first, then the directory temporary is
//   built (frame word +4) and the sum lands in frame word +0; the temporaries die in reverse order (sum, directory).
//   Same shape as the landed CSubtitledMessage::Initialise 0x004CDB10.
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};

CWideString __fastcall GetDefsDirectory();             // 0x0041A080
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

class CSymbolMap {
public:
    bool LoadFromFile(const CWideString& filename);     // 0x009E5C90
};
extern CSymbolMap g_DoorTypeSymbolMap;                  // 0x013BADB4

CSymbolMap* __fastcall GetDoorTypeSymbolMap()
{
    static bool s_loaded = false;                       // 0x013BADB0
    if (!s_loaded) {
        g_DoorTypeSymbolMap.LoadFromFile(GetDefsDirectory() + L"thing_def.h");
        s_loaded = true;
    }
    return &g_DoorTypeSymbolMap;
}