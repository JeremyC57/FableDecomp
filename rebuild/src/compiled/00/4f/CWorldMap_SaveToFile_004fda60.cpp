// CWorldMap::SaveToFile(CAFile&) -- write the map and region tables to a world file as one text block
// Leaf: ?SaveToFile@CWorldMap@@UAEXAAVCAFile@@@Z
// Retail: Fable.exe @ 0x004FDA60, 65 bytes, `ret 4`. PDB ?SaveToFile@CWorldMap@@UAEXAAVCAFile@@@Z (primary vtable
// 0x01244AEC slot 4).
// Callees: CCharString::CCharString() 0x0099E4B0, CWorldMap::SaveMapsToString(CCharString&) 0x004FB990,
// CWorldMap::SaveRegionsToString(CCharString&) 0x004FD040, CCharString::SaveToFile(CAFile&) const 0x0099E5F0,
// CCharString::~CCharString() 0x0099EAE0 (manifest label "_Cons_val<...>" is a propagation).
// Shape: one 4-byte CCharString frame local (`push ecx` frame) built, filled by the two savers, written and destroyed;
// no EH frame (the retail module is built without /EHsc).
class CAFile;
class CCharString {
public:
    void* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    void SaveToFile(CAFile& file) const;        // 0x0099E5F0
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void SaveToFile(CAFile& file);      // slot 4
    void SaveMapsToString(CCharString& text);   // 0x004FB990
    void SaveRegionsToString(CCharString& text);    // 0x004FD040
};

void CWorldMap::SaveToFile(CAFile& file)
{
    CCharString text;
    SaveMapsToString(text);
    SaveRegionsToString(text);
    text.SaveToFile(file);
}