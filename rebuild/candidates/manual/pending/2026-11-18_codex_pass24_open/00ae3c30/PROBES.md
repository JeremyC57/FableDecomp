# Compiler history

At 00AE40AC the texture-coordinate buffer starts in EDI rather than EAX. Retail copies EAX to EDI after the count test. The later read and the remainder of the function realign.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | COMPILE_FAIL | — | — | — |
| donor stream variables | DIFFER | 2318 | 3 | +0xD3 |
| donor empty vertex ctor | DIFFER | 2310 | 1 | +0x47D |
| get as array at read | DIFFER | 2278 | 60 | +0x442 |
| select ternary data | DIFFER | 2286 | 61 | +0x442 |
| data from end minus size | DIFFER | 2334 | 7 | +0x467 |
| data with backed reference | DIFFER | 2310 | 1 | +0x47D |
| select from explicit base | DIFFER | 2302 | 2 | +0x47D |
| first and last iterators | DIFFER | 2318 | 4 | +0x467 |
| call per branch | DIFFER | 2326 | 72 | +0xC |
| offset reference const size | DIFFER | 2286 | 59 | +0x442 |
| source scope read | DIFFER | 2310 | 1 | +0x47D |
| named all array parameters | DIFFER | 2310 | 1 | +0x47D |
| assign empty first | DIFFER | 2310 | 2 | +0x47D |
| unsigned distance | DIFFER | 2318 | 4 | +0x467 |
| count through array reference | DIFFER | 2308 | 1 | +0x47D |
| conditional base before nonempty | DIFFER | 2278 | 61 | +0x442 |
| loop single array | DIFFER | 2310 | 1 | +0x47D |
| local ref to zero array | DIFFER | 2310 | 1 | +0x47D |
| shared byte length | DIFFER | 2294 | 2 | +0x47D |
| shared element count | DIFFER | 2302 | 4 | +0x47D |
| particle final selected | DIFFER | 2310 | 1 | +0x47D |
| array original both accessors | DIFFER | 2278 | 60 | +0x442 |
| array original accessors direct start | DIFFER | 2278 | 60 | +0x442 |
| array original both accessors visible resize | DIFFER | 2286 | 61 | +0x442 |
| array original both accessors preceding output argument | DIFFER | 2286 | 59 | +0x442 |
| array original accessors direct start visible resize | DIFFER | 2286 | 61 | +0x442 |
| array original accessors direct start preceding output argument | DIFFER | 2286 | 59 | +0x442 |

Retained source and test coverage. The direct fixture covers independent empty arrays, simulation and allocation failure, exports and both groups.

## Additional main-chat comparisons

- main_close_from_end_sub — DIFFER, 2286 bytes, 65 strict instruction hunks.
- main_close_size_then_pick — DIFFER, 2286 bytes, 61 strict instruction hunks.
- main_close_iterator_pick — DIFFER, 2286 bytes, 61 strict instruction hunks.
- main_close_buffer_lvalue — DIFFER, 2286 bytes, 61 strict instruction hunks.
- main_close_separate_if — DIFFER, 2286 bytes, 61 strict instruction hunks.
- main_close_subtract_ptr — DIFFER, 2318 bytes, 4 strict instruction hunks.

The selected result is PARKED (2310 object bytes, 2310 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
