// CSkeletalMorphResourceManager::PreloadBoneConfigs() -- load every Data\Bones\*.bncfg morph target that is not already in the resource map
// Leaf: PreloadBoneConfigs
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C37D0, length 459 bytes (/GS cookie, no EH frame, `and esp,-8`).
// Callees: the L"Data\\Bones\\" fastcall getter 0x0041A290 (manifest "GetAutoSaveFileName", propagated),
// operator+(CWideString, const wchar16*) 0x0099BF30, CWideString(const wchar16*) 0x0099B6B0, ~CWideString 0x0099B510,
// CWideString::ConvertToCharString 0x0099B2C0, ~CCharString 0x0099EAE0, FindFirstFile 0x00999760 / FindNextFile
// 0x00999850 (__fastcall pattern, finder, name buffer), the map's find 0x006C31B0 / insert 0x006C3410,
// CDiskFileWin32(path, 1, 2) 0x0099AD80 (inline dtor: Close 0x0099A920, ~CBase 0x0099A300), operator new
// 0x00BFEA1A, C3DSkeletalMorphTarget() 0x00A44E60, CCountedPointer::Reset 0x006C3AC0,
// GFLoadSkeletalMorphTargetFromFile 0x0088B0C0 (__fastcall).
// Layout: same map model as the landed GetMorphTarget 0x006C3620 (map at +0x04, node key +0x10, value +0x14).
// Shape: the /GS cookie sits above a 264-wchar16 name buffer; each found file is wrapped in a CWideString, its
// CCharString key is rebuilt for every find/insert/find; a missing key is inserted, re-found, and loaded through
// a scoped CDiskFileWin32 into a freshly reset target.
typedef unsigned short wchar16;               // wchar_t is not a built-in type under the lane flags
struct CWideString {
    void* PStringData;
    CWideString(const wchar16* text);              // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
    struct CCharString ConvertToCharString() const;   // 0x0099B2C0
};
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};
CWideString __fastcall GetBonesDirectory();        // 0x0041A290 (L"Data\\Bones\\")
CWideString __fastcall operator+(const CWideString& a, const wchar16* b);      // 0x0099BF30
struct CFileFinder { void* Handle; unsigned long Flags; };
bool __fastcall FindFirstFile(const CWideString& pattern, CFileFinder* finder, wchar16* fileName);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFileFinder* finder, wchar16* fileName);    // 0x00999850
namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBase {                    // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                          // 0x0099A920 (slot 2)
    long        AccessType;
    long        Position;
    long        Length;
    void*       WinFileHandle;
    bool        Opened;
    CWideString Pathname;                          // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};
struct C3DSkeletalMorphTarget {
    unsigned char _pad[0x10];
    C3DSkeletalMorphTarget();                      // 0x00A44E60
};
void __fastcall GFLoadSkeletalMorphTargetFromFile(CDiskFileWin32* file, C3DSkeletalMorphTarget* target);   // 0x0088B0C0
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* Deleter)(void* object);
    void* Object;
};
struct CMorphTargetPointer {                       // CCountedPointer<C3DSkeletalMorphTarget>
    C3DSkeletalMorphTarget* Data;
    CCPPointerInfo*         Info;
    CMorphTargetPointer() : Data(0), Info(0) {}
    ~CMorphTargetPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->Deleter(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    void Reset(C3DSkeletalMorphTarget* p);         // 0x006C3AC0
};
struct CMorphPair {
    CCharString         first;
    CMorphTargetPointer second;
    CMorphPair(const CCharString& k) : first(k) {}
};
struct CMorphNode {
    unsigned char       Color;
    CMorphNode*         Parent;
    CMorphNode*         Left;
    CMorphNode*         Right;
    CCharString         Key;                       // +0x10
    CMorphTargetPointer Value;                     // +0x14
};
struct CMorphInsertResult { CMorphNode* Position; bool Inserted; CMorphInsertResult() {} };
struct CMorphMap {                                 // STLport map<CCharString, CCountedPointer<C3DSkeletalMorphTarget>>
    CMorphNode* Header;
    unsigned long Count;
    unsigned long Compare;
    CMorphNode* find(const CCharString& key);      // 0x006C31B0
    CMorphInsertResult insert(const CMorphPair& value);   // 0x006C3410
    CMorphNode* end() const { return Header; }
};
struct CSkeletalMorphResourceManager {
    virtual void v00();
    CMorphMap Morphs;                              // +0x04
    void PreloadBoneConfigs();
};

void CSkeletalMorphResourceManager::PreloadBoneConfigs()
{
    CWideString pattern = GetBonesDirectory() + L"*.bncfg";
    CFileFinder finder;
    wchar16 fileName[264];
    if (FindFirstFile(pattern, &finder, fileName)) {
        do {
            CWideString name(fileName);
            CMorphNode* node = Morphs.find(name.ConvertToCharString());
            if (node == Morphs.end()) {
                Morphs.insert(CMorphPair(name.ConvertToCharString()));
                node = Morphs.find(name.ConvertToCharString());
                CDiskFileWin32 file(name, FILE_ACCESS_READ, 2);
                node->Value.Reset(new C3DSkeletalMorphTarget);
                GFLoadSkeletalMorphTargetFromFile(&file, node->Value.Data);
            }
        } while (FindNextFile(pattern, &finder, fileName));
    }
}