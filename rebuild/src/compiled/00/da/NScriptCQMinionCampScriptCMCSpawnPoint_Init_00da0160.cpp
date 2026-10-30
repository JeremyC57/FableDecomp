// NScript::CQ_MinionCampScript::CMC_SpawnPoint::Init() -- clear DoneIntroMinion and file Me under the parent's left (data string 0) or right (1) spawn points unless the thread is terminating
// Leaf: ?Init@CMC_SpawnPoint@CQ_MinionCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0160, 123 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D6D44, which the entity Alloc 0x00D9ED50 (`NScript::CQ_MinionCampScript::CMC_SpawnPoint::Alloc`) stores; ?Init@CMC_SpawnPoint@CQ_MinionCampScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00da0160" is a gap-scan placeholder.
// Callees: CScriptThing::GetDataString (slot 3, +0x0c, by value), GFCharStringToInt(const CCharString&) 0x0099E7F0 (__fastcall), ~CCharString 0x0099EAE0, CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, std::list<CScriptThing>::push_back 0x00DA01E0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CMC_SpawnPoint: DoneIntroMinion +0x1c; parent LeftSpawnPoints +0x90, RightSpawnPoints +0x94 (STLport lists, 4 bytes each; Ego_r.pdb +0xa4/+0xb0).
// Shape: each arm is guarded by `if (IsActiveThreadTerminating()) return;`; the fall-through arm keeps only the call.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
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
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..9, 11..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10 (+0x28)
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase;
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class T, class A = allocator<T> > class list {   // STLport 4.6: 4 bytes (the header-node pointer)
public:
    void* _M_node;
    void push_back(const T& x);                           // 0x00DA01E0 for list<CScriptThing>
};
}
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes
public:
    virtual ~CEntityScriptBase();
private:
    virtual void Init();                            // slot 2 (+0x8)
public:
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript {
public:
    class CMC_SpawnPoint;
    unsigned char _pad000[0x90];
    std::list<CScriptThing> LeftSpawnPoints;            // +0x90 (Ego_r.pdb +0xa4)
    std::list<CScriptThing> RightSpawnPoints;           // +0x94
};
class CQ_MinionCampScript::CMC_SpawnPoint : public CEntityScriptBase {
public:
    CQ_MinionCampScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      DoneIntroMinion;            // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_MinionCampScript::CMC_SpawnPoint::Init()
{
    DoneIntroMinion = false;
    long side = GFCharStringToInt(Me.GetDataString());
    if (side == 0) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->LeftSpawnPoints.push_back(Me);
    } else if (side == 1) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->RightSpawnPoints.push_back(Me);
    } else {
        if (IsActiveThreadTerminating())
            return;
    }
}