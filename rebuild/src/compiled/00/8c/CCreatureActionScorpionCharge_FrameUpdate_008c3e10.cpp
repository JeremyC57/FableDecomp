// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00859d80();
extern "C" void CCreatureAction_ScorpionCharge_FrameUpdate() { ForwardTarget_00859d80(); }