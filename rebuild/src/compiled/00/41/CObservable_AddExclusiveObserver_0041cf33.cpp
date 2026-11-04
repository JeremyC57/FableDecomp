// Member setter at this+0xc4: one stack arg, `ret 4`. __fastcall this=ecx.
// Leaf: ?Set@T@@QAEXH@Z
#pragma pack(push,1)
struct T {
    char pad[0xc4];
    int field;
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->field = value; }