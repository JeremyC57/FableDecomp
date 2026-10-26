#pragma optimize("s",on)
struct T { char raw[0x70]; };

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)

void __stdcall Fill(T** begin, T** end)
{
    for (T** p = begin; p < end; ++p)
    {
        *p = (T*)malloc(0x70);
    }
}