// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00857520();
extern "C" void CCreatureAction_ReleaseTimeSpell_Initialise() { ForwardTarget_00857520(); }