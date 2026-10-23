// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00463950();
extern "C" void CThingBaseDef_Copy() { ForwardTarget_00463950(); }