# Attempts

| Attempt | Strategy | Result |
|---|---|---|
| 1 | Original STLport 4.6 _heap.c auxiliary algorithm, real shared pair/comparator and native size-optimized TU | Exact masked 34/34 bytes and exact callee binding |

The first compiler invocation exposed a missing macro activation; enabling the existing calling-convention macro under the new opt-in fixed compilation. This is one source-shape attempt, not two.

Behavior fixture:35 valid heaps (five patterns, lengths1–7), checking each forwarded argument, copied value, callee count, heap property, permutation and outside sentinels. Complete auxiliary body is unchanged. The initial test library lacked pointer iterator_traits; the final fixture uses the original shared pair excerpt and a pointer trait instead. Both VC71 /Od /GR- and g++ C++03 pass. Test infrastructure corrections are not matching attempts.
