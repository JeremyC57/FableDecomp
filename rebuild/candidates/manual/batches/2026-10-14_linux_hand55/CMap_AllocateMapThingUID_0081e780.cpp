// CMap::AllocateMapThingUID() -- return the 64-bit UID counter at +0x1d30 and advance it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E780, length 39 bytes.
// Shape: the counter in esi:edi, the old value copied to eax:edx, `add ; adc 0`, the high word stored first, `ret`.
struct CMap {
    unsigned char    _pad[0x1d30];
    unsigned __int64 m_nextThingUID;               // +0x1d30
    unsigned __int64 AllocateMapThingUID();
};

unsigned __int64 CMap::AllocateMapThingUID()
{
    return m_nextThingUID++;
}
