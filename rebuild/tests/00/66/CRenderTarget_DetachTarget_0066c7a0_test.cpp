#include <cstdio>
#include <cstring>
// Behaviour test for 0x0066C7A0 (links the row object): the CSurface at +0x24 is uninitialised only when its PD3DSurface (+0x28) is set.
// NF142 (2026-11-20, q6305): rewritten on engine/CSurface.h, the header the row now uses (the old test was a standalone copy over the
//   dt_helper stand-in); CSurface::Uninitialise is the test's fixture. PassPattern unchanged.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#include "engine/CSurface.h"
struct S { char pad[0x24]; CSurface f24; };
void __fastcall DetachTarget(S* self);

static const void* g = 0;
void CSurface::Uninitialise() { g = this; }

int main(){
 static double storage[(sizeof(S) + 7) / 8];
 char* raw = (char*)storage;
 std::memset(storage, 0, sizeof storage);
 DetachTarget((S*)raw); if(g){ std::printf("0066c7a0_TEST FAIL\n"); return 1;}
 void* surface = (void*)1; std::memcpy(raw + 0x28, &surface, 4);
 DetachTarget((S*)raw); if(g!=raw+0x24){ std::printf("0066c7a0_TEST FAIL\n"); return 1;}
 std::printf("0066c7a0_TEST PASS\n"); return 0;}
