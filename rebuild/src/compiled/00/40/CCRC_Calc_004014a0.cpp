// CCRC::Calc(unsigned long seed, const void* data, unsigned long byteCount) -- table-driven reflected CRC-32 (crc0: no pre/post inversion) of a byte run folded into `seed`
// Leaf: ?Calc@CCRC@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004014A0, 56 bytes. No callees; reads the 256-entry
// table at 0x0129A168.
//
// Retail bytes:  push esi ; mov esi,[byteCount] ; test esi,esi ; mov eax,ecx ; je X ; xor ecx,ecx ;
//   test esi,esi ; jbe X ; push ebx ; push edi ; L: movzx edi,byte [ecx+edx] ; mov ebx,eax ;
//   and ebx,0xff ; xor edi,ebx ; mov ebx,[edi*4+table] ; shr eax,8 ; xor eax,ebx ; inc ecx ;
//   cmp ecx,esi ; jb L ; pop edi ; pop ebx ; X: pop esi ; ret 4
// __fastcall (seed in ecx, data in edx, byteCount on the stack -> `ret 4`); the manifest records
// it as a static `Calc` with (ulong, void*, ulong). This is the engine's crc0 (docs: reflected
// poly 0xEDB88320, seed 0, no final inversion) step. The explicit `byteCount != 0` guard is kept
// separate from the loop test (both `test esi,esi` survive); the running value stays in eax.
// An earlier lane already landed this body as Global_CCRC_Calc_004014a0.cpp (fable_crc.h); this
// TU is self-contained.
extern unsigned long g_CRCTable[256];             // 0x0129A168

struct CCRC {
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long byteCount);
};

unsigned long __fastcall CCRC::Calc(unsigned long seed, const void* data, unsigned long byteCount)
{
    if (byteCount != 0)
    {
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        for (unsigned long index = 0; index < byteCount; ++index)
            seed = g_CRCTable[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
    }
    return seed;
}