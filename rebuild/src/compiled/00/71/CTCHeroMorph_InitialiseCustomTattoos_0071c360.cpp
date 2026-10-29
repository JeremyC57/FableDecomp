// CTCHeroMorph::InitialiseCustomTattoos() -- copy the five custom tattoo cards' tattoo files (face, chest, back, arms, legs) into the profile's save directory, then copy the custom tattoo tutorial text there when it is missing and the disk has room
// Leaf: ?InitialiseCustomTattoos@CTCHeroMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C360, 1068 bytes, `ret`.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetPDefFromInstantiationName 0x009ADA40 (hidden result),
//   CDefClassBase::GetSubDef<CTattooDef> 0x005B1EFC (manifest label is a propagated instantiation name), CTattooDef::
//   CopyCustomTattooToUserDir 0x0071B840; the save/custom-tattoo directories 0x0041A650 / 0x0041A6A0, operator+ 0x0099BF30,
//   PathExists 0x00999230, IsRoomForFile 0x00999AB0, CDiskFileWin32 0x0099AD80 (landed CopyCustomTattooToUserDir model),
//   CopyFileW 0x00998E70.
// Data: L"Custom Tattoo Tutorial.txt" 0x01265BC0.
#include <stddef.h>
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);            // 0x0099BF30
class CAFile {
public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
};        // 0x00999230
bool __fastcall IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir_0041a650();               // 0x0041A650
CWideString __fastcall GetCustomTattooDir();                // 0x0041A6A0
}
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                               // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                             // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                                   // 0x0099A920 (slot 2)
    long        AccessType;                                 // +0x04
    long        Position;                                   // +0x08
    long        Length;                                     // +0x0c
    void*       WinFileHandle;                              // +0x10
    bool        Opened;                                     // +0x14
    CWideString Pathname;                                   // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};
void __fastcall CopyFileW(CDiskFileWin32& from, CDiskFileWin32& to);   // 0x00998E70

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* s, long len = -1);              // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void AddRef() { ++m_count; }
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointerBase {                                    // untyped result of the instantiation-name lookup
    CDefBase* m_p;
    ~CDefPointerBase()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
class CTattooDef;
template<class T> class CDefPointer { public:
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointerBase& o) : m_p(o.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return (T*)m_p; }
    operator bool() const { return m_p != 0; }
};
class CTattooDef { public:
    bool CopyCustomTattooToUserDir() const;                 // 0x0071B840
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTattooDef>* out) const;     // 0x005B1EFC
};
void* FindSubDef(const CDefClassBase* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef (retail's body is out of line; landed AddTattoo 0x0071DA70 model): it reads the old out pointer and
// stores the new one like CDefPointer::operator=, so VC7.1 knows the out pointer's address does not escape the call.
bool CDefClassBase::GetSubDef(CDefPointer<CTattooDef>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    CDefPointerBase GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA40
};
class CTCHeroMorph { public:
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0 (CTCBase)
    void InitialiseCustomTattoos();
};

void CTCHeroMorph::InitialiseCustomTattoos()
{
    CDefPointer<CDefClassBase> face = GetDefinitionManager()->GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_FACE_CUSTOM_01");
    if (face) {
        CDefPointer<CTattooDef> tattoo;
        face->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> chest = GetDefinitionManager()->GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01");
    if (chest) {
        CDefPointer<CTattooDef> tattoo;
        chest->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> back = GetDefinitionManager()->GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_BACK_CUSTOM_01");
    if (back) {
        CDefPointer<CTattooDef> tattoo;
        back->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> arms = GetDefinitionManager()->GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01");
    if (arms) {
        CDefPointer<CTattooDef> tattoo;
        arms->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> legs = GetDefinitionManager()->GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01");
    if (legs) {
        CDefPointer<CTattooDef> tattoo;
        legs->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CWideString destination = NResourceDirectoryNames::GetSaveDir_0041a650() + L"Custom Tattoo Tutorial.txt";
    if (!CAFile::PathExists(destination)) {
        CWideString source = NResourceDirectoryNames::GetCustomTattooDir() + L"Custom Tattoo Tutorial.txt";
        if (CAFile::PathExists(source)) {
            CDiskFileWin32 from(source, FILE_ACCESS_READ, 2);
            if (IsRoomForFile(from.Length, destination)) {
                CDiskFileWin32 to(destination, FILE_ACCESS_WRITE, 2);
                CopyFileW(from, to);
            }
        }
    }
}