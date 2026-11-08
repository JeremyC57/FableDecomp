// When does VC7.1 /O2 pad a loop head to 16 bytes (lea esp,[esp] / lea ecx,[ecx] / nop)?
extern int g[64]; int f(int); bool T(); void NSF();
int a1_simple(int n) { int s = 0; for (int i = 0; i < n; ++i) s += g[i]; return s; }
int a2_call(int n) { int s = 0; for (int i = 0; i < n; ++i) s += f(i); return s; }
int a3_while_call_ret(int n) { while (!f(n)) { NSF(); if (T()) return 1; } return 0; }
int a4_while_call(int n) { while (!f(n)) { NSF(); } return 0; }
int a5_nested(int n) { int s = 0; for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) s += g[i] * g[j]; return s; }
int a6_do(int n) { int s = 0; do { s += f(s); } while (s < n); return s; }
int a7_two_loops(int n) { int s = 0; for (int i = 0; i < n; ++i) s += g[i]; for (int i = 0; i < n; ++i) s += f(i); return s; }
int a8_loop_after_calls(int n) { f(1); f(2); f(3); int s = 0; for (int i = 0; i < n; ++i) s += g[i]; return s; }
int a9_loop_ret_inside(int n) { for (int i = 0; i < n; ++i) { if (g[i] == n) return i; } return -1; }
int a10_while_T_first(int n) { while (!T()) { if (f(n)) return 2; NSF(); } return 0; }
