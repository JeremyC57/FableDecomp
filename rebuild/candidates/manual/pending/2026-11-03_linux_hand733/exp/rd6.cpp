#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CFileDataInputStream.h"
void f(CAFile& file, CCharString& text, int n)
{
    for (int i = 0; i < n; ++i) {
    CFileDataInputStream stream(file, 0x4000);
    stream.ReadULONG();
    unsigned long length = stream.ReadULONG();
    text = stream.ReadString(length - stream.GetPosition());
    }
}
void k(CDataInputStream& s, unsigned long* out)
{
    out[0] = s.ReadULONG();
    out[1] = s.ReadULONG();
}
