extern "C" long g_charStringRefTrace; // 0x013bca20
// Leaf: SetSubstring
extern "C" unsigned short g_emptyWideStringData; // 0x00129a8e0 (empty wide-string storage code unit)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CWideStringData
{ public:
    unsigned short* data;
    long unused1;
    long unused2;
    long refCount; // offset +0xC
};

class CCharString
{ public:
    CWideStringData* storage;

    CWideStringData* AllocStringData(unsigned short* s, long n);
    void UnassignString_0099b4d0();
    CCharString* SetSubstring(CCharString* retVal, unsigned long start, unsigned long len);
};

// Real implementations live at 0x0099b3c0 (AllocStringData) / 0x0099b4d0 (UnassignString);
// they are separate retail functions and are intentionally left undefined here (external
// callees only) so this translation unit authors ONLY SetSubstring itself.

// CCharString SetSubstring(CCharString* this, ulong start, ulong len)
// Hidden-return-pointer thiscall: ecx = this, [esp+4]=retptr, [esp+8]=start, [esp+0xc]=len
CCharString* CCharString::SetSubstring(CCharString* retVal, unsigned long start, unsigned long len)
{
    CWideStringData* newStorage = 0;
    CCharString localTmp;

    ++g_charStringRefTrace;

    CWideStringData* storage = this->storage;
    unsigned short* base;
    if (!storage)
        base = &g_emptyWideStringData;
    else
        base = storage->data;

    unsigned short* subPtr = base + start;
    if (subPtr == 0)
        goto skipAlloc;
    if (*subPtr == 0)
        goto skipAlloc;
    {
        long subLen = (long)len - (long)start;
        newStorage = localTmp.AllocStringData(subPtr, subLen);
    }
skipAlloc:

    retVal->storage = 0;
    ++g_charStringRefTrace;

    if ((void*)&localTmp != (void*)retVal)
    {
        CWideStringData* oldRetStorage = retVal->storage;
        if (newStorage != oldRetStorage)
        {
            if (oldRetStorage != 0)
            {
                retVal->UnassignString_0099b4d0();
            }
            if (newStorage != 0)
            {
                retVal->storage = newStorage;
                ++newStorage->refCount;
            }
        }
    }

    if (newStorage != 0)
    {
        if (--newStorage->refCount <= 0)
        {
            if (newStorage->data != 0)
                free(newStorage->data);
            operator delete(newStorage);
        }
    }
    --g_charStringRefTrace;

    return retVal;
}