// 00c2da20 CSegmentFile::CSegmentFile (EH parent; its generated cleanup 0x01213BA0 lands as a separate funclet row that includes this file)
// Leaf: ??0CSegmentFile@@QAE@PAXK@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#include "libpk/CSegmentFile.h"
#include <string.h>
CSegmentFile::CSegmentFile(void* source,unsigned long chunkSize)
    : Reader(source, chunkSize), ErrorCount(0)
{
    if (Reader.SetRange(0, 8) < 0) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    char signature[9];
    signature[8] = 0;
    if (Reader.Read(signature, 8) < 8) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    if (strcmp(signature, "LiOnHeAd") != 0) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    unsigned long pos = 8;
    if (Reader.GetCount() > pos) {
        do {
            if (Reader.SetRange(pos, 0x24) < 0) {
                ++ErrorCount;
                break;
            }
            char name[0x21];
            name[0x20] = 0;
            if (Reader.Read(name, 0x20) < 0x20) {
                ++ErrorCount;
                Reader.SetRange(0, 0);
                return;
            }
            long segmentSize;
            if (Reader.Read(&segmentSize, 4) < 4) {
                ++ErrorCount;
                Reader.SetRange(0, 0);
                return;
            }
            if (pos + 0x24 + segmentSize > Reader.GetCount()) {
                ++ErrorCount;
                break;
            }
            CSegment* segment = new CSegment;
            segment->Offset = pos + 0x24;
            segment->Size = segmentSize;
            segment->Name = name;
            Segments.Add(segment);
            pos = segment->Offset + segment->Size;
        } while (pos < Reader.GetCount());
    }
    Reader.SetRange(0, 0);
}