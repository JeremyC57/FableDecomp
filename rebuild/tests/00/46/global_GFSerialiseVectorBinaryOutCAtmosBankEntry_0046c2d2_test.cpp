#include "rebuild_abi.h"
#include <stdio.h>

class CAtmosBankEntry { public: int a; int b; };
struct GFVector { CAtmosBankEntry* begin; CAtmosBankEntry* end; };

struct Serialiser {
    int  intBuf[16];  int  intCnt;
    float fltBuf[16]; int  fltCnt;
};
// Naming (NF443, 2026-11-25; NF438 q2): the callee stubs follow the row's rebind to the PDB spellings
// ?WriteSLONG@CDataOutputStream@@QAEXJ@Z / ?WriteFloat@CDataOutputStream@@QAEXM@Z (were Serialiser::Serialise(int / float)).
class CDataOutputStream { public: void WriteSLONG(long v); void WriteFloat(float v); };
void CDataOutputStream::WriteSLONG(long v) { Serialiser* s = reinterpret_cast<Serialiser*>(this); s->intBuf[s->intCnt++] = (int)v; }
void CDataOutputStream::WriteFloat(float v) { Serialiser* s = reinterpret_cast<Serialiser*>(this); s->fltBuf[s->fltCnt++] = v; }

void __fastcall GFSerialiseVectorBinaryOut_CAtmosBankEntry(Serialiser* thisptr, GFVector* vec);

int main(void)
{
    CAtmosBankEntry data[3];
    data[0].a = 10; data[0].b = 1;
    data[1].a = 20; data[1].b = 2;
    data[2].a = 30; data[2].b = 3;

    GFVector v; v.begin = &data[0]; v.end = &data[3];

    Serialiser s; s.intCnt = 0; s.fltCnt = 0;
    GFSerialiseVectorBinaryOut_CAtmosBankEntry(&s, &v);

    int ok = 1;
    if (s.intCnt != 4) ok = 0;              // count + 3 element ints
    if (s.intBuf[0] != 3) ok = 0;
    if (s.intBuf[1] != 10 || s.intBuf[2] != 20 || s.intBuf[3] != 30) ok = 0;
    if (s.fltCnt != 3) ok = 0;
    if (s.fltBuf[0] != 1.0f || s.fltBuf[1] != 2.0f || s.fltBuf[2] != 3.0f) ok = 0;

    if (ok) printf("PASS_0046c2d2_OK\n");
    else    printf("FAIL_0046c2d2\n");
    return 0;
}