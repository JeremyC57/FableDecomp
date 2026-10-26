// Forwarder to a member of the sub-object pointer at this+0x24. VC7.1 tail-calls
// Leaf: CEntry>_const&>
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#pragma pack(push,1)
class CThingCreatureBase { public: public:
    bool IsInMeleeAsAttacked() const;   // 0x00662430
};
struct T {
    char pad[0x24];
    CThingCreatureBase* sub;
    void Run();
};
#pragma pack(pop)
void T::Run() { this->sub->IsInMeleeAsAttacked(); }