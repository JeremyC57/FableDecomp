// Forward the ADDRESS of the stack argument to a member of the sub-object at
// Leaf: AddLearnedExpression
// this+0x198. __fastcall this=ecx, value=stack (ret 4).
#pragma pack(push,1)
struct Sub { void Add_0042b6b8(int* value); };
struct T {
    char pad[0x198];
    Sub sub;
    void Add(int value);
};
#pragma pack(pop)
void T::Add(int value) { this->sub.Add_0042b6b8(&value); }