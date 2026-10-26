// Forwarder to a member of the sub-object pointer at this+0xc. VC7.1 tail-calls
// Leaf: CEntry>_const&>
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#pragma pack(push,1)
class CDisplayEngine { public: public:
    void EndScreenFadeOut();   // 0x00434C90
};
struct T {
    char pad[0xc];
    CDisplayEngine* sub;
    void Run();
};
#pragma pack(pop)
void T::Run() { this->sub->EndScreenFadeOut(); }