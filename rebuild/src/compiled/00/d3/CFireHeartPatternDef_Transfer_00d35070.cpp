// CFireHeartPatternDef::Transfer(CPersistContext& context) -- persist the def's PadActive flags under the key "PadActive"
// Leaf: ?Transfer@CFireHeartPatternDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D35070, 21 bytes, thiscall, `ret 4` (virtual slot 18 of the def vtable).
// Identity (proven): vtable 0x012CCC50 (no RTTI) is the one CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize
//   0x00F342B0 and the def's constructor 0x00F33DC0 store; Ego_r.pdb CFireHeartPatternDef has exactly Transfer, Copy and
//   GetSizeofClass beyond CDefClassBase, and its one member is vector<bool> PadActive (Ego_r +0x28). Slot 18 is the keyed
//   PadActive persist (Transfer), slot 19 the member-wise copy from a CDefClassBase* (Copy), slot 20 0x00D34EF0 GetSizeofClass.
// Layout: CDefClassBase is 0x28 in retail (Ego_r 0x25 rounded up); PadActive +0x28.
// Model: the shared FABLE_RETAIL_DEF_CLASS_BASE CDefClassBase (vtable 0x01230BCC, 25 slots).
// Callees: CPersistContext::TransferVector<bool>(const char*, vector<bool>&) 0x00CDCF80 (`ret 8`; Ego_r.pdb
//   ??$TransferVector@_N@CPersistContext@@...; the manifest label Transfer<signed char> is propagated); key 0x012CCCB4.
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6 (no partial specialisation: vector<bool> is a byte vector, 0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& other);             // 0x00462E78 for vector<bool>
};
}
class CPersistContext {
public:
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                      // retail 0x28: CDefPointeeBase/CResource chain, PDefManager +0x1c, GlobalIndex +0x20, flags +0x24
class CFireHeartPatternDef : public CDefClassBase {     // vtable 0x012CCC50 (no RTTI); slot 0 0x00D34F60
public:
    std::vector<bool> PadActive;                        // +0x28
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00D35070
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00D34FB0
    virtual unsigned long GetSizeofClass();             // slot 20 0x00D34EF0
};

void CFireHeartPatternDef::Transfer(CPersistContext& context)
{
    context.TransferVector("PadActive", PadActive);
}