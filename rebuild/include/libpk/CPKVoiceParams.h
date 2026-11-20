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
#ifdef FABLE_RETAIL_CPKVOICEPARAMS_TRACKBITS
 // NF165 (rows 0x00C27B70 / 0x00C27B80 / 0x00C27B90): bits 0..2 of the byte at +0x44, which StartEvent 0x00C12940 passes to
 // CSoundEvent::Initialise as its three track flags (+0xAE bits 1 / 2 / 4). Descriptive names (libpk has no PDB).
 unsigned char UnrecoveredState[0x44];
 unsigned char TrackFlags;                     // +0x44
 unsigned char UnrecoveredState45[0x0b];
 bool GetTrackBit0() const;                    // 0x00C27B70
 bool GetTrackBit1() const;                    // 0x00C27B80
 bool GetTrackBit2() const;                    // 0x00C27B90
#else
 unsigned char UnrecoveredState[0x50];
#endif
#endif
};
#ifdef FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
FABLE_STATIC_ASSERT_AT(25, sizeof(CPKVoiceParams)==0x50);
FABLE_STATIC_ASSERT_AT(26, offsetof(CPKVoiceParams,Position)==0x30);
#endif
#endif
