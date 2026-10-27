// CThingCreatureBase::ClearCurrentAction() -- release the PCurrentAction counted pointer (+0x100): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006633a0, 64 bytes, thiscall, plain `ret`.
// Identity (probable): Ego_r.pdb ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ; Ego_r.pdb CThingCreatureBase::PCurrentAction (CCountedPointer<CCreatureActionBase>, PDB +0x11C, retail +0x100 after the STLport shrink); callers CCreatureActionBase::Update 0x006929A0 (tail jump on the owner when the action is done), ::EditUpdate 0x00692DB0, CGameScriptInterface::SheatheHeroWeapons 0x008916A0 and the ScriptedThing ClearAllActions 0x00903970 (after the queued-action clears).
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L028): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                        // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    void ReleaseData()                                    // Ego_r.pdb ?ReleaseData@?$CCountedPointer@..@@QAEXXZ (inline here)
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    ~CCountedPointer()                                    // the destructor clears Data before Info
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CCreatureActionBase;
class CThingCreatureBase {
public:
    unsigned char _pad[0x100];                         // (members not touched here)
    CCountedPointer<CCreatureActionBase> PCurrentAction;                        // +0x100
    void ClearCurrentAction();
};
void CThingCreatureBase::ClearCurrentAction()
{
    PCurrentAction.ReleaseData();
}