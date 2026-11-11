// Leaf: ?SetSubstring@CWideString@@
// Name debt: retail 0x0099BDB0 is ?GetSubString@CWideString@@QBE?AV1@KK@Z (Ego_r / FableWin; fable_string.h FABLE_RETAIL_CWIDESTRING_SUBSTRING).
//   This stand-in keeps the return pointer as an explicit parameter; the real by-value spelling (a named local copied out) grades
//   195v179 / 198v179 because the local is given a frame slot where retail keeps its storage in esi (queued land1011m).
extern "C" unsigned short g_emptyWideStringData; // 0x00129a8e0 (empty wide-string storage code unit)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CWideStringData
{ public:
    unsigned short* data;
    long unused1;
    long unused2;
    long refCount; // offset +0xC
};

class CWideString
{ public:
    CWideStringData* storage;
    CWideString* SetSubstring(CWideString* retVal, unsigned long start, unsigned long len);
protected:
    CWideStringData* AllocStringData(const unsigned short* s, long n);   // 0x0099B3C0 protected (Ego_r / FableWin: IAE)
    void UnassignString_0099b4d0();
protected:
    static long NoStrings;                              // 0x013BCA20 ?NoStrings@CWideString@@1JA (protected static instance counter)
};

// Real implementations live at 0x0099b3c0 (AllocStringData) / 0x0099b4d0 (UnassignString);
// they are separate retail functions and are intentionally left undefined here (external
// callees only) so this translation unit authors ONLY SetSubstring itself.

// CWideString SetSubstring(CWideString* this, ulong start, ulong len)
// Hidden-return-pointer thiscall: ecx = this, [esp+4]=retptr, [esp+8]=start, [esp+0xc]=len
CWideString* CWideString::SetSubstring(CWideString* retVal, unsigned long start, unsigned long len)
{
    CWideStringData* newStorage = 0;
    CWideString localTmp;

    ++NoStrings;

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
    ++NoStrings;

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
    --NoStrings;

    return retVal;
}