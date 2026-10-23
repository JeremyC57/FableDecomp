// Grade: DIFFER(945v937) -- structure and every call match in order; residue is register/frame allocation: retail keeps this in ebx, the process in ebp and a zero in esi, copies the group count into its own down-counter slot and keeps the entity size in ebp across groups (frame 0x94 vs ours 0x8c)
// CThingManager::LoadEntitiesBinary(CMemoryDataInputStream&, long, long, const set<CCharString>&, CASuspendableProcess*,
// long, vector<CThing*>&) -- read the entity-group count; for each group read its name and size-prefixed block, and
// when the name (or the global "all groups" name) is in the wanted set, read size-prefixed entity blocks until a zero
// size, loading each through LoadSingleThing into the output vector and ticking the suspendable process every
// interval; finally tick the process once more and activate the loaded things
// Leaf: ?LoadEntitiesBinary@CThingManager@@QAEXAAVCMemoryDataInputStream@@JJABV?$set@VCCharString@@...
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521240, `ret 0x1c`.
// Callees: CCharString operator=(const char*) 0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0 / copy 0x0099EC30 /
//   0x0099EBF0 / dtor 0x0099EAE0, the stream's slots 2 (+0x8, position) and 6 (+0x18, a CCharString by value),
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 / SetPositionAssumeWithinSourceChunk 0x00993C10,
//   CMemoryDataInputStream ctor 0x00994930 / dtor 0x00993B10, set find-node 0x004B9370, NProgressDisplay::DisplayProgress
//   0x009E9F40 (__fastcall), CPersistContext(stream, bool) 0x009BAE30, LoadSingleThing 0x0051FD80, the vector's grow
//   0x00436AC0, the process's slot 2 (+0x8), ActivateAfterLoading 0x0051E5A0.
// Layout: retail CThingManager +0xd8 / +0xdc the two longs, +0xe0 the current group name; CMemoryDataInputStream data
//   base +0x18 (0x20 bytes). Globals: tick interval 0x01375714, the all-groups name 0x013BAB08, progress text 0x01245AF4.
//   Inline stream read as the landed 0x00B16F60.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
#include <new>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CDataInputStream {
    virtual void s00();
    virtual void s01();
    virtual unsigned long GetPosition();                // slot 2 (+0x8)
    virtual void s03(); virtual void s04(); virtual void s05();
    virtual CCharString ReadCharString();               // slot 6 (+0x18)
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void SetPositionAssumeWithinSourceChunk(unsigned long position);   // 0x00993C10
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
struct CMemoryDataInputStream : CDataInputStream {
    const unsigned char* Data;                          // +0x18
    unsigned char _pad1c[0x20 - 0x1c];
    CMemoryDataInputStream(const unsigned char* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                          // 0x00993B10
    __forceinline const unsigned char* SkipBlock(long size)
    {
        if (size > 0) {
            unsigned long position = GetPosition();
            __int64 np = (__int64)(position + size);
            if (IsValidPosition(np)) {
                const unsigned char* block = Data + position;
                SetPositionAssumeWithinSourceChunk(position + size);
                return block;
            }
        }
        return 0;
    }
};
struct CStringSetNode;
struct CStringSet {                                     // std::set<CCharString>: header node first
    CStringSetNode* Header;
    CStringSetNode* FindNode(const CCharString& key) const;   // 0x004B9370
    bool Contains(const CCharString& key) const { return FindNode(key) != Header; }
};
struct SScalarTag { SScalarTag() { } };
class CThing;
struct CThingVector {
    CThing** first;
    CThing** last;
    CThing** end;
    void InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(CThing* const& value)
    {
        if (last != end) {
            new (last) CThing*(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CASuspendableProcess {
    virtual void p00(); virtual void p01();
    virtual void Tick();                                // slot 2 (+0x8)
};
struct CPersistContext {
    unsigned char _body[0x1c];
    CPersistContext(CDataInputStream* stream, bool loading);   // 0x009BAE30
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
extern unsigned long g_LoadTickInterval;                // 0x01375714
extern CCharString g_AllEntityGroupsName;               // 0x013BAB08

class CThingManager {
public:
    unsigned char _pad00[0xd8];
    long          LoadParam0;                           // +0xd8
    long          LoadParam1;                           // +0xdc
    CCharString   CurrentGroupName;                     // +0xe0
    CThing* LoadSingleThing(CPersistContext& context, long mode);   // 0x0051FD80
    void ActivateAfterLoading(long mode);               // 0x0051E5A0
    void LoadEntitiesBinary(CMemoryDataInputStream& in, long param0, long param1, const CStringSet& groups,
                            CASuspendableProcess* process, long mode, CThingVector& things);
};

void CThingManager::LoadEntitiesBinary(CMemoryDataInputStream& in, long param0, long param1, const CStringSet& groups,
                                       CASuspendableProcess* process, long mode, CThingVector& things)
{
    unsigned long interval = g_LoadTickInterval;
    unsigned long ticks = 0;
    bool hasProcess = process != 0;
    LoadParam0 = param0;
    LoadParam1 = param1;
    CurrentGroupName = "";
    unsigned long groupCount;
    in.Read(&groupCount, sizeof(groupCount));
    long entitySize;
    for (; groupCount; --groupCount) {
        CCharString name = in.ReadCharString();
        long groupSize;
        in.Read(&groupSize, sizeof(groupSize));
        CMemoryDataInputStream group(in.SkipBlock(groupSize), groupSize);
        if (groups.Contains(name) || groups.Contains(CCharString(g_AllEntityGroupsName))) {
            CurrentGroupName = name;
            for (;;) {
                group.Read(&entitySize, sizeof(entitySize));
                if (!entitySize)
                    break;
                NProgressDisplay::DisplayProgress(CCharString("Loading entities from script"), -1.0f, false, false);
                CMemoryDataInputStream entity(group.SkipBlock(entitySize), entitySize);
                CPersistContext context(&entity, true);
                CThing* thing = LoadSingleThing(context, mode);
                if (thing)
                    things.push_back(thing);
                if (++ticks >= interval) {
                    ticks = 0;
                    if (hasProcess)
                        process->Tick();
                }
            }
        }
    }
    if (process)
        process->Tick();
    ActivateAfterLoading(param1);
}
