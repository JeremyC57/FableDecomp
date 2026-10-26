// CWorldMap::GetLevelDimensions(const CCharString&) -- open the level file (from the WAD bank when WAD loading is on, else from disk) and return its world box through the file overload
// Leaf: ?GetLevelDimensions@CWorldMap@@QAE?AVC2DBoxI@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00506E30, 247 bytes (`ret 8`, C2DBoxI returned through the hidden pointer).
// Callees: CCharString::ToWideString @ 0x0099E480, ~CWideString @ 0x0099B510, operator new @ 0x00BFEA1A, CWADFile ctor
//          @ 0x00A39D80, CDiskFileWin32 ctor @ 0x0099AD80, CCountedPointer<CAFile>::Reset @ 0x0050F550 (same-TU stand-in),
//          CWorldMap::GetLevelDimensions(CAFile*) @ 0x00506BB0, operator delete @ 0x00BFE9BC.
// Layout (Ego_r.pdb): CWorldMap LevelWADBank (CCountedPointer<CWADBank>) +0xA0, UseWADFiles +0xA8; CCountedPointer = {Data, Info},
//          count block {RefCount, DeleteFunc, Data}.
// Shape: the local counted pointer is adopted through Reset; because Reset is a same-TU stand-in that only writes the two
//        members, the pointer's address does not escape and the final inline release is retail's `dec dword ptr [esi] ; jne`
//        (an extern Reset makes VC7.1 reload the count after the decrement, `dec [eax] ; cmp [esi],0`).
void* operator new(unsigned int size);                 // 0x00BFEA1A
void operator delete(void* p);                         // 0x00BFE9BC
class CWideString { public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
class CCharString { public:
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CAFile { public: virtual void v00(); };
class CWADBank;
class CWADFile : public CAFile {
public:
    CWADFile(CWADBank* bank, const CWideString& name, FileAccessType access, unsigned long flags);   // 0x00A39D80
    unsigned char _body[0xc];
};
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);         // 0x0099AD80
    unsigned char _body[0x18];
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info) {
            if (--info->RefCount == 0) {
                info->DeleteFunc(info->Data);
                delete info;
            }
        }
    }
    void Reset(T* object);                             // 0x0050F550
    T* Get() const { return Data; }
    operator T*() const { return Data; }
};
CCPPointerInfo* AllocPointerInfo_00506e30(void* object);   // stand-in callee of the stand-in body below
// Same-TU stand-in for CCountedPointer<CAFile>::Reset @ 0x0050F550 (a template member compiled in retail's TU): releases the
// old count block and adopts the object with a fresh one; it only writes the two members, so VC7.1 knows the pointer's
// address does not escape the call.
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CCountedPointer<T>::Reset(T* object)
{
    CCPPointerInfo* info = Info;
    if (info) {
        if (--info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
    Data = object;
    Info = AllocPointerInfo_00506e30(object);
}
#pragma auto_inline(on)
class CWorldMap {
public:
    unsigned char _pad00[0xa0];
    CCountedPointer<CWADBank> LevelWADBank;              // +0xA0
    bool          UseWADFiles;                         // +0xA8
    C2DBoxI GetLevelDimensions(CAFile* file);          // 0x00506BB0 (landed, PDB takes CAFile&)
    C2DBoxI GetLevelDimensions(const CCharString& levelName);
};
C2DBoxI CWorldMap::GetLevelDimensions(const CCharString& levelName)
{
    CCountedPointer<CAFile> file;
    if (UseWADFiles)
        file.Reset(new CWADFile(LevelWADBank, levelName.ToWideString(), FILE_ACCESS_READ, 2));
    else
        file.Reset(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    return GetLevelDimensions(file.Data);
}