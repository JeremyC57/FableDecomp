// Member getter at this+0xc4. __fastcall this=ecx.
// Leaf: DrawGetSeaLevel
struct T {
    char pad[0xc4];
    float field;
    float Get();
};
float T::Get() { return this->field; }