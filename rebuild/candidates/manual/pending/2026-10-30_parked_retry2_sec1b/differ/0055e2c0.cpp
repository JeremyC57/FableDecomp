// CryptoMatic(const char* source, char* destination, int length, const char* key, char mode) -- (anonymous namespace) XTEA buffer cipher of the LHNetworkLib crypto TU: derive a byte key schedule from the key string, then encipher (mode 0: length prefix written first) or decipher (mode 1: length prefix read and sanity-clamped) the buffer in 8-byte morsels, perturbing the 128-bit key after each morsel; a short tail is zero-padded into one more morsel. Returns the encrypted size (mode 0) or the stored plain length (mode 1)
// Grade: DIFFER(549v549) (sec1b 2026-10-30) -- result=0 / scheduleIndex=4 declared before the strlen (gives retail's length and register map: 210 -> 26 difflines) and the four key words assigned in retail's order k[2],k[1],k[3],k[0] (23). Left: retail runs the inline strlen before the callee-saved pushes and does the two inits after memset (ours does them after the pushes); declaring them uninitialised and assigning after memset (545, 210 lines), or moving either declaration later (527/561) is worse. Next: a shape that keeps the inits late but the pushes sunk
// Leaf: ?CryptoMatic@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E2C0, 549 bytes, __fastcall: ecx = source, edx = destination, then length,
//   key, mode on the stack (`ret 0Ch`).
// Identity (proven): Ego_r.pdb and FableWin.pdb ?CryptoMatic@?A0xf010c0d0@@YIHPBDPADH0D@Z (int __fastcall
//   (const char*, char*, int, const char*, char)), between DecipherMorsel and EncryptBuffer in both PDBs' donor order,
//   exactly as retail places it (0x0055E180 DecipherMorsel, 0x0055E2C0 this, 0x0055E4F0 EncryptBuffer = mode 0,
//   0x0055E510 DecryptBuffer = mode 1). The catalog name "CryptoMatic" is right.
// Callees: operator new[] 0x00BFEB22 / operator delete[] 0x00BFEB1C (the key schedule bytes), EncipherMorsel 0x0055E040
//   and DecipherMorsel 0x0055E180 (__fastcall: in block, out block, key on the stack). strlen and memset/memcpy are the
//   /O2 intrinsics (inline scan, rep stos, rep movs).
// Shape: the morsel loop counts `length / 8` down in the dead key slot; the 4-dword key holds the schedule bytes in
//   reverse order; the tail block shares the source pointer's frame words.
extern "C" unsigned int __cdecl strlen(const char* s);
extern "C" void* __cdecl memset(void* d, int c, unsigned int n);
extern "C" void* __cdecl memcpy(void* d, const void* s, unsigned int n);
#pragma intrinsic(strlen, memset, memcpy)
void* __cdecl operator new[](unsigned int size);       // 0x00BFEB22
void __cdecl operator delete[](void* p);               // 0x00BFEB1C

namespace {
void __fastcall EncipherMorsel(unsigned long* in, unsigned long* out, unsigned long* key);   // 0x0055E040
void __fastcall DecipherMorsel(unsigned long* in, unsigned long* out, unsigned long* key);   // 0x0055E180

int __fastcall CryptoMatic(const char* source, char* destination, int length, const char* key, char mode)
{
    int result = 0;
    unsigned int scheduleIndex = 4;
    unsigned int keyLength = strlen(key);
    char* schedule = new char[keyLength + 1];
    memset(schedule, 0, keyLength + 1);
    for (unsigned int i = 0; i < keyLength; ++i)
        schedule[i] = (char)(i + 1) * key[keyLength - i];

    unsigned long k[4];
    k[2] = schedule[1];
    k[1] = schedule[2];
    k[3] = schedule[0];
    k[0] = schedule[3];

    char* start = destination;
    if (mode == 0) {
        *(int*)destination = length;
        destination += 4;
    }
    else {
        result = *(const int*)source;
        if (result > length + 10) {
            length -= length % 8;
            result = length;
        }
        source += 4;
        length -= 4;
    }

    while (length >= 8) {
        {
            if (mode == 0)
                EncipherMorsel((unsigned long*)source, (unsigned long*)destination, k);
            else
                DecipherMorsel((unsigned long*)source, (unsigned long*)destination, k);
            source += 8;
            destination += 8;
            if (scheduleIndex >= keyLength)
                scheduleIndex = 0;
            k[0] += schedule[scheduleIndex++];
            if (scheduleIndex >= keyLength)
                scheduleIndex = 0;
            k[1] -= schedule[scheduleIndex++];
            if (scheduleIndex >= keyLength)
                scheduleIndex = 0;
            k[2] += schedule[scheduleIndex++];
            if (scheduleIndex >= keyLength)
                scheduleIndex = 0;
            k[3] -= schedule[scheduleIndex++];
        }
        length -= 8;
    }

    if (length > 0) {
        unsigned long block[2];
        block[0] = 0;
        block[1] = 0;
        memcpy(block, source, length);
        if (mode == 0)
            EncipherMorsel(block, (unsigned long*)destination, k);
        else
            DecipherMorsel(block, (unsigned long*)destination, k);
        destination += 8;
    }

    if (mode == 0)
        result = destination - start;
    delete[] schedule;
    return result;
}
}

int (__fastcall *g_keepCryptoMatic)(const char*, char*, int, const char*, char) =
    &CryptoMatic;   // keeps the internal-linkage body emitted; not part of the retail function
