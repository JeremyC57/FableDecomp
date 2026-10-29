// CSkeletalMorphResourceManager::GetMorphTarget(const CCharString& name) -- find (or create and load from Data\Bones\<name>) the shared skeletal morph target for a name and return its counted pointer
// Leaf: GetMorphTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3620, length 425 bytes (`ret 4`, no EH frame).
// Callees: CCharString::ToWideString 0x0099E480, the L"Data\\Bones\\" fastcall getter 0x0041A290 (manifest
// "CUserProfileManager::GetAutoSaveFileName", propagated), operator+(CWideString, CWideString) 0x0099BE70,
// ConvertPathToFullPath 0x00999110, CWideString::ConvertToCharString 0x0099B2C0, ~CWideString 0x0099B510, the map's
// find 0x006C31B0 / insert 0x006C3410, CCharString copy 0x0099EC30 / dtor 0x0099EAE0, operator new 0x00BFEA1A /
// delete 0x00BFE9BC, C3DSkeletalMorphTarget() 0x00A44E60, CCountedPointer::Reset 0x006C3AC0 (manifest label
// propagated from CCountedPointer<CIPhysicsFacesSegmentBase const>), PathExists 0x00999230,
// CDiskFileWin32(path, 1, 2) 0x0099AD80 with the inlined dtor of the landed 0x006F92A0 (vtable 0x0122D06C, Close
// 0x0099A920, ~CBaseClassNonCopyable 0x0099A300), GFLoadSkeletalMorphTargetFromFile 0x0088B0C0 (__fastcall).
// Layout: the resource manager's STLport map sits at +0x04 (header node +0, node key +0x10, value +0x14); the
// counted pointer is {Data, Info}, Info = {RefCount, __fastcall deleter, object}.
// Shape: the name is a const reference, so the full path CCharString is homed in its dead argument slot (and
// destroyed there at the end); the four CWideString temporaries of the path expression die in reverse order.
// A miss inserts pair(path, null pointer) (the pair's inline counted-pointer dtor releases through Info, then
// clears both words) and finds again; an empty pointer is reset to a new target that is loaded from the file
// when the path exists (a scoped CDiskFileWin32).
class CWideString { public:
    void* PStringData;
    ~CWideString();                                // 0x0099B510
    class CCharString ConvertToCharString() const;   // 0x0099B2C0
};
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};
CWideString __fastcall GetBonesDirectory();        // 0x0041A290 (L"Data\\Bones\\")
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);   // 0x00999110

public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
};         // 0x00999110
                           // 0x00999230
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                    // vtable 0x0122D06C, 0x1c bytes
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
class C3DSkeletalMorphTarget { public:
    unsigned char _pad[0x10];
    C3DSkeletalMorphTarget();                      // 0x00A44E60
};
void __fastcall GFLoadSkeletalMorphTargetFromFile(CDiskFileWin32* file, C3DSkeletalMorphTarget* target);   // 0x0088B0C0
class CCPPointerInfo { public:
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
class CSkeletalMorphResourceManager { public:
    virtual void v00();
    CMorphMap Morphs;                              // +0x04
    CMorphTargetPointer& GetMorphTarget(const CCharString& name);
};

CMorphTargetPointer& CSkeletalMorphResourceManager::GetMorphTarget(const CCharString& name)
{
    CCharString path = CAFile::ConvertPathToFullPath(GetBonesDirectory() + name.ToWideString()).ConvertToCharString();
    CMorphNode* node = Morphs.find(path);
    if (node == Morphs.end()) {
        Morphs.insert(CMorphPair(path));
        node = Morphs.find(path);
    }
    CMorphTargetPointer& target = node->Value;
    if (target.Data == 0) {
        target.Reset(new C3DSkeletalMorphTarget);
        if (CAFile::PathExists(path.ToWideString())) {
            CDiskFileWin32 file(path.ToWideString(), FILE_ACCESS_READ, 2);
            GFLoadSkeletalMorphTargetFromFile(&file, target.Data);
        }
    }
    return target;
}