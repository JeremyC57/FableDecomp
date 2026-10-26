// Forwarder to a member of the sub-object pointer at this+0x14. VC7.1 tail-calls
// Leaf: CEntry>_const&>
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#pragma pack(push,1)
class CGraphicDataBank { public: public:
    void ChangeScene();   // 0x009FD0C0
};
struct T {
    char pad[0x14];
    CGraphicDataBank* sub;
    void Run();
};
#pragma pack(pop)
void T::Run() { this->sub->ChangeScene(); }