// CRegistryKey::QueryDWord(const char* name, unsigned long* value) -- read a REG_DWORD value of the open key; TRUE only when the query succeeds, the type is REG_DWORD and four bytes came back
// Grade: DIFFER(72v72) -- block placement only: retail keeps the merged `xor eax,eax ; pop ecx ; ret 8` failure block right after the RegQueryValueExA test (`je` over it, the type test jumps back), ours puts it after the type test (the last failure) | tried: two separate ifs, `return type == 4 && size == 4`, `if (type == 4) return size == 4; return 0;`, the QFE-4035 compiler
// Leaf: ?QueryDWord@CRegistryKey
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73830, length 72 bytes, ret 8. Callee: ADVAPI32 RegQueryValueExA
//   (IAT 0x0143FBEC). Called three times by the audio driver probe 0x00C5DE5F.. (driver id / flags values).
// Layout (descriptive; libpk audio): CRegistryKey -- vtable 0x012BEFB0, HKEY +4 (see Create 0x00C737D0).
// Shape: the size word is the push-ecx local; the type local lives in the dead `value` argument slot (the
//   pointer is pushed first); the failure return is `xor eax,eax` shared by both early outs; the last test is
//   `size == 4` as an int (`xor eax,eax ; cmp ; sete al`).
typedef struct HKEY__* HKEY;
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(HKEY key, const char* name, unsigned long* reserved,
    unsigned long* type, unsigned char* data, unsigned long* size);

struct CRegistryKey {
    virtual void Dummy();                  // vtable 0x012BEFB0
    HKEY m_hKey;                           // +0x04
    int QueryDWord(const char* name, unsigned long* value);
};

int CRegistryKey::QueryDWord(const char* name, unsigned long* value)
{
    unsigned long type;
    unsigned long size = sizeof(unsigned long);
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0)
        return 0;
    if (type != 4)
        return 0;
    return size == sizeof(unsigned long);
}
