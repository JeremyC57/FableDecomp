// CVtable_012b2f98::Hash(const char* key) -- hash a C-string key: the Jenkins hash of its characters (seed 0), or 0xFFFFFFFF for a null key
// Leaf: ?Hash@CVtable_012b2f98@@UAEKPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19970, 45 bytes, thiscall, `ret 4` on the null path; otherwise the inlined strlen
//   loop, then a tail jump into the __fastcall JenkinsHashBuffer with the seed 0 written over the key's argument slot.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B2F98 (the libpk string-keyed hash container's hasher);
//   libpk has no RTTI or PDB, so the class is named by its vtable. The callee is the hash the landed CString::GetHash
//   0x00C0DBB0 calls the same way.
// Callee: JenkinsHashBuffer(const char*, unsigned long, unsigned long) __fastcall 0x00C1D500.
#include <string.h>

unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed);   // 0x00C1D500

class CVtable_012b2f98 {
public:
    virtual unsigned long Hash(const char* key);   // slot 0
};

unsigned long CVtable_012b2f98::Hash(const char* key)
{
    if (!key)
        return 0xFFFFFFFF;
    return JenkinsHashBuffer(key, strlen(key), 0);
}