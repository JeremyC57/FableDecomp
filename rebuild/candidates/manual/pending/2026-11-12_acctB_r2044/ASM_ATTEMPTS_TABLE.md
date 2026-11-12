
Near sites, plain C++ (X = y: `(cosom + 1.0f) * 512.5f` in GFFastArcCos, `angle * 1024.0f` in GFFastSin; retail = GFFloatToLongNear(y - 0.5f)); counts = the fragment's three expanded conversions:
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| near_00 | `(long)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_01 | `(int)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_02 | `static_cast<long>(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_03 | `long(X - 0.5f) functional cast` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_04 | `(long)X (truncation, no bias)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_05 | `(long)(double)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_06 | `(long)((double)X - 0.5)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x `fistp qword`, 6x fldcw, 3x fnstcw |
| near_07 | `(long)(long double)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_08 | `(long)(__int64)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_09 | `(long)(unsigned long)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_10 | `(long)(short)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_11 | `(long)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_12 | `(long)floorf(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_13 | `(long)floor((double)X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_14 | `(long)std::floor(X) (<cmath> float overload)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_15 | `(long)ceil(X - 1.0f)` | 3x `__ftol2`, 3x _ceil | 3x _ceil, 3x `fistp qword` | 3x _ceil, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_16 | `(long)(X - fmod(X, 1.0f))` | 3x __CIfmod, 3x `__ftol2` | 3x __CIfmod, 3x `fistp qword` | 3x __CIfmod, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_17 | `(long)(X - fmodf(X, 1.0f))` | 3x __CIfmod, 3x `__ftol2` | 3x __CIfmod, 3x `fistp qword` | 3x __CIfmod, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_18 | `modf(X, &ip) then (long)ip` | 3x `__ftol2`, 3x _modf | 3x _modf, 3x `fistp qword` | 3x _modf, 3x cvtsi2ss, 3x cvttsd2si |
| near_19 | `long R = X - 0.5f; (implicit conversion)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_20 | `named float v = X - 0.5f; R = (long)v` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_21 | `through inline long ToLong(float)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_22 | `through inline long ToLongRef(const float&)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_23 | `sign-split round: v >= 0 ? (long)(v + 0.5f) : (long)(v - 0.5f), v = X - 0.5f` | 3x `__ftol2` | 6x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_24 | `(long)(X + 0.5f) - 1` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_25 | `(long)(X * 1.0f - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_26 | `union double magic: d = (X - 0.5f) + 6755399441055744.0, R = low word` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_27 | `union float magic: f = (X - 0.5f) + 12582912.0f, R = bits - 0x4B400000` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_28 | `pointer pun: f = (X - 0.5f) + 12582912.0f; *(long*)&f - 0x4B400000` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_29 | `memcpy pun of the 12582912.0f magic sum` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_30 | `(long)(unsigned short)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x `__ftol2`, 3x cvtsi2ss |
| near_31 | `trunc then floor fix-up: R = (long)X; if ((float)R > X) --R` | 3x `__ftol2` | 3x `fistp qword` | 6x cvtsi2ss, 3x cvttss2si |
| near_32 | `(long)_copysign(floor(fabs(v)), v), v = X - 0.5f` | 3x __copysign, 3x `__ftol2`, 3x _floor | 3x __copysign, 3x _floor, 3x `fistp qword` | 3x __copysign, 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_33 | `volatile float v = X - 0.5f; R = (long)v (homed like retail)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_34 | `(long)(double)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_35 | `(long)(X - 0.5) (double constant)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x `fistp qword`, 6x fldcw, 3x fnstcw |
| near_36 | `(long)(__int64)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_37 | `direct-init long r(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |

Near sites, intrinsics (12 compiler intrinsics + 2 CRT x87 control-word calls + 1 MXCSR macro):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| nintr_00 | `_mm_cvtss_si32(_mm_set_ss(X - 0.5f))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_01 | `_mm_cvt_ss2si(_mm_set_ss(X - 0.5f))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_02 | `_mm_cvttss_si32(_mm_set_ss(X))` | 3x cvttss2si | 3x cvttss2si | 3x cvtsi2ss, 3x cvttss2si |
| nintr_03 | `_mm_cvtt_ss2si(_mm_load_ss(&v)), v = X` | 3x cvttss2si | 3x cvttss2si | 3x cvtsi2ss, 3x cvttss2si |
| nintr_04 | `_mm_cvtss_si32(_mm_sub_ss(_mm_set_ss(X), _mm_set_ss(0.5f)))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_05 | `_mm_cvtsd_si32(_mm_set_sd(X - 0.5f))` | 3x cvtsd2si | 3x cvtsd2si | 3x cvtsd2si, 3x cvtsi2ss |
| nintr_06 | `_mm_cvttsd_si32(_mm_set_sd(X))` | 3x cvttsd2si | 3x cvttsd2si | 3x cvtsi2ss, 3x cvttsd2si |
| nintr_07 | `_mm_cvtsi64_si32(_mm_cvtps_pi32(_mm_set_ss(X - 0.5f)))` | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi, 3x cvtsi2ss |
| nintr_08 | `_mm_cvtsi64_si32(_mm_cvttps_pi32(_mm_set_ss(X)))` | 1x ?sqrt@@YAMM@Z, 3x cvttps2pi | 1x ?sqrt@@YAMM@Z, 3x cvttps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtsi2ss, 3x cvttps2pi |
| nintr_09 | `_mm_cvtsi128_si32(_mm_cvtps_epi32(_mm_set_ss(X - 0.5f)))` | 3x cvtps2dq | 3x cvtps2dq | 3x cvtps2dq, 3x cvtsi2ss |
| nintr_10 | `_mm_cvtsi128_si32(_mm_cvttps_epi32(_mm_set_ss(X)))` | 3x cvttps2dq | 3x cvttps2dq | 3x cvtsi2ss, 3x cvttps2dq |
| nintr_11 | `_mm_cvtsi128_si32(_mm_cvtpd_epi32(_mm_set_sd(X - 0.5f)))` | 3x cvtpd2dq | 3x cvtpd2dq | 3x cvtpd2dq, 3x cvtsi2ss |
| nintr_12 | `_controlfp(_RC_NEAR, _MCW_RC) then (long)(X - 0.5f)` | 3x __controlfp, 3x `__ftol2` | 3x __controlfp, 3x `fistp qword` | 3x __controlfp, 3x cvtsi2ss, 3x cvttss2si |
| nintr_13 | `_control87(_RC_DOWN, _MCW_RC) then (long)X` | 3x __control87, 3x `__ftol2` | 3x __control87, 3x `fistp qword` | 3x __control87, 3x cvtsi2ss, 3x cvttss2si |
| nintr_14 | `_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN) then _mm_cvtss_si32(_mm_set_ss(X))` | 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr | 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr | 3x cvtsi2ss, 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr |

Floor site 0x00A52677 (ConvertTimeToFrames), plain C++ (X = frames = `time * SamplesPerSecond`; retail = GFFloatToLongFloor(frames)):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| floor_00 | `(long)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_01 | `(long)floorf(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_02 | `(long)floor((double)X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_03 | `(long)std::floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_04 | `(int)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_05 | `static_cast<long>(floor(X))` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_06 | `(long)(double)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_07 | `(long)(float)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_08 | `(long)(__int64)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_09 | `(long)floor((long double)X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_10 | `(long)(X - 0.5f) (biased truncation)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_11 | `(long)X - (X < (float)(long)X ? 1 : 0)` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_12 | `trunc + fix-up: R = (long)X; if ((float)R > X) --R` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_13 | `X >= 0 ? (long)X : (long)X - ((float)(long)X != X)` | 2x `__ftol2` | 2x `fistp qword` | 2x cvtsi2ss, 2x cvttss2si |
| floor_14 | `retail's algorithm in C++: r = (long)(X - 0.5f); if (X == (float)r + 1.0f) ++r` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_15 | `retail's algorithm with the integer bit compare (memcpy)` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_16 | `retail's algorithm with *(long*)& compares` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_17 | `(long)(X + 0.5f) - 1` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_18 | `(long)(X + 1.0f) - 1` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_19 | `(long)(X < 0 ? X - 0.99999994f : X)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_20 | `(long)(X - fmod(X, 1.0f)) - (fmod < 0)` | 1x __CIfmod, 1x `__ftol2` | 1x __CIfmod, 1x `fistp qword` | 1x __CIfmod, 1x cvtsi2ss, 1x `fistp qword`, 2x fldcw, 1x fnstcw |
| floor_21 | `modf(X, &ip); (long)ip - (X < ip)` | 1x `__ftol2`, 1x _modf | 1x _modf, 1x `fistp qword` | 1x _modf, 1x cvtsi2ss, 1x cvttsd2si |
| floor_22 | `-(long)ceil(-X)` | 1x `__ftol2`, 1x _ceil | 1x _ceil, 1x `fistp qword` | 1x _ceil, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_23 | `(long)ceil(X) - ((float)ceil(X) != X)` | 1x `__ftol2`, 1x _ceil | 1x _ceil, 1x `fistp qword` | 1x _ceil, 1x cvtsi2ss, 1x cvttsd2si |
| floor_24 | `through inline long FloorToLong(float)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_25 | `union double magic of (X - 0.5f)` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 1x cvtsi2ss |
| floor_26 | `union float magic of (X - 0.5f)` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 1x cvtsi2ss |
| floor_27 | `named float f = (float)floor(X); (long)f` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_28 | `implicit long R = floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_29 | `volatile float v = X - 0.5f; (long)v + fix-up` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_30 | `(long)(double)(X - 0.5f)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_31 | `(unsigned long)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |

Floor site, intrinsics (11 compiler intrinsics + 1 MXCSR macro + 2 CRT x87 control-word calls):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| fintr_00 | `_mm_cvtss_si32(_mm_set_ss(X - 0.5f)) (retail's biased round, SSE)` | 1x cvtss2si | 1x cvtss2si | 1x cvtsi2ss, 1x cvtss2si |
| fintr_01 | `_mm_cvt_ss2si(_mm_set_ss(X - 0.5f))` | 1x cvtss2si | 1x cvtss2si | 1x cvtsi2ss, 1x cvtss2si |
| fintr_02 | `_mm_cvttss_si32(_mm_set_ss(X))` | 1x cvttss2si | 1x cvttss2si | 1x cvtsi2ss, 1x cvttss2si |
| fintr_03 | `_mm_cvtt_ss2si(_mm_load_ss(&X))` | 1x cvttss2si | 1x cvttss2si | 1x cvtsi2ss, 1x cvttss2si |
| fintr_04 | `_mm_cvtsd_si32(_mm_set_sd(X - 0.5f))` | 1x cvtsd2si | 1x cvtsd2si | 1x cvtsd2si, 1x cvtsi2ss |
| fintr_05 | `_mm_cvttsd_si32(_mm_set_sd(X))` | 1x cvttsd2si | 1x cvttsd2si | 1x cvtsi2ss, 1x cvttsd2si |
| fintr_06 | `_mm_cvtsi64_si32(_mm_cvtps_pi32(_mm_set_ss(X - 0.5f)))` | 1x cvtps2pi | 1x cvtps2pi | 1x cvtps2pi, 1x cvtsi2ss |
| fintr_07 | `_mm_cvtsi64_si32(_mm_cvttps_pi32(_mm_set_ss(X)))` | 1x cvttps2pi | 1x cvttps2pi | 1x cvtsi2ss, 1x cvttps2pi |
| fintr_08 | `_mm_cvtsi128_si32(_mm_cvtps_epi32(_mm_set_ss(X - 0.5f)))` | 1x cvtps2dq | 1x cvtps2dq | 1x cvtps2dq, 1x cvtsi2ss |
| fintr_09 | `_mm_cvtsi128_si32(_mm_cvttps_epi32(_mm_set_ss(X)))` | 1x cvttps2dq | 1x cvttps2dq | 1x cvtsi2ss, 1x cvttps2dq |
| fintr_10 | `_mm_cvtsi128_si32(_mm_cvtpd_epi32(_mm_set_sd(X - 0.5f)))` | 1x cvtpd2dq | 1x cvtpd2dq | 1x cvtpd2dq, 1x cvtsi2ss |
| fintr_11 | `_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN) then _mm_cvtss_si32(_mm_set_ss(X))` | 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr | 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr | 1x cvtsi2ss, 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr |
| fintr_12 | `_controlfp(_RC_DOWN, _MCW_RC) then (long)X` | 1x __controlfp, 1x `__ftol2` | 1x __controlfp, 1x `fistp qword` | 1x __controlfp, 1x cvtsi2ss, 1x cvttss2si |
| fintr_13 | `_control87(_RC_NEAR, _MCW_RC) then the biased (long)(X - 0.5f) + fix-up` | 1x __control87, 1x `__ftol2` | 1x __control87, 1x `fistp qword` | 1x __control87, 2x cvtsi2ss, 1x cvttss2si |
