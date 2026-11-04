// Behaviour test for CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate (0x00AF12A0): a model of the row -- a stream over
// the file data (pointer, length) is read by a newly built template, then the file data's last reference is released.
#include <stdio.h>
static int g_deleted, g_read;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteBank(void*) { ++g_deleted; }
struct CBankFileAsyncData { const char* Data; unsigned long Length; const void* PeekData() const { return Data; } unsigned long Sizeof() const { return Length; } };
struct Stream { const void* Data; unsigned long Size; Stream(const void* d, unsigned long s) : Data(d), Size(s) {} };
struct CParticleEmitter { unsigned long BytesRead; CParticleEmitter() : BytesRead(0) {} void ReadBinary(Stream& s) { BytesRead = s.Size; ++g_read; } };
template <class T> struct CCountedPointer {
    T* Data; Info* PInfo;
    void ReleaseData() { if (PInfo) { if (--PInfo->RefCount == 0) { PInfo->DeleteFunc(PInfo->Data); delete PInfo; } } PInfo = 0; Data = 0; }
    void Reset(T* d) { ReleaseData(); Data = d; if (d) { PInfo = new Info; PInfo->RefCount = 1; PInfo->DeleteFunc = 0; PInfo->Data = d; } }
    T* operator->() const { return Data; }
};
struct CEmitterData {
    CCountedPointer<CBankFileAsyncData> FileData; CCountedPointer<CParticleEmitter> EmitterTemplate;
    void BuildEmitterTemplate()
    {
        Stream stream(FileData->PeekData(), FileData->Sizeof());
        EmitterTemplate.Reset(new CParticleEmitter);
        EmitterTemplate->ReadBinary(stream);
        FileData.ReleaseData();
    }
};
int main()
{
    CBankFileAsyncData bank = { "abcdef", 6 };
    CEmitterData e; e.FileData.Data = &bank; e.FileData.PInfo = new Info; e.FileData.PInfo->RefCount = 1;
    e.FileData.PInfo->DeleteFunc = DeleteBank; e.FileData.PInfo->Data = &bank;
    e.EmitterTemplate.Data = 0; e.EmitterTemplate.PInfo = 0;
    e.BuildEmitterTemplate();
    int ok = e.EmitterTemplate.Data && e.EmitterTemplate.Data->BytesRead == 6 && g_read == 1 && g_deleted == 1
        && e.FileData.Data == 0 && e.FileData.PInfo == 0;
    printf(ok ? "00af12a0_TEST PASS\n" : "00af12a0_TEST FAIL\n");
    return ok ? 0 : 1;
}