// Member getter at this+0x8c. __fastcall this=ecx.
// Leaf: ?Get@T@@QAEMXZ
struct T {
    char pad[0x8c];
    float field;
    float Get();
};
float T::Get() { return this->field; }