#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" float __cdecl CUnprovenFloatProvider_00A76320();
float g_UnprovenScaledFloat_013D2D64=CUnprovenFloatProvider_00A76320()*0.9f;
static int calls;extern "C" float __cdecl CUnprovenFloatProvider_00A76320(){++calls;return 5.0f;}int main(){bool ok=calls==1&&g_UnprovenScaledFloat_013D2D64==4.5f;puts(ok?"01224430_TEST PASS":"01224430_TEST FAIL");return ok?0:1;}