// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00a36f90();
extern "C" void CNavigatorManager_AddNewClassType() { ForwardTarget_00a36f90(); }