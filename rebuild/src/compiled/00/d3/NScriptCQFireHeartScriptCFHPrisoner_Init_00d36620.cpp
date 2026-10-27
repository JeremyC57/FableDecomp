// NScript::CQ_FireHeartScript::CFH_Prisoner::Init() -- the prisoner is not chosen; its cage number is its data string; the parent stops that prophet speaking and records the prisoner as that cage's prophet
// Leaf: ?Init@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D36620, 88 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CCF10, which the landed
//   NScript::CQ_FireHeartScript::CFH_Prisoner::Alloc 0x00D36540 stores (site 0x00D36576); Ego_r.pdb has ?Init@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CScriptThing::GetDataString (virtual slot 3, +0x0c, CCharString by value) 0x004AA900, GFCharStringToInt (__fastcall)
//   0x0099E7F0, ~CCharString 0x0099EAE0, CScriptThing::operator= inline over CCountedPointer<CScriptThing>::operator= 0x00704580.
// Layout: Ego_r.pdb CFH_Prisoner: ParentClass +0x14, PMasterData +0x18, Chosen +0x1c, MyCageNumber +0x20.
//   CQ_FireHeartScript (Ego_r - 0x14, then -4 per STLport map: CutsceneFlags +0x4c and csargs +0x58 are 0xC each; STLport
//   vector<bool> here is a plain byte vector of 0xC, so the bool vectors run StartingPattern +0xbc, ProphetsKilled +0xc8,
//   ProphetsReleased +0xd4, ShouldProphetSpeak +0xe0, ...; Prophets (vector<CScriptThing>) +0x104 -- both offsets confirmed by
//   this body: the byte store through [+0xe0] and the 12-byte stride through [+0x104]).
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass() { }
    virtual ~CBaseClass();
};
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
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};
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
    unsigned char _pad48[0xe0 - 0x48];
    std::vector<bool> ShouldProphetSpeak;               // +0xe0
    unsigned char _padec[0x104 - 0xec];
    std::vector<CScriptThing> Prophets;                 // +0x104
    class CFH_Prisoner;
};
class CQ_FireHeartScript::CFH_Prisoner : public CEntityScriptBase {
public:
    CQ_FireHeartScript*            ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
    bool                          Chosen;        // +0x1c
    long                          MyCageNumber;  // +0x20
private:
    virtual void Init();
};

void CQ_FireHeartScript::CFH_Prisoner::Init()
{
    Chosen = false;
    MyCageNumber = GFCharStringToInt(Me.GetDataString());
    ParentClass->ShouldProphetSpeak[MyCageNumber] = false;
    ParentClass->Prophets[MyCageNumber] = Me;
}
}