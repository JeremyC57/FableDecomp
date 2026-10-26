// CTCWeapon::GetCreatedShotDefGlobalIndex() -- the weapon def's created-shot def index, else look up a default def by name
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA750, 126 bytes (+2 int3). __fastcall, returns long.
// Callees: CCharString(const char*, long) 0x0099EBF0; ~CCharString 0x0099EAE0; CTCBase::GetDefinitionManager() 0x006869D0;
//          CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410; def release via vtable slot 1.
// Shape: GetWeaponDef() returns the CDefPointer<CWeaponDef const> at +0x0C BY VALUE (add-ref null-tested, release not) and is
//        called twice; the `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne` release only appears when the def's own inline
//        Release() member does the decrement (`--Object->RefCount` written in the CDefPointer dtor gives `dec eax` before the
//        `[ecx+0xa0]` load). The name string "SHOT_CROSSBOW_BOLT" (0x0125556C) is a named CCharString local (`lea eax ; push eax`).
// Layout: CTCWeapon (engine header): ParentThing +0x04, WeaponDef +0x0C; CWeaponDef: RefCount +0x04, created-shot index +0xA0.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CWeaponDef { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad_0x08[0xa0 - 0x08];
    long CreatedShotDefIndex;                    // +0xa0
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CThing;
class CTCWeapon { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    CDefPointer<CWeaponDef> WeaponDef;           // +0x0c
    const CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    __forceinline CDefPointer<CWeaponDef> GetWeaponDef() const { return WeaponDef; }
    long GetCreatedShotDefGlobalIndex() const;
};
long CTCWeapon::GetCreatedShotDefGlobalIndex() const
{
    if (GetWeaponDef()->CreatedShotDefIndex)
        return GetWeaponDef()->CreatedShotDefIndex;
    CCharString name("SHOT_CROSSBOW_BOLT", -1);
    return GetDefinitionManager()->GetDefGlobalIndexFromName(name);
}