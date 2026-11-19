// CThingPhysicalSwitch::GetCurrentGraphic() const -- the def's triggered or untriggered CEngineGraphic for the switch's current state
// Grade: DIFFER(145v125) -- 34 difflines; engine/CDefPointer.h verify copy (+ proposed ENullDef ctor) and the PDB callee CThing::PeekDefinitionManager() const 0x004C7990 (Ego_r ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ). Residue: the result address (lea esi,[def+82h]) is formed before the release so the release takes the load/dec/store form; retail keeps the def in esi (dec [esi+4]; jne) and forms the address after. Tried: per-arm scoped defs (159v125), named def local (145v125). Next: a dtor form that leaves the def live in esi.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DB60, length 128 bytes (plain `ret`, result in eax).
// Callees: the global definition-manager getter @ 0x004C7990 (`mov eax,[0x013B8A1C] ; mov eax,[eax+20h]`,
// manifest label CViewBase::GetDisplayEngine is a propagated twin), CDefinitionManager::GetDef
// <CThingPhysicalSwitchDef>(long, CDefPointer<const CThingPhysicalSwitchDef>*) @ 0x0083E0D0 (`ret 8`).
// Layout (Ego_r.pdb vs retail): CThing::DefGlobalIndex PDB +0x9c -> retail +0x8c; CThingPhysicalSwitch::
// Triggered PDB +0xe8 -> retail +0xd0 (the 0x18 CThing/CThingPhysical shrink, see the landed 0x0083D940);
// CThingPhysicalSwitchDef UntriggeredGraphic PDB +0x78 -> retail +0x74, TriggeredGraphic PDB +0x86 -> +0x82.
// Shape: one CDefPointer declared before the test (its zero store is hoisted above the branch), each arm
// fetches the def (arguments pushed before the manager getter is called), then the pointer's dtor releases
// Leaf: ?GetCurrentGraphic@CThingPhysicalSwitch
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CEngineGraphic { public: unsigned char _data[0x0e]; };
class CThingPhysicalSwitchDef : public CDefClassBase {
public:
    unsigned char _pad[0x74 - sizeof(CDefClassBase)];
    CEngineGraphic UntriggeredGraphic;                 // +0x74 [PDB +0x78]
    CEngineGraphic TriggeredGraphic;                   // +0x82 [PDB +0x86]
};
class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0083E0D0 for the switch def
};
class CGameDefinitionManager : public CDefinitionManager { };
class CThing {
public:
    unsigned char _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;                     // +0x8c CThing [PDB +0x9c]
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};
class CThingPhysicalSwitch : public CThing {
public:
    unsigned char _pad_0x8e[0xd0 - 0x8e];
    bool Triggered;                                    // +0xd0 [PDB +0xe8]
    const CEngineGraphic* GetCurrentGraphic() const;
};

const CEngineGraphic* CThingPhysicalSwitch::GetCurrentGraphic() const
{
    CDefPointer<const CThingPhysicalSwitchDef> def(NULL_DEF);
    if (Triggered) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return &def->TriggeredGraphic;
    }
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return &def->UntriggeredGraphic;
}
