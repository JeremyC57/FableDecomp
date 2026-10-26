// One-line forwarder that VC7.1 tail-calls (`jmp rel32`): void member -> void member,
// identical signature, so the frame is reused. __fastcall this=ecx, src=stack.
class CControlsDef { public:
    void Copy(const CControlsDef* src);
    void CopyImpl_0067ce50(const CControlsDef* src);
};
void CControlsDef::Copy(const CControlsDef* src) { this->CopyImpl_0067ce50(src); }