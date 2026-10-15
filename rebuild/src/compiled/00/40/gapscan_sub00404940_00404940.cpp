// T::Set(int value) -- store one stack argument into the member at +0x10
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404940, 10 bytes. Callees: none.
//
// A plain setter: one dword argument written to the member at +0x10, then `ret 4`. Listed in the symbol
// tables as NUISystem::CIMEFont slot 0, which fits the IME font object's small virtual setter; the body
// is the same for any class with a dword at that offset.
//
// Matching: `this` in ecx, the value read from [esp+4] and stored, `ret 4`; packed so the field sits
// exactly at +0x10.
#pragma pack(push,1)
struct T {
    char pad[0x10];
    int field;                 // the value being set
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->field = value; }
