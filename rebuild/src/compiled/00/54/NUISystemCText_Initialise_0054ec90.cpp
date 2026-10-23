// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_0052c730();
extern "C" void CText_Initialise() { ForwardTarget_0052c730(); }