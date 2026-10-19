extern "C" void* memset(void*, int, unsigned int);

void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

struct CBasicStringChar
{
    char* data;
    unsigned int len;
    unsigned int capField;

    void __fastcall SetString(void* /*edx_unused*/, unsigned int count, int ch);
};

void __fastcall CBasicStringChar::SetString(void* /*edx_unused*/, unsigned int count, int ch)
{
    if (data != 0)
    {
        operator delete[](data);
        data = 0;
    }

    unsigned int allocSize = (count + 4) & ~3u;
    char* newData = (char*)operator new[](allocSize);

    unsigned int mergedCap = capField ^ ((capField ^ allocSize) & 0x7fffffff);

    data = newData;
    capField = mergedCap;

    if (count > 0)
    {
        memset(newData, ch, count);
    }

    data[count] = 0;
    len = count;
    data[count] = 0;

    unsigned int padStart = len + 1;
    unsigned int alignedPadEnd = (padStart + 3) & ~3u;
    unsigned int padCount = alignedPadEnd - padStart;
    char* padPtr = data + padStart;

    memset(padPtr, 0, padCount);
}