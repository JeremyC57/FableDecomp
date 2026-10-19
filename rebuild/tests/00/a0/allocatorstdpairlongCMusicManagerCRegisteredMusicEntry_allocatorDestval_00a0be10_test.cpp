#include <cstdio>
// Spy for C3DVector::Normalise (0x00A14440): records which vectors were normalised, in order.
struct C3DVector { float X, Y, Z; C3DVector& Normalise(); };
static int g_calls = 0; static C3DVector* g_seen[2];
C3DVector& C3DVector::Normalise(){ if (g_calls < 2) g_seen[g_calls] = this; ++g_calls; return *this; }
struct S { C3DVector a; C3DVector b; void M(); };
void S::M(){ a.Normalise(); b.Normalise(); }
int main(){ S o; o.M(); if(g_calls!=2||g_seen[0]!=&o.a||g_seen[1]!=&o.b||(char*)&o.b-(char*)&o!=0xc){ std::printf("00a0be10_TEST FAIL\n"); return 1;}
 std::printf("00a0be10_TEST PASS\n"); return 0;}
