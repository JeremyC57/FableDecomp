// CWorldMap::SetAsUsingWAD(const CWideString& wadName) -- switch the level data source: an empty name drops the level WAD bank; otherwise a new CWADBank is opened on the file (dropped again if it fails to open)
// Leaf: SetAsUsingWAD
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FDAB0, 269 bytes, `ret 4`. Called from CWorld::Load (0x004A1840) on CWorld::PWorldMap.
//   PDB: ?SetAsUsingWAD@CWorldMap@@QAEXABVCWideString@@@Z.
// Label correction: catalogued as CPhysicsFacesSegment::LoadPhysicsData; the caller passes CWorld+0x14 (PWorldMap) and the
//   members are CWorldMap's LevelWADBank / UseLevelWADBank.
// Callees: wcscmp 0x00BFE94C, MemCmp_Unsigned16 0x00403C60 (the STLport wide char_traits compare), operator new 0x00BFEA1A,
//   CBankFileAsync::CBankFileAsync() 0x009D5F80 (the CWADBank base), CCountedPointer<CWADBank>::Reset(CWADBank*) 0x0050F2B0
//   (catalogued for CIPhysicsFacesSegmentBase), CWADBank virtual slot 3 (+0x0C, open by name with flags 0x102; descriptive),
//   operator delete 0x00BFE9BC. L"" is 0x0122D70C. CWADBank vtable 0x012442DC (0x164 bytes).
// Layout: engine/CWorldMap.h carries Ego_r.pdb offsets (LevelWADBank +0xC0); retail CWorldMap containers are 12 bytes, so
//   LevelWADBank is at +0xA0 (Data) / +0xA4 (Info) and UseLevelWADBank at +0xA8 (GOTCHAS chunk 179/220 retail offsets); a
//   local model is used.
// Shape: the inline CWideString `== L""` is written as two separate tests in the caller (wcscmp of the unassigned case, the
//   STLport size + compare of the block) that `goto` the empty arm (the GOTCHAS retry-9 form); the counted pointer's inline
//   Clear zeroes Info then Data.
extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b);   // 0x00BFE94C
int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)

static const unsigned short s_Empty[] = { 0 };          // L"" 0x0122D70C

struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
};

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                // 0x0050F2B0 for T = CWADBank
    T* operator->() const { return Data; }
    void Clear()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};

class CBankFileAsync {
public:
    CBankFileAsync();                                   // 0x009D5F80
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool Open(const CWideString& name, unsigned long flags);   // slot 3 (+0x0c)
    unsigned char _body[0x164 - 4];
};
class CWADBank : public CBankFileAsync {               // vtable 0x012442DC
public:
    virtual bool Open(const CWideString& name, unsigned long flags);
};

class CWorldMap {
public:
    unsigned char             _pad00[0xa0];
    CCountedPointer<CWADBank> LevelWADBank;             // +0xa0
    bool                      UseLevelWADBank;          // +0xa8
    void SetAsUsingWAD(const CWideString& wadName);
};

void CWorldMap::SetAsUsingWAD(const CWideString& wadName)
{
    if (wadName.PStringData == 0) {
        if (wcscmp(s_Empty, s_Empty) == 0)
            goto empty;
    }
    else if ((unsigned long)(wadName.PStringData->_M_finish - wadName.PStringData->_M_start) == 0
             && MemCmp_Unsigned16(wadName.PStringData->_M_start, s_Empty, 0) == 0)
        goto empty;
    UseLevelWADBank = true;
    LevelWADBank.Reset(new CWADBank);
    if (!LevelWADBank->Open(wadName, 0x102)) {
        UseLevelWADBank = false;
        LevelWADBank.Clear();
    }
    return;
empty:
    UseLevelWADBank = false;
    LevelWADBank.Clear();
}
