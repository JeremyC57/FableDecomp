// CAIStateGroup_JackOfBladesBase::SetupArena() -- find the "FrescoCentre" thing within 50m (rough area) of the creature, latch it as the arena centre, and switch the camera manager to the CAMERA_MANAGER_SET_DEFAULT camera set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972830, length 352 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / CCharString(const CCharString&) @ 0x0099EC30 / ~CCharString
//          @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools
//          @ 0x008FD000, CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex, CThing> @ 0x007FD110,
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable 0x01238C6C), CAIStateGroupBase::GetPWorld
//          @ 0x008FCFC0, the world's +0x34 object virtual slot 9 (+0x24, camera manager out pointer),
//          CAIStateGroupBase::GetPDefinitionManager @ 0x008FCFB0 (manifest "_Cons_val" is propagated),
//          CDefinitionManager::GetDef<CCameraManagerSetDef> @ 0x004AB990, CGameCameraManager::SetCameraManagerSetDef
//          @ 0x0069AC90; the def's virtual slot 1 (destroy on the last release).
// Shape: the search is one full expression (arguments right to left: the filter temporary built from the name
//          temporary, then the 100m box around the truncated creature XY); the def pointer's release goes through the
//          ref-count base's DecRef/GetRefCount accessors.
// Layout: engine/CAIStateGroup_JackOfBladesBase.h (PArenaCentre +0x1c); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_JackOfBladesBase.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class C3DVector { public:
    float X, Y, Z;
};
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(const C3DVector& centre, long half)   // the square of half-width `half` around the truncated XY
    {
        long x = (long)centre.X;
        long y = (long)centre.Y;
        MinX = x - half;
        MinY = y - half;
        MaxX = x + half;
        MaxY = y + half;
    }
};

class CThing { public:
    const C3DVector& GetPosition();             // 0x004C73D0
};
class CThingFilter_HasDefGlobalIndex { public:
    CCharString DefName;
    CThingFilter_HasDefGlobalIndex(const CCharString& name) : DefName(name) { }
};
class CThingSearchTools { public:
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter,
                                   CThing** result);   // 0x007FD110
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CCameraManagerSetDef : public CRefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* def);   // 0x004AB990
};
class CGameCameraManager { public:
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def);       // 0x0069AC90
};
struct CCameraHost {                            // the world's +0x34 object
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char _pad[0x34];
    CCameraHost* PCameraHost;                   // +0x34
    CCameraHost* GetPCameraHost() { return PCameraHost; }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupJackOfBladesBaseMethods : CAIStateGroup_JackOfBladesBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointer<CThing>& ArenaCentre() { return *(CIntelligentPointer<CThing>*)PArenaCentre; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CWorld* GetPWorld();                        // 0x008FCFC0
    CDefinitionManager* GetPDefinitionManager();   // 0x008FCFB0
    void SetupArena();
};

void CAIStateGroupJackOfBladesBaseMethods::SetupArena()
{
    {
        CThing* centre = 0;
        PeekThingSearchTools()->GetPFirstThingInRoughArea(
            C2DBoxI(GetCreature()->GetPosition(), 50),
            CThingFilter_HasDefGlobalIndex("FrescoCentre"), &centre);
        ArenaCentre() = CIntelligentPointer<CThing>(centre);
    }
    CGameCameraManager* camera;
    if (GetPWorld()->GetPCameraHost()->GetCameraManager(&camera)) {
        CDefPointer<CCameraManagerSetDef> def;
        GetPDefinitionManager()->GetDef("CAMERA_MANAGER_SET_DEFAULT", &def);
        camera->SetCameraManagerSetDef(def);
    }
}