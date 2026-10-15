// CStreamingFontDataBank::GetLastModifiedTime(const CCharString&) -- streaming font banks keep no entry timestamps: returns the fixed date CDateAndTime(1, 1, 1, 1, 1, 0, 0)
// Leaf: GetLastModifiedTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5F9D0, 32 bytes; slot 18 of the CStreamingFontDataBank vtable
// 0x0129D5CC (?GetLastModifiedTime@CStreamingFontDataBank@@MAE?AVCDateAndTime@@ABVCCharString@@@Z). Callee:
// CDateAndTime::CDateAndTime(long x7) @ 0x009E9A70 (landed). Byte-identical to the landed 0x009CB710 body (same
// source): the result is built in the caller's hidden slot (first stack slot, kept in esi) and returned in eax.
// Layout: no member is read; the class is modelled through engine/CStreamingFontDataBank.h.
#include "engine/CStreamingFontDataBank.h"

class CCharString;

struct CDateAndTime {
    unsigned char _raw[0x10];
    CDateAndTime(long year, long month, long day, long hour, long minute, long second, long millisecond);   // 0x009E9A70
};

struct CStreamingFontDataBank_Methods : CStreamingFontDataBank {
    CDateAndTime GetLastModifiedTime(const CCharString& entryName);
};

CDateAndTime CStreamingFontDataBank_Methods::GetLastModifiedTime(const CCharString& entryName)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}