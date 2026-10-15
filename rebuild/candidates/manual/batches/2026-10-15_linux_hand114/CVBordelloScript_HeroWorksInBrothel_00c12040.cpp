// CV_BordelloScript::HeroWorksInBrothel() -- per-update step: tick the embedded timer (+0x7d8), update the child list, and when a request is pending (+0x828) and the timer has elapsed, hand the request record (+0x82c) to the handler at +0x7bc, clear the request and reset the timer
// Retail: Fable.exe @ 0x00C12040, 82 bytes (+ int3 padding). Called once (0x00C12901).
// Callees: the embedded timer's update @ 0x00C25F40, elapsed test @ 0x00C25DE0 (bool) and reset @ 0x00C25E00 (the last
// statement, a tail jump); this class's child update @ 0x00C12000 (walks the list at +0x224); the handler's
// request call @ 0x00C1C210.
// Label note: "CV_BordelloScript::HeroWorksInBrothel" and the callee labels are propagated (the body lives in an
// engine module at 0x00C1xxxx); the members are modelled descriptively.
// Shape: `&m_timer` is held in edi for all three timer calls, `this` in esi; `if (a && b)` gives two `je` to the
// shared plain return; the byte clear is scheduled between the `pop edi` and the tail jump.
struct CRequestTimer {
    void Update();                                       // 0x00C25F40
    bool HasElapsed();                                   // 0x00C25DE0
    void Reset();                                        // 0x00C25E00
};

struct CRequestHandler {
    void Handle(void* request);                          // 0x00C1C210
};

struct CV_BordelloScript {
    unsigned char _pad000[0x7bc];
    CRequestHandler* m_pHandler;                         // +0x7bc
    unsigned char _pad7c0[0x7d8 - 0x7c0];
    CRequestTimer m_timer;                               // +0x7d8
    unsigned char _pad7d9[0x828 - 0x7d9];
    bool m_requestPending;                               // +0x828
    unsigned char _pad829[3];
    unsigned char m_request[4];                          // +0x82c
    void UpdateChildren();                               // 0x00C12000
    void HeroWorksInBrothel();
};

void CV_BordelloScript::HeroWorksInBrothel()
{
    m_timer.Update();
    UpdateChildren();
    if (m_requestPending && m_timer.HasElapsed()) {
        m_pHandler->Handle(m_request);
        m_requestPending = false;
        m_timer.Reset();
    }
}
