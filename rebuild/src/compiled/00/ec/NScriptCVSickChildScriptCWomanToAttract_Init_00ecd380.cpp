// NScript::CV_SickChildScript::CWomanToAttract::Init() -- GivenObject = false, TimesMadeLaugh = 0
// Leaf: ?Init@CWomanToAttract@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD380, 9 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F2404, stored by the entity Alloc 0x00ECD2D0 (`NScript::CV_SickChildScript::CWomanToAttract::Alloc`); ?Init@CWomanToAttract@CV_SickChildScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ecd380" is a gap-scan placeholder.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets: GivenObject +0x1c, TimesMadeLaugh +0x20.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript {
public:
    class CWomanToAttract;
};
class CV_SickChildScript::CWomanToAttract : public CEntityScriptBase {
public:
    CV_SickChildScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    GivenObject;                // +0x1c
    long                    TimesMadeLaugh;             // +0x20
private:
    virtual void Init();
};
}

void NScript::CV_SickChildScript::CWomanToAttract::Init()
{
    GivenObject = false;
    TimesMadeLaugh = 0;
}