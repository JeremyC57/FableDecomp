// Destructor of a polymorphic class with no members to release: MSVC still
// Leaf: ??1T@@QAE@XZ
// re-installs the vptr (`mov dword ptr [ecx], offset vftable`). The vtable
// address is a data relocation. __fastcall this=ecx.
struct T {
    virtual void Method() {}
    ~T();
};
T::~T() {}