// CDataOutputStream::WriteEBOOL(bool) -- write the flag as one byte (1 / 0) through the virtual Write
// Name fix (naming agent NF21, 2026-11-11): ?WriteEBOOL@CDataOutputStream@@QAEX_N@Z (Ego_r.pdb 0x00AFBE70, FableWin.pdb; the
//   shared engine/CDataOutputStream.h already names 0x00993E30). The CDataOutputStream_Methods stand-in is replaced by the
//   retail polymorphic class (FABLE_RETAIL_CDATAOUTPUTSTREAM): Write is slot 4 (+0x10) after ~CDataStream / SetPosition /
//   GetPosition / Close.
#include "rebuild_abi.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"

void CDataOutputStream::WriteEBOOL(bool value)
{
    unsigned char serializedValue = value ? 1 : 0;
    Write(&serializedValue, 1);
}
