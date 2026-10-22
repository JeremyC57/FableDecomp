// CPersistContext::Transfer<ESaveType>(const char* name, ESaveType& value) -- the two-argument overload: forward to the three-argument Transfer with a value-initialised (zero) default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB7F0, 32 bytes, `ret 8`. Unreferenced out-of-line instance.
// Callees: CPersistContext::Transfer<ESaveType>(const char*, ESaveType&, const ESaveType&) 0x004AB640 (catalogued as the
//   propagated "Transfer<signed_char>"; its callers in CWorld's save-state code (0x0049F79C, 0x0049F7BA) pass
//   CWorld::MostRecentSaveType / MostRecentSaveTypeBeforeManualSave, PDB type ESaveType, so T = ESaveType).
// Shape: the `T()` default is a 4-byte frame temporary (`push ecx`), its address pushed first and the zero stored after the
//   three pushes; `this` stays in ecx; `pop ecx ; ret 8`. The sibling double instance is the landed 0x004AB810.
enum ESaveType { SAVE_TYPE_DEFAULT = 0 };

struct CPersistContext {
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004AB640 for T = ESaveType
    template <class T> void Transfer(const char* name, T& value)
    {
        Transfer(name, value, T());
    }
};

template void CPersistContext::Transfer<ESaveType>(const char* name, ESaveType& value);