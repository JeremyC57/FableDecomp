// Member getter at this+0xec. __fastcall this=ecx.
// Leaf: GetCanItemBeQuickAssigned
struct T {
    char pad[0xec];
    char field;
    char Get();
};
char T::Get() { return this->field; }