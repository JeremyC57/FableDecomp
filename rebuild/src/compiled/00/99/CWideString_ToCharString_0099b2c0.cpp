// CWideString::ToCharString() const -- the narrow copy of the wide text: each UTF-16 code unit truncated to a char, into an owned temporary buffer
// Leaf: ?ToCharString@CWideString@@QBE?AVCCharString@@XZ
// Retail 0x0099B2C0, 177 bytes, ending ret 4 at 0x0099B36E. malloc 0x00BFEA0E / free 0x00BFEA14, CCharString text/length constructor
// 0x0099EBF0. Ego_r.pdb 0x00B02850 / FableWin.pdb 0x02F67B80 ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (ConvertToCharString is in
// neither PDB); fable_string.h declares it and the landed 0x005CC772 binds it.
// NF80 (q10939): moved off the CWideStringConversionView / CWideVectorStorageView stand-ins onto fable_string.h's retail CWideString
//   (FABLE_RETAIL_CWIDESTRING): FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY gives the inline GetLength / GetAsCharArray over
//   CWideStringData::GetLength / PeekStringData (Ego_r.pdb) that retail expands here (each length re-tests the data pointer; the null
//   text is the literal L"" 0x0129A8E0, not the old named-address stand-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY
#include "fable_string.h"
#include <stdlib.h>
#include <string.h>
struct CTemporaryNarrowBuffer {
    char* Text;
    CTemporaryNarrowBuffer(long size) : Text(size ? (char*)malloc(size) : 0) {
        memset(Text, 0, size);
    }
    ~CTemporaryNarrowBuffer() { if (Text) free(Text); }
};
CCharString CWideString::ToCharString() const
{
    CTemporaryNarrowBuffer buffer(GetLength() + 1);
    for (long i = 0; i < GetLength(); ++i)
        buffer.Text[i] = (char)GetAsCharArray()[i];
    buffer.Text[GetLength()] = 0;
    return CCharString(buffer.Text, -1);
}
