#include <stdio.h>
struct State{int index;};static int GetMainPlayerJoystickDeviceNumber(State* s){return s->index;}
int main(){State s={7};if(GetMainPlayerJoystickDeviceNumber(&s)!=7)return 1;s.index=-3;if(GetMainPlayerJoystickDeviceNumber(&s)!=-3)return 2;puts("TINY_CATCHES_PASS");return 0;}