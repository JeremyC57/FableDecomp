// CFrontEndDef::Transfer(CPersistContext& context) -- transfer the attract-mode movie list and the three graphic indices
// Leaf: ?Transfer@CFrontEndDef@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe @ 0x004331C0, 57 bytes, `ret 4`; slot of the CFrontEndDef vtable (reference at 0x01231004);
// Ego_r.pdb ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z. Catalog label std::deque<NUISystem::CComponent*>::...
// is wrong. Body: CPersistContext::TransferVectorBinary<CCharString>(vAttractModeMovie) (0x004331F9, +0x3c), then the
// context's long transfer (0x00431102, CheckCRC("") then load/save by Mode, as named in the landed
// CPlayerInventoryDef::Transfer 0x00467553) on ErrorMessageBackgroundGraphic (+0x48), ButtonABigGraphic (+0x4c) and
// ButtonBBigGraphic (+0x50). No base-class Transfer call. Modelled without virtuals (the vtable slot is the only
// reference).
// Layout: retail CFrontEndDef (as modelled for its deleting destructor 0x00431E40): CParentDefClassBase 0x39 bytes padded
// to 0x3c, vAttractModeMovie +0x3c (12-byte STLport vector), the three longs +0x48/+0x4c/+0x50 (Ego_r.pdb +0x40..+0x58).
// Shape: /O2.
class CCharString {
public:
    void* PStringData;
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorBinary(std::vector<T>& v);   // 0x004331F9 for CCharString
    template <class T> void Transfer(T& value);                      // 0x00431102
};
class CFrontEndDef {
public:
    unsigned char            _base[0x3c];                // CParentDefClassBase (vptr, 0x39 bytes, padding)
    std::vector<CCharString> vAttractModeMovie;          // +0x3c
    long                     ErrorMessageBackgroundGraphic;  // +0x48
    long                     ButtonABigGraphic;          // +0x4c
    long                     ButtonBBigGraphic;          // +0x50
    void Transfer(CPersistContext& context);
};
void CFrontEndDef::Transfer(CPersistContext& context)
{
    context.TransferVectorBinary(vAttractModeMovie);
    context.Transfer(ErrorMessageBackgroundGraphic);
    context.Transfer(ButtonABigGraphic);
    context.Transfer(ButtonBBigGraphic);
}