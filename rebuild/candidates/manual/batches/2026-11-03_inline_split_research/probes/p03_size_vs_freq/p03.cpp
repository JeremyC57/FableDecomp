// p03: callee size (k extra global stores) vs site nesting depth (freq 2^-d)
bool Test(int); void Yield();
extern int G[64];
inline void F0(int a) { G[0] = a + 0; }
void s0_0(int a) { F0(a); Yield(); }
void s0_1(int a) { if (Test(0)) F0(a); Yield(); }
void s0_2(int a) { if (Test(0)) if (Test(1)) F0(a); Yield(); }
void s0_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F0(a); Yield(); }
void s0_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F0(a); Yield(); }
void s0_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F0(a); Yield(); }
void s0_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F0(a); Yield(); }
void s0_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F0(a); Yield(); }
inline void F2(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; }
void s2_0(int a) { F2(a); Yield(); }
void s2_1(int a) { if (Test(0)) F2(a); Yield(); }
void s2_2(int a) { if (Test(0)) if (Test(1)) F2(a); Yield(); }
void s2_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F2(a); Yield(); }
void s2_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F2(a); Yield(); }
void s2_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F2(a); Yield(); }
void s2_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F2(a); Yield(); }
void s2_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F2(a); Yield(); }
inline void F4(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; }
void s4_0(int a) { F4(a); Yield(); }
void s4_1(int a) { if (Test(0)) F4(a); Yield(); }
void s4_2(int a) { if (Test(0)) if (Test(1)) F4(a); Yield(); }
void s4_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F4(a); Yield(); }
void s4_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F4(a); Yield(); }
void s4_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F4(a); Yield(); }
void s4_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F4(a); Yield(); }
void s4_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F4(a); Yield(); }
inline void F6(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; }
void s6_0(int a) { F6(a); Yield(); }
void s6_1(int a) { if (Test(0)) F6(a); Yield(); }
void s6_2(int a) { if (Test(0)) if (Test(1)) F6(a); Yield(); }
void s6_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F6(a); Yield(); }
void s6_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F6(a); Yield(); }
void s6_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F6(a); Yield(); }
void s6_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F6(a); Yield(); }
void s6_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F6(a); Yield(); }
inline void F8(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; }
void s8_0(int a) { F8(a); Yield(); }
void s8_1(int a) { if (Test(0)) F8(a); Yield(); }
void s8_2(int a) { if (Test(0)) if (Test(1)) F8(a); Yield(); }
void s8_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F8(a); Yield(); }
void s8_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F8(a); Yield(); }
void s8_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F8(a); Yield(); }
void s8_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F8(a); Yield(); }
void s8_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F8(a); Yield(); }
inline void F10(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; }
void s10_0(int a) { F10(a); Yield(); }
void s10_1(int a) { if (Test(0)) F10(a); Yield(); }
void s10_2(int a) { if (Test(0)) if (Test(1)) F10(a); Yield(); }
void s10_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F10(a); Yield(); }
void s10_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F10(a); Yield(); }
void s10_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F10(a); Yield(); }
void s10_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F10(a); Yield(); }
void s10_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F10(a); Yield(); }
inline void F12(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; }
void s12_0(int a) { F12(a); Yield(); }
void s12_1(int a) { if (Test(0)) F12(a); Yield(); }
void s12_2(int a) { if (Test(0)) if (Test(1)) F12(a); Yield(); }
void s12_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F12(a); Yield(); }
void s12_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F12(a); Yield(); }
void s12_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F12(a); Yield(); }
void s12_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F12(a); Yield(); }
void s12_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F12(a); Yield(); }
inline void F14(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; G[13] = a + 13; G[14] = a + 14; }
void s14_0(int a) { F14(a); Yield(); }
void s14_1(int a) { if (Test(0)) F14(a); Yield(); }
void s14_2(int a) { if (Test(0)) if (Test(1)) F14(a); Yield(); }
void s14_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F14(a); Yield(); }
void s14_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F14(a); Yield(); }
void s14_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F14(a); Yield(); }
void s14_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F14(a); Yield(); }
void s14_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F14(a); Yield(); }
inline void F16(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; G[13] = a + 13; G[14] = a + 14; G[15] = a + 15; G[16] = a + 16; }
void s16_0(int a) { F16(a); Yield(); }
void s16_1(int a) { if (Test(0)) F16(a); Yield(); }
void s16_2(int a) { if (Test(0)) if (Test(1)) F16(a); Yield(); }
void s16_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F16(a); Yield(); }
void s16_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F16(a); Yield(); }
void s16_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F16(a); Yield(); }
void s16_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F16(a); Yield(); }
void s16_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F16(a); Yield(); }
inline void F20(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; G[13] = a + 13; G[14] = a + 14; G[15] = a + 15; G[16] = a + 16; G[17] = a + 17; G[18] = a + 18; G[19] = a + 19; G[20] = a + 20; }
void s20_0(int a) { F20(a); Yield(); }
void s20_1(int a) { if (Test(0)) F20(a); Yield(); }
void s20_2(int a) { if (Test(0)) if (Test(1)) F20(a); Yield(); }
void s20_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F20(a); Yield(); }
void s20_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F20(a); Yield(); }
void s20_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F20(a); Yield(); }
void s20_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F20(a); Yield(); }
void s20_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F20(a); Yield(); }
inline void F24(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; G[13] = a + 13; G[14] = a + 14; G[15] = a + 15; G[16] = a + 16; G[17] = a + 17; G[18] = a + 18; G[19] = a + 19; G[20] = a + 20; G[21] = a + 21; G[22] = a + 22; G[23] = a + 23; G[24] = a + 24; }
void s24_0(int a) { F24(a); Yield(); }
void s24_1(int a) { if (Test(0)) F24(a); Yield(); }
void s24_2(int a) { if (Test(0)) if (Test(1)) F24(a); Yield(); }
void s24_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F24(a); Yield(); }
void s24_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F24(a); Yield(); }
void s24_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F24(a); Yield(); }
void s24_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F24(a); Yield(); }
void s24_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F24(a); Yield(); }
inline void F32(int a) { G[0] = a + 0; G[1] = a + 1; G[2] = a + 2; G[3] = a + 3; G[4] = a + 4; G[5] = a + 5; G[6] = a + 6; G[7] = a + 7; G[8] = a + 8; G[9] = a + 9; G[10] = a + 10; G[11] = a + 11; G[12] = a + 12; G[13] = a + 13; G[14] = a + 14; G[15] = a + 15; G[16] = a + 16; G[17] = a + 17; G[18] = a + 18; G[19] = a + 19; G[20] = a + 20; G[21] = a + 21; G[22] = a + 22; G[23] = a + 23; G[24] = a + 24; G[25] = a + 25; G[26] = a + 26; G[27] = a + 27; G[28] = a + 28; G[29] = a + 29; G[30] = a + 30; G[31] = a + 31; G[32] = a + 32; }
void s32_0(int a) { F32(a); Yield(); }
void s32_1(int a) { if (Test(0)) F32(a); Yield(); }
void s32_2(int a) { if (Test(0)) if (Test(1)) F32(a); Yield(); }
void s32_3(int a) { if (Test(0)) if (Test(1)) if (Test(2)) F32(a); Yield(); }
void s32_4(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) F32(a); Yield(); }
void s32_5(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) F32(a); Yield(); }
void s32_6(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) F32(a); Yield(); }
void s32_7(int a) { if (Test(0)) if (Test(1)) if (Test(2)) if (Test(3)) if (Test(4)) if (Test(5)) if (Test(6)) F32(a); Yield(); }
