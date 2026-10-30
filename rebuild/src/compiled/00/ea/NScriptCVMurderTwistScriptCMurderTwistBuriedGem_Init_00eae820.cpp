// NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::Init() -- unless the murder-twist event is 7 the buried gem is removed; the gem joins the magic-rune rumble list
// Leaf: ?Init@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAE820, 103 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EF7C8, which the landed
//   NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::Alloc 0x00EAD730 stores (site 0x00EAD766); Ego_r.pdb has ?Init@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMurderTwistBuriedGem (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slot 108 RemoveThing; std::vector<CScriptThing>::_M_insert_overflow 0x008AD5F0 (push_back inline:
//   the in-place implicit copy stores only CScriptThing's vtable 0x01238C8C, then add-refs PImp).
class CCharString;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };            // STLport type_traits tag (namespace std)
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x008AD5F0
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    std::vector<CScriptThing> MagicRuneRumbleList;      // +0x54
    class CMurderTwistBuriedGem;
};
class CV_MurderTwistScript::CMurderTwistBuriedGem : public CEntityScriptBase {
public:
    CV_MurderTwistScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Init();
};

void CV_MurderTwistScript::CMurderTwistBuriedGem::Init()
{
    if (ParentClass->MurderTwistEvent != 7)
        Game->RemoveThing(Me, false, false);
    ParentClass->MagicRuneRumbleList.push_back(Me);
}
}