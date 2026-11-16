#ifndef FABLE_LIBPK_CPKVOICEPARAMS_H
#define FABLE_LIBPK_CPKVOICEPARAMS_H
// Descriptive libpk parameter object. Native ctorC10A30 and requestC266E0 prove0x50 bytes;
// LoadC27C10 copies settings; members are unaccessed by PlayStreamed and remain opaque.
#ifdef FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
#include "libpk/CSoundPosition.h"
#include "rebuild_abi.h"
#include <stddef.h>
#endif
struct Input;
struct CPKVoiceParams {
 CPKVoiceParams();
 void Load(const Input*);
#ifdef FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
 // Native C10A30 and independent request ctorC266E0 construct C20390 at+30.
 // Native LoadC27C10 copies XYZ to+30/+34/+38 and Relative to+3c.
 unsigned char UnrecoveredPrefix[0x30];
 CSoundPosition Position;
 unsigned char UnrecoveredTail[0x10];
#else
 unsigned char UnrecoveredState[0x50];
#endif
};
#ifdef FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
FABLE_STATIC_ASSERT(sizeof(CPKVoiceParams)==0x50);
FABLE_STATIC_ASSERT(offsetof(CPKVoiceParams,Position)==0x30);
#endif
#endif
