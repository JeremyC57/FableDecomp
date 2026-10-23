// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00750dd0();
extern "C" void CTCExplodingObject_OnDie() { ForwardTarget_00750dd0(); }