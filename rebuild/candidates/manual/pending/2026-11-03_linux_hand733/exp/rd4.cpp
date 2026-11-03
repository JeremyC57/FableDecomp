#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CFileDataInputStream.h"
#pragma auto_inline(off)
unsigned long pre(CDataInputStream& s) { return s.ReadULONG(); }
#pragma auto_inline(on)
void f(CAFile& file, CCharString& text)
{
    CFileDataInputStream stream(file, 0x4000);
    stream.ReadULONG();
    unsigned long length = stream.ReadULONG();
    text = stream.ReadString(length - stream.GetPosition());
}
