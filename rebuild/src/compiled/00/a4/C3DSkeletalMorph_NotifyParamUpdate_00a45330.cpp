// C3DSkeletalMorph::NotifyParamUpdate() -- release the CombinedMorph counted pointer (+0x10): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?NotifyParamUpdate@C3DSkeletalMorph@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a45330, 49 bytes, thiscall, plain `ret`.
// Identity (probable): Ego_r.pdb C3DSkeletalMorph::CombinedMorph (CCountedPointer<C3DSkeletalMorphTarget>, PDB +0x14, retail +0x10 after the 12-byte MorphParameters CArray); of C3DSkeletalMorph's two Ego_r.pdb `void ()` members (NotifyParamUpdate, Reset) this one only drops the cached combined morph, and its callers CTCSkeletalMorph::ForceSkeletalMorph 0x007864C0 / CalculateSkeletalMorph 0x00786700 (landed) call it right after adding morph parameters; bound only as the stand-ins ApplyParameters / ResetCombinedMorph.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L108): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
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
class C3DSkeletalMorphTarget;
class C3DSkeletalMorph {
public:
    unsigned char _pad[0x10];                         // (members not touched here)
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;                        // +0x10
    void NotifyParamUpdate();
};
void C3DSkeletalMorph::NotifyParamUpdate()
{
    CombinedMorph.ReleaseData();
}