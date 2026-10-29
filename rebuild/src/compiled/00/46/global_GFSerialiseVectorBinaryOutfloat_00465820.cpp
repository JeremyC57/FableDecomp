#pragma optimize("s",on)
#include "rebuild_abi.h"
// GFSerialiseVectorBinaryOut<float> @ 0x00465820 (retail, 50 bytes): write a 4-byte-element vector to a binary stream.
// The stream (a CDataOutputStream) arrives in ecx, the vector's {first, last} in edx.
//   count = (last - first) >> 2 goes out through CDataOutputStream::WriteLong(long) @ 0x00993EB0,
//   then every element's raw dword (`push [first+i*4] ; call`) through the same WriteLong.
// engine/CDataOutputStream.h is data-only; CDataOutputStream is the lane's method carrier
// (as in the landed 0x00B17F70). WriteULong(unsigned long) is the separate body at 0x00993EA0.
#include "engine/CDataOutputStream.h"
struct FloatVecRange {
    long* first;   // +0x00  begin
    long* last;    // +0x04  end
};
void __fastcall GFSerialiseVectorBinaryOut_float(CDataOutputStream* thisptr, void* /*edx*/ vec)
{
    FloatVecRange* r = (FloatVecRange*)vec;
    long count = (long)(((char*)r->last - (char*)r->first) >> 2);
    thisptr->WriteSLONG(count);
    for (long i = 0; i < count; ++i) {
        thisptr->WriteSLONG(*(long*)((char*)r->first + i * 4));
    }
}
