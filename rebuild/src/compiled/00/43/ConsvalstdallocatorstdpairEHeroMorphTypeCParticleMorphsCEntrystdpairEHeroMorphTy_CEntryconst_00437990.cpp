// Forwarder to a member of the sub-object pointer at this+0x150. VC7.1 tail-calls
// Leaf: CEntry>_const&>
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#pragma pack(push,1)
struct Sub { void Run_00643d8c(); };
struct T {
    char pad[0x150];
    Sub* sub;
    void Run();
};
#pragma pack(pop)
void T::Run() { this->sub->Run_00643d8c(); }