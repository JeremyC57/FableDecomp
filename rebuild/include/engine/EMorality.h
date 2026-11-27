#ifndef FABLE_ENGINE_EMORALITY_H
#define FABLE_ENGINE_EMORALITY_H
#ifdef FABLE_RETAIL_BE_EVIL_MORALITY_PASS29
// Ego_d names MORALITY_SUPER_EVIL as zero. The original editor call returns
// EMorality and the retail script tests that result against zero.
enum EMorality { MORALITY_SUPER_EVIL = 0 };
#endif
#endif
