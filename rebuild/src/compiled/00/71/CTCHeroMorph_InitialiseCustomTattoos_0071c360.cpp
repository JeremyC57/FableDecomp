// CTCHeroMorph::InitialiseCustomTattoos() -- copy the five custom tattoo cards' tattoo files (face, chest, back, arms, legs) into the profile's save directory, then copy the custom tattoo tutorial text there when it is missing and the disk has room
// Leaf: ?InitialiseCustomTattoos@CTCHeroMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C360, 1068 bytes, `ret`.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetPDefFromInstantiationName 0x009ADA40 (hidden result),
//   CDefClassBase::GetSubDef<CTattooDef> 0x005B1EFC (manifest label is a propagated instantiation name), CTattooDef::
//   CopyCustomTattooToUserDir 0x0071B840; NResourceDirectoryNames::GetTattooUserDir 0x0041A650 / GetTattooDataDir 0x0041A6A0, operator+ 0x0099BF30,
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
namespace NResourceDirectoryNames {
CWideString __fastcall GetTattooUserDir();                  // 0x0041A650 (My Documents + L"My Games\\Fable\\" + L"Tattoos\\")
CWideString __fastcall GetTattooDataDir();                  // 0x0041A6A0 (L"Data\\Tattoos\\")
}
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                               // 0x0099A300
};
class CAFile : public CBaseClassNonCopyable {
public:
    static bool __fastcall PathExists(const CWideString&);                 // 0x00999230
    static bool __fastcall IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0 (Ego_r CAFile static)
    static void __fastcall CopyFileW(CAFile& from, CAFile& to);            // 0x00998E70 (Ego_r CAFile static)
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 : public CAFile { public:                             // vtable 0x0122D06C, 0x1c bytes
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
template<class T> class CDefPointer;
class CDefClassBase;
template<> class CDefPointer<const CDefClassBase> { public:   // the manager's by-value lookup result (released and cleared)
    CDefBase* m_p;
    ~CDefPointer()
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
    CDefPointer(const CDefPointer<const CDefClassBase>& o) : m_p(o.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return static_cast<T*>(m_p); }
    operator bool() const { return m_p != 0; }
};
class CDefClassBase : public CDefBase { public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005B1EFC GetSubDef<CTattooDef> (pushes "CTattooDef")
};
class CTattooDef : public CDefClassBase { public:
    bool CopyCustomTattooToUserDir() const;                 // 0x0071B840
};
void* FindSubDef(const CDefClassBase* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef (retail's body is out of line; landed AddTattoo 0x0071DA70 model): it reads the old out pointer and
// stores the new one like CDefPointer::operator=, so VC7.1 knows the out pointer's address does not escape the call.
template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CTattooDef>(CDefPointer<const CTattooDef>* out) const
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
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString& name) const;   // 0x009ADA40 (Ego_r QBE)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CThing;
class CTCBase { public:                                     // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCHeroMorph : public CTCBase {
protected:
    void InitialiseCustomTattoos() const;                   // Ego_r.pdb ?InitialiseCustomTattoos@CTCHeroMorph@@IBEXXZ
};

void CTCHeroMorph::InitialiseCustomTattoos() const
{
    CDefPointer<CDefClassBase> face = GetDefinitionManager().GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_FACE_CUSTOM_01");
    if (face) {
        CDefPointer<const CTattooDef> tattoo;
        face->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> chest = GetDefinitionManager().GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01");
    if (chest) {
        CDefPointer<const CTattooDef> tattoo;
        chest->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> back = GetDefinitionManager().GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_BACK_CUSTOM_01");
    if (back) {
        CDefPointer<const CTattooDef> tattoo;
        back->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> arms = GetDefinitionManager().GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01");
    if (arms) {
        CDefPointer<const CTattooDef> tattoo;
        arms->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CDefPointer<CDefClassBase> legs = GetDefinitionManager().GetPDefFromInstantiationName("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01");
    if (legs) {
        CDefPointer<const CTattooDef> tattoo;
        legs->GetSubDef(&tattoo);
        tattoo->CopyCustomTattooToUserDir();
    }
    CWideString destination = NResourceDirectoryNames::GetTattooUserDir() + L"Custom Tattoo Tutorial.txt";
    if (!CAFile::PathExists(destination)) {
        CWideString source = NResourceDirectoryNames::GetTattooDataDir() + L"Custom Tattoo Tutorial.txt";
        if (CAFile::PathExists(source)) {
            CDiskFileWin32 from(source, FILE_ACCESS_READ, 2);
            if (CAFile::IsRoomForFile(from.Length, destination)) {
                CDiskFileWin32 to(destination, FILE_ACCESS_WRITE, 2);
                CAFile::CopyFileW(from, to);
            }
        }
    }
}