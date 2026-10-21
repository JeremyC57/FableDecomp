// CBrainOutput::Clear() -- no output types, no look flags, drop the action
// Leaf: ?Clear@CBrainOutput@@QAEXXZ
// Retail: Fable.exe @ 0x00835330, 54 bytes, plain `ret`, no direct reference. Catalog label
// NUISystem::CFrontEndScreen::~CFrontEndScreen is wrong: there is no vptr store, and the body is byte-for-byte the tail
// of the CBrainOutput constructor (0x00835620, after its member setup): Types (+0) = 0, LookFlags (+4) = 0, then the
// inline CCountedPointer<CCreatureActionBase>::Reset of PAction (+0x20): when Info (+0x24) is set, --RefCount; at zero
// call Info->DeleteFunc(Info->Data) (__fastcall, the DeleteData statics) and operator delete(Info) (0x00BFE9BC); then
// Info = 0, Data = 0. The name is descriptive (Ego_r.pdb lists only CBrainOutput::SetAsLookAtThing out of line).
// Layout: CBrainOutput (Ego_r.pdb, 0x48): Types +0, LookFlags +4, CNavigationInput +8, PAction +0x20 (Data +0x20,
// CCPPointerInfo* Info +0x24 {RefCount +0, DeleteFunc +4, Data +8}); no engine/ header.
// Shape: /O2 (RefCount is re-read through Info after the decrement).
class CCreatureActionBase;
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    void Reset()
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
};
class CBrainOutput {
public:
    unsigned long                        Types;          // +0x00
    long                                 LookFlags;      // +0x04
    unsigned char                        NavigationInput[0x18];  // +0x08 CNavigationInput
    CCountedPointer<CCreatureActionBase> PAction;        // +0x20
    void Clear();
};
void CBrainOutput::Clear()
{
    Types = 0;
    LookFlags = 0;
    PAction.Reset();
}