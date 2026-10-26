// CTCBoastUI::GlobalUpdate(long) -- per-frame update of the boast UI: a quitting UI deletes its menu components,
// closes the front end, clears the global boast list and drops the player's boast mode; a requested activation
// either snaps the camera to the "BoastingCamera" point (when active and not already in a null camera mode) or opens
// the UI when allowed
// Retail 0x0056BE00, 0x23E bytes, `ret 4`. Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall; virtual
// slots +0x18, +0x74, +0x94), CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::RemoveMode 0x00634190, CCharString(const char*, int) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0,
// CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName,CThing> 0x0056D050, CGameDefinitionManager::Get
// 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, GetDef<CCameraModeDef> 0x0056D0C0, operator
// new 0x00BFEA1A, NCameraModes::CSnapToPoint ctor 0x0056E330, CCountedPointer<CBase>(T*) 0x0056C880 and dtor 0x0056C8D0,
// CGameCameraManager::PushCameraMode 0x006987A0, memmove 0x00BFEAE6, free 0x00BFEA14, CTCBoastUI::Activate 0x0056AD40.
// Layout: retail CTCBoastUI with the 12-byte STLport map/vector (Active +0x18, Components +0x1c, Quitting +0x28,
// AllowedToOpen +0x29, ActivationRequested +0x2a), as the landed 0x0056AC10 / 0x0056A880 model it.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
extern "C" void __cdecl free(void* p);                                        // 0x00BFEA14
void* operator new(unsigned int size);                                        // 0x00BFEA1A

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, int length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CThing;
class CThingFilter_HasScriptName {
public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) {}
};
class CThingSearchTools {
public:
    template <class F, class T> bool GetPFirstThing(const F& filter, T** out) const;   // 0x0056D050
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
class CCameraModeDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer() { if (Data) Data->ReleaseTemporary(); }
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);                   // 0x009AD410
    bool GetDef(long globalIndex, CDefPointer<CCameraModeDef>* out);         // 0x0056D0C0
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CGameCameraManager;
struct SCountInfo { long Count; };
template <class T> class CCountedPointer { public:
    T* Data;
    SCountInfo* Info;
    CCountedPointer(T* data);                          // 0x0056C880
    CCountedPointer(const CCountedPointer& other)
    {
        SCountInfo* info = other.Info;
        Data = other.Data;
        Info = info;
        if (info)
            ++info->Count;
    }
    ~CCountedPointer();                                // 0x0056C8D0
};
namespace NCameraModes {
    class CBase { public:
        virtual ~CBase();
        virtual void v01();
        virtual long GetModeType();                    // slot 2
    };
    struct CSnapToPoint : CBase {
        CSnapToPoint(CGameCameraManager* manager, const CDefPointer<CCameraModeDef>& def, CThing* point);   // 0x0056E330
        unsigned char _body[0xD0 - 4];
    };
}
class CGameCameraManager {
public:
    unsigned char _pad00[0x98];
    NCameraModes::CBase* CurrentMode;                  // +0x98
    void PushCameraMode(CCountedPointer<NCameraModes::CBase> mode);   // 0x006987A0
};
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;              // +0x20
    unsigned char _pad24[0x34 - 0x24];
    CGameCameraManager* CameraManager;                 // +0x34
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
class CEngine { public:
    unsigned char _pad00[0x24];
    CWorld* World;                                     // +0x24
};
extern CEngine* g_pEngine;                             // 0x013B86A0

struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05)
    virtual void Close();                              // +0x18
    VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22)
    VF(23) VF(24) VF(25) VF(26) VF(27) VF(28)
    virtual void ClearMenus();                         // +0x74
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36)
    virtual void ResetInput();                         // +0x94
};

struct SListNode { SListNode* Next; SListNode* Prev; };
extern SListNode* g_BoastList;                         // 0x013B8A98 (STLport list head node)

struct CComponent { virtual ~CComponent(); };
class CPlayer { public: void RemoveMode(long mode); };        // 0x00634190
class CPlayerManager { public: CPlayer* GetMainPlayer(); };   // 0x00449970

struct CComponentVector {
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CComponent*& operator[](unsigned long n) { return _M_start[n]; }
    CComponent** begin() { return _M_start; }
    CComponent** end() { return _M_finish; }
    static CComponent** copy(CComponent** first, CComponent** last, CComponent** result)
    {
        if (first == last)
            return result;
        unsigned int n = (char*)last - (char*)first;
        return (CComponent**)((char*)memmove(result, first, n) + n);
    }
    CComponent** erase(CComponent** first, CComponent** last)
    {
        CComponent** i = copy(last, end(), first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCBoastUI : public CTCBase { public:
    void*            __vftable;                        // +0x00
    CThing*          ParentThing;                      // +0x04
    unsigned char    _pad08[0x18 - 0x08];              // CTCBase flags, BoastGroups (12-byte map)
    bool             Active;                           // +0x18
    unsigned char    _pad19[3];
    CComponentVector Components;                       // +0x1c
    bool             Quitting;                         // +0x28
    bool             AllowedToOpen;                    // +0x29
    bool             ActivationRequested;              // +0x2a
    bool             ActivateLater;                    // +0x2b
    void Activate();                                   // 0x0056AD40
    void GlobalUpdate(long);
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
template <> bool CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing>(const CThingFilter_HasScriptName& filter, CThing** out) const
{
    *out = (CThing*)StandInSink((int)this, *(const int*)&filter);
    return *out != 0;
}
template <> CCountedPointer<NCameraModes::CBase>::CCountedPointer(NCameraModes::CBase* data)
{
    Data = data;
    Info = (SCountInfo*)StandInSink((int)data, 0);
}
template <> CCountedPointer<NCameraModes::CBase>::~CCountedPointer()
{
    StandInSink((int)Data, (int)Info);
}
#pragma auto_inline(on)
void CTCBoastUI::GlobalUpdate(long)
{
    if (Quitting) {
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
        CFrontEndManager::GetInstance()->Close();
        CFrontEndManager::GetInstance()->ClearMenus();
        SListNode* node = g_BoastList->Next;
        while (node != g_BoastList) {
            SListNode* dead = node;
            node = node->Next;
            free(dead);
        }
        g_BoastList->Next = g_BoastList;
        g_BoastList->Prev = g_BoastList;
        CFrontEndManager::GetInstance()->ResetInput();
        Active = false;
        Quitting = false;
        ActivationRequested = false;
        GetPlayerManager().GetMainPlayer()->RemoveMode(0x20);
    }
    if (ActivationRequested) {
        if (Active) {
            CGameCameraManager* camera = g_pEngine->World->CameraManager;
            if (camera->CurrentMode->GetModeType() != 2) {
                CThing* point = 0;
                if (g_pEngine->World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("BoastingCamera"), &point)) {
                    CDefPointer<CCameraModeDef> def;
                    CGameDefinitionManager::Get().GetDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CAMERA_MODE_MODE_NULL"), &def);
                    CCountedPointer<NCameraModes::CBase> mode(new NCameraModes::CSnapToPoint(camera, def, point));
                    camera->PushCameraMode(mode);
                }
            }
        } else if (AllowedToOpen) {
            Activate();
        }
    }
}