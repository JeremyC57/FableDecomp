// CRegistryKey::Close() -- close the wrapped registry key (+4) with RegCloseKey when one is open, and forget it
// Leaf: ?Close@CRegistryKey@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73800, 26 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je done ; push eax ; call [RegCloseKey] ;
//   mov dword ptr [esi+4],0 ; done: pop esi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC73790/0xC739A0/0xC73800 = CRegistryKey ctor/dtor/Close"):
//   - agent444's landed CRegistryKey::Create 0x00C737D0 keeps RegCreateKeyExA's key at +4 of the same object (vtable
//     0x012BEFB0); 0x00C737A0 opens into +4 with RegOpenKeyExA and 0x00C73830 (agent444's QueryDWord) reads through it;
//   - the vtable's only slot is the scalar deleting destructor 0x00C739C0 (the destructor's close inlined, then the CRT's
//     0x00BFE9BC); the users are the device registry routine 0x00C5DE14.. (constructor, Create/QueryDWord, destructor).
//   Imports: RegCloseKey (IAT 0x0143FBFC), RegOpenKeyExA (0x0143FC08), RegCreateKeyExA (0x0143FBF0).
//   The landed CBaseIntelligentPointer / CLandscapeBackgroundPatch::ClearTextures names are propagated; descriptive names
//   (libpk: no PDB/RTTI), agent444's class.
//   No direct caller remains (the destructor 0x00C739A0 and its deleting form 0x00C739C0 carry the same close inline).
typedef struct HKEY__* HKEY;
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY key);   // IAT 0x0143FBFC

class CRegistryKey {                                    // vtable 0x012BEFB0
public:
    virtual ~CRegistryKey();                            // 0x00C739A0
    HKEY m_hKey;                                        // +0x04
    void Close();
};

void CRegistryKey::Close()
{
    if (m_hKey) {
        RegCloseKey(m_hKey);
        m_hKey = 0;
    }
}
