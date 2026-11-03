#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CFileDataInputStream.h"
unsigned long pre(CDataInputStream& s) { return s.ReadULONG(); }
void f(CAFile& file, CCharString& text)
{
    CFileDataInputStream stream(file, 0x4000);
    stream.ReadULONG();
    unsigned long length = stream.ReadULONG();
    text = stream.ReadString(length - stream.GetPosition());
}
