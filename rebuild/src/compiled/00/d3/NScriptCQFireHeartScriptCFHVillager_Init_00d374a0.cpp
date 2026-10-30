// NScript::CQ_FireHeartScript::CFH_Villager::Init() -- take the villager's id from its data string, register Me in the parent's Villagers slot, clear its ShouldVillagerSpeak flag and make it unpushable
// Leaf: ?Init@CFH_Villager@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D374A0, 135 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012CD01C, which the entity Alloc 0x00D373D0 (`NScript::CQ_FireHeartScript::CFH_Villager::Alloc`) stores; ?Init@CFH_Villager@CQ_FireHeartScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d374a0" is a gap-scan placeholder.
// Callees: CScriptThing::GetDataString (slot 3), GFCharStringToInt 0x0099E7F0 (Ego_r.pdb ?GFCharStringToInt@@YIJABVCCharString@@@Z), ~CCharString 0x0099EAE0, CCountedPointer<CScriptThing>::operator= 0x00704580 (the inline CScriptThing::operator=); interface slot 844 SetIsPushableByHero (by value).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CFH_Villager: MyID +0x1c. Parent: STLport containers are smaller than Ego_r's Dinkumware ones (map 0x0C, vector 0x0C, vector<bool> a plain 0x0C byte vector), so ShouldVillagerSpeak is +0xec (Ego_r +0x128) and Villagers +0x110 (Ego_r +0x160).
class CScriptThing;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6 layout (0x0C): start, finish, end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
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
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript {
public:
    unsigned char _pad000[0xec];
    std::vector<bool> ShouldVillagerSpeak;              // +0xec (Ego_r.pdb +0x128)
    std::vector<bool> CommentGivenThisRound;            // +0xf8
    std::vector<CScriptThing> Prophets;                 // +0x104
    std::vector<CScriptThing> Villagers;                // +0x110 (Ego_r.pdb +0x160)
    class CFH_Villager;
};
class CQ_FireHeartScript::CFH_Villager : public CEntityScriptBase {
public:
    CQ_FireHeartScript*       ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      MyID;                       // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_FireHeartScript::CFH_Villager::Init()
{
    MyID = GFCharStringToInt(Me.GetDataString());
    ParentClass->Villagers[MyID] = Me;
    ParentClass->ShouldVillagerSpeak[MyID] = false;
    Game->SetIsPushableByHero(Me, false);
}