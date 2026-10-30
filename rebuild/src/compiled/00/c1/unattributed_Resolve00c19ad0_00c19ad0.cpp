// CClothingSuitMap_00c19ad0::Resolve(void*, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, 0 when the key is not found (@ 0x00C196A0), else the entry resolved out of line with the two further arguments (@ 0x00C19760); the result survives the lock's destructor in esi
// Leaf: HeroExperience_QueryDamageWithRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19AD0, oracle length 160 bytes. Built under /EHsc.
// Retail bytes:  ... call Find ; test eax,eax ; jne hit ; ... xor eax,eax ; ... ret 0Ch ; hit: mov ecx,[b] ; mov edx,[a] ; push ecx ; push edx ; push eax ; mov ecx,esi ; call ResolveEntry ; lea ecx,[esp+4] ; mov esi,eax ; ...
// The three-argument sibling of 0x00C19B70 / 0x00C19C10 (miss value 0). The manifest label is
// propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CEntry_00c19ad0;

struct CClothingSuitMap_00c19ad0 {
    CEntry_00c19ad0* Find(void* key);      // 0x00C196A0
    long ResolveEntry(CEntry_00c19ad0* entry, void* a, void* b);   // 0x00C19760
    long Resolve(void* key, void* a, void* b);
};

long CClothingSuitMap_00c19ad0::Resolve(void* key, void* a, void* b)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CEntry_00c19ad0* entry = Find(key);
    if (!entry)
        return 0;
    return ResolveEntry(entry, a, b);
}