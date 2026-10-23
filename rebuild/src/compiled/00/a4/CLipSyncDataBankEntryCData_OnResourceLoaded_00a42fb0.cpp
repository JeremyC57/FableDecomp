// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00a42ea0();
extern "C" void CData_OnResourceLoaded() { ForwardTarget_00a42ea0(); }