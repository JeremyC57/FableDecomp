// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_008fcec0();
extern "C" void CAIStateGroup_MazeTeleport_Initialise() { ForwardTarget_008fcec0(); }