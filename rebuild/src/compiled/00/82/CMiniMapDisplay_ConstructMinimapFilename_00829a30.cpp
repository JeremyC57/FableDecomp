// CMiniMapDisplay::ConstructMinimapFilename(const CWideString& path, CWorldMap* map, long index) -- build "<file name of path>_<current region name><index, two digits below 10>.tga"
// Retail 0x00829A30, 498 bytes, `ret 0x10` (hidden CWideString return, path, map, index; `this` unused).
// Callees: CWideString() 0x0099AED0 (manifest label "CCharString::CCharString" is wrong), ~CWideString 0x0099B510,
//   CWideString(const CWideString&) 0x0099B720, operator= 0x0099B7D0, operator+=(const wchar_t*) 0x0099B940,
//   operator+=(const CWideString&) 0x0099B8D0, the varargs (__cdecl) CWideString::Sprintf 0x0099BA70 (label
//   "CCharString::SetFromFormatV"), __fastcall operator+(const CWideString&, const CWideString&) 0x0099BE70, __fastcall
//   ConvertPathToFullPath 0x00999110, __fastcall SplitPath 0x00997620 (file name only), CCharString(const CCharString&)
//   0x0099EC30, ~CCharString 0x0099EAE0, CCharString::ToWideString 0x0099E480, CWorldMap's current-region index
//   0x004FB150 and GetRegion 0x004FC180 (CRegion name at +0x18).
// Strings: L"_" 0x0122DE7C, L"%d" 0x0122DE44, L".tga" 0x01231634.
// Shape: the number strings come from an inline by-value ToString (local Format, copy out, no NRVO); the region
//   name + number sum is built into a named local that is never used (retail destroys it with the others).
class CWideString { public:
    void* m_pData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    const CWideString& operator=(const CWideString& other);         // 0x0099B7D0
    const CWideString& operator+=(const unsigned short* text);   // 0x0099B940
    const CWideString& operator+=(const CWideString& other);     // 0x0099B8D0
    void Sprintf(const unsigned short* format, ...);            // 0x0099BA70
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);   // 0x00999110

public:
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
};           // 0x00999110
   // 0x00997620
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};
class CRegion { public:
    unsigned char _pad00[0x18];
    CCharString   Name;                            // +0x18
};
class CWorldMap { public:
    long GetLoadedRegion() const;            // 0x004FB150
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
};

inline CWideString ToString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}

class CMiniMapDisplay { public:
    CWideString ConstructMinimapFilename(const CWideString& path, CWorldMap* map, long index);
};

CWideString CMiniMapDisplay::ConstructMinimapFilename(const CWideString& path, CWorldMap* map, long index)
{
    CWideString filename;
    CWideString fullPath = CAFile::ConvertPathToFullPath(path);
    CAFile::SplitPath(fullPath, 0, 0, &filename, 0);
    CCharString regionName(map->GetRegion(map->GetLoadedRegion()).Name);
    filename += L"_";
    filename += regionName.ToWideString();
    CWideString number;
    if (index < 10)
        number = ToString(0) + ToString(index);
    else
        number = ToString(index);
    CWideString unused = regionName.ToWideString() + number;
    filename += number;
    filename += L".tga";
    return filename;
}