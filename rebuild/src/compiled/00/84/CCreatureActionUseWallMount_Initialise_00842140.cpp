// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00694d10();
extern "C" void CCreatureAction_UseWallMount_Initialise() { ForwardTarget_00694d10(); }