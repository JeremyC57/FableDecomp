// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_008b0650();
extern "C" void CCombatSequence_KingScorpionTailPlunge_OnCreate() { ForwardTarget_008b0650(); }