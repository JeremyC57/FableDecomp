// CSpawnedFuncBase::RemovePredicate() -- release the PPredicate counted pointer (+0x2C): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?RemovePredicate@CSpawnedFuncBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f35a30, 49 bytes, thiscall, plain `ret`.
// Identity (proven): bound ?RemovePredicate@CSpawnedFuncBase@@QAEXXZ (Ego_r.pdb); Ego_r.pdb CSpawnedFuncBase::PPredicate (CCountedPointer<const NScript::CPredicateBase>, +0x2C); callers CScriptBase::UpdateSpawnedFunction 0x00CB7950 (twice) and CScriptBase::SetPredicate 0x00CB7920's tail path.
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
namespace NScript { class CPredicateBase; }
class CSpawnedFuncBase {
public:
    unsigned char _pad[0x2C];                         // (members not touched here)
    CCountedPointer<const NScript::CPredicateBase> PPredicate;                        // +0x2C
    void RemovePredicate();
};
void CSpawnedFuncBase::RemovePredicate()
{
    PPredicate.ReleaseData();
}