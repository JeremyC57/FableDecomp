/* POSIX build: the MSVC intrinsics the Windows headers use, on compiler atomics.
   Renamed through macros so they never collide with clang's own MS builtins. */
#pragma once
#include <stdint.h>
#include <stdlib.h>
#define W32_INTRIN static inline __attribute__((always_inline))
#define _InterlockedIncrement w32_InterlockedIncrement
#define _InterlockedDecrement w32_InterlockedDecrement
#define _InterlockedExchange w32_InterlockedExchange
#define _InterlockedExchangeAdd w32_InterlockedExchangeAdd
#define _InterlockedCompareExchange w32_InterlockedCompareExchange
#define _InterlockedIncrement64 w32_InterlockedIncrement64
#define _InterlockedDecrement64 w32_InterlockedDecrement64
#define _InterlockedExchange64 w32_InterlockedExchange64
#define _InterlockedExchangeAdd64 w32_InterlockedExchangeAdd64
#define _InterlockedCompareExchange64 w32_InterlockedCompareExchange64
#define _InterlockedAnd64 w32_InterlockedAnd64
#define _InterlockedOr64 w32_InterlockedOr64
#define _InterlockedXor64 w32_InterlockedXor64
#define _InterlockedCompareExchangePointer w32_InterlockedCompareExchangePointer
#define _InterlockedExchangePointer w32_InterlockedExchangePointer
W32_INTRIN int32_t _InterlockedIncrement(int32_t volatile* p) { return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST); }
W32_INTRIN int32_t _InterlockedDecrement(int32_t volatile* p) { return __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST); }
W32_INTRIN int32_t _InterlockedExchange(int32_t volatile* p, int32_t v) { return __atomic_exchange_n(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN int32_t _InterlockedExchangeAdd(int32_t volatile* p, int32_t v) { return __atomic_fetch_add(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN int32_t _InterlockedCompareExchange(int32_t volatile* p, int32_t x, int32_t c) { __atomic_compare_exchange_n(p, &c, x, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return c; }
W32_INTRIN long long _InterlockedIncrement64(long long volatile* p) { return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedDecrement64(long long volatile* p) { return __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedExchange64(long long volatile* p, long long v) { return __atomic_exchange_n(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedExchangeAdd64(long long volatile* p, long long v) { return __atomic_fetch_add(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedCompareExchange64(long long volatile* p, long long x, long long c) { __atomic_compare_exchange_n(p, &c, x, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return c; }
W32_INTRIN long long _InterlockedAnd64(long long volatile* p, long long v) { return __atomic_fetch_and(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedOr64(long long volatile* p, long long v) { return __atomic_fetch_or(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN long long _InterlockedXor64(long long volatile* p, long long v) { return __atomic_fetch_xor(p, v, __ATOMIC_SEQ_CST); }
W32_INTRIN void* _InterlockedCompareExchangePointer(void* volatile* p, void* x, void* c) { __atomic_compare_exchange_n(p, &c, x, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return c; }
W32_INTRIN void* _InterlockedExchangePointer(void* volatile* p, void* v) { return __atomic_exchange_n(p, v, __ATOMIC_SEQ_CST); }
#ifdef __cplusplus
extern "C"
#endif
unsigned long long w32_readgsqword(unsigned long off); /* fake TEB (posix/w32/sync.cpp) */
#define __readgsqword(off) w32_readgsqword(off)
#define _aligned_malloc(n, a) aligned_alloc((a), (((n) + (a) - 1) / (a)) * (a))
#define _aligned_free free
#include <string.h>
#define __stosb w32___stosb
W32_INTRIN void __stosb(unsigned char* d, unsigned char v, size_t n) { memset(d, v, n); }
