// Behaviour test for 0x009CC8B0: CBankFile::UpdateEntryInfoOnly -- read infoSize bytes from the stream (buffered fast path
// or the overflow reader), store them as the entry's info, clear RequiresUpdate, notify (data type, index, info).
#include <cstdio>
#include <stdlib.h>
#include <string.h>

class CDataInputStream { public:
    unsigned long StreamPos;
    const unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    int OverflowReads;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)   // stand-in: the same bytes, counted
    {
        ++OverflowReads;
        memcpy(buffer, CurrentSourcePtr, size);
        CurrentSourcePtr += size; StreamPos += size; SourceChunkBytesRemaining -= size;
    }
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        double newPosition = (double)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class CBankFileEntryUpdateData { public:
    unsigned long InfoSize;
    char* InfoBuffer;
    bool RequiresUpdate;
    void SetInfo(const char* data, unsigned long size)
    {
        if (InfoBuffer) { delete[] InfoBuffer; InfoBuffer = 0; }
        InfoSize = size;
        if (size) { InfoBuffer = new char[size]; memcpy(InfoBuffer, data, size); }
    }
};
class CBankFile { public:
    CBankFileEntryUpdateData* UpdateData[4];
    unsigned char DataType[4];
    unsigned long NotifiedType, NotifiedIndex, NotifiedSize;
    void NotifyEntryUpdated(unsigned long dataType, unsigned long index, unsigned long infoSize)
    { NotifiedType = dataType; NotifiedIndex = index; NotifiedSize = infoSize; }
    void UpdateEntryInfoOnly(unsigned long index, CDataInputStream& stream, unsigned long infoSize)
    {
        CBankFileEntryUpdateData* updateData = UpdateData[index];
        char* info = (char*)calloc(infoSize ? infoSize : 1, 1);
        stream.Read(info, infoSize);
        updateData->SetInfo(info, infoSize);
        updateData->RequiresUpdate = false;
        NotifyEntryUpdated(DataType[index], index, infoSize);
        free(info);
    }
};

int main()
{
    static const unsigned char bytes[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CDataInputStream s = { 0, bytes, 5, 0 };
    CBankFileEntryUpdateData e = { 0, 0, true };
    CBankFile bank;
    memset(&bank, 0, sizeof(bank));
    bank.UpdateData[2] = &e; bank.DataType[2] = 9;
    bank.UpdateEntryInfoOnly(2, s, 3);                         // buffered path
    if (e.InfoSize != 3 || e.InfoBuffer[2] != 3 || e.RequiresUpdate || s.StreamPos != 3 || s.OverflowReads != 0
        || bank.NotifiedType != 9 || bank.NotifiedIndex != 2 || bank.NotifiedSize != 3) { std::printf("009cc8b0_TEST FAIL\n"); return 1; }
    e.RequiresUpdate = true;
    bank.UpdateEntryInfoOnly(2, s, 4);                         // more than the chunk holds: the overflow reader
    if (e.InfoSize != 4 || e.InfoBuffer[0] != 4 || e.InfoBuffer[3] != 7 || e.RequiresUpdate || s.OverflowReads != 1) { std::printf("009cc8b0_TEST FAIL\n"); return 1; }
    bank.UpdateEntryInfoOnly(2, s, 0);                         // empty info: nothing read, buffer released
    if (e.InfoSize != 0 || e.InfoBuffer != 0 || s.StreamPos != 7) { std::printf("009cc8b0_TEST FAIL\n"); return 1; }
    std::printf("009cc8b0_TEST PASS\n");
    return 0;
}