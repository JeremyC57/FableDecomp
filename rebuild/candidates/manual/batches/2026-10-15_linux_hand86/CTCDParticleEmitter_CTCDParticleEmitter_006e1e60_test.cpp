// Behaviour test for CTCDParticleEmitter::~CTCDParticleEmitter() @ 0x006E1E60: the pending creation parameters are released
// (deleter + block free only on the last reference), then AttachmentPosName, PAttachedToThing, PEmitter and EmitterName die
// in that order, then the CTCBase destructor runs.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CParticleCreationParameters { int id; };
static int g_order = 0;
static int g_deleted = 0;
static void* g_deletedData = 0;
static int g_seq[32];
static int g_seqCount = 0;
struct CTCBase_Calls {
    void* __vftable; void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    ~CTCBase_Calls() { g_seq[g_seqCount++] = 9; }
};
struct CCharString { int tag; ~CCharString() { g_seq[g_seqCount++] = tag; } };
struct CIntelligentPointer { void* __vftable; int tag; ~CIntelligentPointer() { g_seq[g_seqCount++] = tag; } };
struct SCountedBlock { long Count; void (__fastcall* DeleteData)(void* data); void* Data; };
static void __fastcall DeleteParams(void* data) { ++g_deleted; g_deletedData = data; }
template<class T> struct CCountedPointer {
    T* Object; SCountedBlock* PBlock;
    ~CCountedPointer()
    {
        if (PBlock != 0) {
            --PBlock->Count;
            if (PBlock->Count == 0) {
                PBlock->DeleteData(PBlock->Data);
                delete PBlock;
            }
        }
        Object = 0;
        PBlock = 0;
    }
};
struct CTCDParticleEmitter : CTCBase_Calls {
    CCharString EmitterName; long ParticleTypeID; CIntelligentPointer PEmitter; CIntelligentPointer PAttachedToThing;
    unsigned long AttachmentFlags; CCharString AttachmentPosName; long AttachmentPosIndex; float FloatOffset;
    long DieOffTimer; long FramesLeftToWaitForParticleToCreate;
    CCountedPointer<CParticleCreationParameters> PCreationParameters;
    unsigned char Flags44; unsigned char Flags45;
    ~CTCDParticleEmitter();
};
CTCDParticleEmitter::~CTCDParticleEmitter() { }
int main()
{
    CParticleCreationParameters params;
    SCountedBlock* shared = new SCountedBlock; shared->Count = 2; shared->DeleteData = DeleteParams; shared->Data = &params;
    {
        CTCDParticleEmitter e;
        e.EmitterName.tag = 1; e.PEmitter.tag = 2; e.PAttachedToThing.tag = 3; e.AttachmentPosName.tag = 4;
        e.PCreationParameters.Object = &params; e.PCreationParameters.PBlock = shared;
    }
    if (shared->Count != 1 || g_deleted != 0 || g_seqCount != 5 || g_seq[0] != 4 || g_seq[1] != 3 || g_seq[2] != 2
        || g_seq[3] != 1 || g_seq[4] != 9) { std::printf("006e1e60_TEST FAIL\n"); return 1; }
    {
        CTCDParticleEmitter e;
        e.EmitterName.tag = 1; e.PEmitter.tag = 2; e.PAttachedToThing.tag = 3; e.AttachmentPosName.tag = 4;
        e.PCreationParameters.Object = &params; e.PCreationParameters.PBlock = shared;   // last reference: freed here
    }
    if (g_deleted != 1 || g_deletedData != &params || g_seqCount != 10) { std::printf("006e1e60_TEST FAIL\n"); return 1; }
    {
        CTCDParticleEmitter e;                               // no block: nothing released
        e.EmitterName.tag = 1; e.PEmitter.tag = 2; e.PAttachedToThing.tag = 3; e.AttachmentPosName.tag = 4;
        e.PCreationParameters.Object = 0; e.PCreationParameters.PBlock = 0;
    }
    if (g_deleted != 1 || g_seqCount != 15) { std::printf("006e1e60_TEST FAIL\n"); return 1; }
    std::printf("006e1e60_TEST PASS\n");
    return 0;
}
