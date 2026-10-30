// NScript::CQ_FireHeartScript::CProphetCage::Init() -- the prophet cage has not been activated; its id is its data string
// Leaf: ?Init@CProphetCage@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D36730, 44 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CCF2C, which the landed
//   NScript::CQ_FireHeartScript::CProphetCage::Alloc 0x00D36680 stores (site 0x00D366B6); Ego_r.pdb has ?Init@CProphetCage@CQ_FireHeartScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CScriptThing::GetDataString (virtual slot 3, +0x0c, CCharString by value) 0x004AA900, GFCharStringToInt (__fastcall)
//   0x0099E7F0, ~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CProphetCage: ParentClass +0x14, PMasterData +0x18, IWasActivated +0x1c, MyID +0x20.
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 for T = CScriptThing
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual const CCharString& GetName() const;         // slot 1 0x004AA8B0
    virtual CCharString GetDefName() const;             // slot 2 0x004AA8D0
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c) 0x004AA900
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6 _Vector_base: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
}
class CGameScriptInterfaceBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CProphetCage;
};
class CQ_FireHeartScript::CProphetCage : public CEntityScriptBase {
public:
    CQ_FireHeartScript*            ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
    bool                          IWasActivated; // +0x1c
    long                          MyID;          // +0x20
private:
    virtual void Init();
};

void CQ_FireHeartScript::CProphetCage::Init()
{
    IWasActivated = false;
    MyID = GFCharStringToInt(Me.GetDataString());
}
}