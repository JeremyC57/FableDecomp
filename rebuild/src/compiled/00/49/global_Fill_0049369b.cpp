#pragma optimize("s",on)
struct T { char raw[0x70]; };

void* __cdecl malloc(unsigned int);

void __stdcall Fill(T** begin, T** end)
{
    for (T** p = begin; p < end; ++p)
    {
        *p = (T*)malloc(0x70);
    }
}