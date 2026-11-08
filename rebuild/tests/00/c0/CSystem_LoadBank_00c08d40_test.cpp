// Behaviour test for CSystem::LoadBank (0x00C08D40): the bank's lock sources come from the params (1 = the system's
// locks, 2 = the bank's own), the size is the system bank size rounded up to the bank's alignment, every lock taken is
// released after the load, a failed load or an unresolved .lug/.lut handle deletes the bank and returns null, and a loaded
// bank is handed over (with the handle for .lug/.lut).
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>

static int g_deleted = 0, g_lockDepth = 0, g_maxDepth = 0;
struct LockPair { int held; void Lock() { ++held; ++g_lockDepth; if (g_lockDepth > g_maxDepth) g_maxDepth = g_lockDepth; } void Unlock() { --held; --g_lockDepth; } };
struct Bank {
    long m_handle; LockPair m_locks; int held;
    Bank() : m_handle(0) { m_locks.held = 0; held = 0; }
    ~Bank() { ++g_deleted; }
    unsigned long GetAlignment() const { return 0x800; }
    void Lock() { ++held; } void Unlock() { --held; }
};
template <class T> struct PKCAutoDelete {
    T* m_pObject;
    PKCAutoDelete(T* o) : m_pObject(o) { }
    ~PKCAutoDelete() { if (m_pObject) delete m_pObject; }
    T* GetChecked() { return m_pObject; }
    T* Release() { T* o = m_pObject; m_pObject = 0; return o; }
};
struct Params { long m_sourceA, m_sourceB; unsigned char m_8, m_9, m_a, p0, m_c, m_d, m_e, p1; };
struct Desc {
    LockPair* m_pLocksA; LockPair* m_pLocksB; unsigned long m_size; unsigned char m_0c, m_0d, m_0e, m_0f, m_10, m_11;
    Desc() : m_pLocksA(0), m_pLocksB(0), m_size(0x1000) { }
};
struct Loader {
    bool ok; long handle; Desc seen; int locksHeldAtLoad;
    long OpenFile(const unsigned short*) { return handle; }
    bool Load(const unsigned short*, Bank* bank, Desc* desc) { seen = *desc; locksHeldAtLoad = g_lockDepth + bank->held; return ok; }
};
static std::string Narrow(const unsigned short* w) { std::string s; while (*w) s += (char)*w++; return s; }
static int CompareNoCase(const std::string& a, const char* b)
{ std::string x = a, y = b; for (size_t i = 0; i < x.size(); ++i) x[i] = (char)std::tolower(x[i]); return x == y ? 0 : 1; }
struct System {
    LockPair m_locks; Loader* m_pLoader; unsigned long m_bankSize;
    Bank* LoadBank(const unsigned short* fileName, const Params& params)
    {
        Bank* bank = new Bank;
        PKCAutoDelete<Bank> autoBank(bank);
        Desc desc;
        if (params.m_sourceB == 1) desc.m_pLocksB = &m_locks; else if (params.m_sourceB == 2) desc.m_pLocksB = &bank->m_locks;
        if (params.m_sourceA == 1) desc.m_pLocksA = &m_locks; else if (params.m_sourceA == 2) desc.m_pLocksA = &bank->m_locks;
        desc.m_0c = params.m_8; desc.m_0d = params.m_9; desc.m_0e = params.m_a; desc.m_0f = params.m_c; desc.m_10 = params.m_d; desc.m_11 = params.m_e;
        unsigned long alignment = bank->GetAlignment();
        desc.m_size = (m_bankSize + alignment - 1) / alignment * alignment;
        if (desc.m_pLocksA) desc.m_pLocksA->Lock();
        if (desc.m_pLocksB) desc.m_pLocksB->Lock();
        bank->Lock();
        bool loaded = m_pLoader->Load(fileName, autoBank.GetChecked(), &desc);
        bank->Unlock();
        if (desc.m_pLocksA) desc.m_pLocksA->Unlock();
        if (desc.m_pLocksB) desc.m_pLocksB->Unlock();
        if (!loaded) return 0;
        std::string name = Narrow(fileName);
        std::string ext = name.size() >= 4 ? name.substr(name.size() - 4) : name;
        if (CompareNoCase(ext, ".lug") == 0 || CompareNoCase(ext, ".lut") == 0) {
            long handle = m_pLoader->OpenFile(fileName);
            if (handle == 0) return 0;
            bank->m_handle = handle;
        }
        return autoBank.Release();
    }
};
static void W(unsigned short* d, const char* s) { while ((*d++ = (unsigned char)*s++)) { } }

int main()
{
    Loader ld; ld.ok = true; ld.handle = 0x99;
    System sys; sys.m_locks.held = 0; sys.m_pLoader = &ld; sys.m_bankSize = 0x1801;
    Params p = { 1, 2, 1, 2, 3, 0, 4, 5, 6, 0 };
    unsigned short name[32];
    W(name, "music.LUG");
    Bank* b = sys.LoadBank(name, p);
    bool ok = b && b->m_handle == 0x99 && ld.seen.m_size == 0x2000 && ld.seen.m_pLocksA == &sys.m_locks && ld.seen.m_pLocksB == &b->m_locks
           && ld.seen.m_0c == 1 && ld.seen.m_0f == 4 && ld.seen.m_11 == 6 && ld.locksHeldAtLoad == 3 && g_lockDepth == 0 && sys.m_locks.held == 0;
    delete b; g_deleted = 0;
    W(name, "speech.met");
    Params q = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    b = sys.LoadBank(name, q);
    ok = ok && b && b->m_handle == 0 && ld.seen.m_pLocksA == 0 && ld.seen.m_pLocksB == 0 && g_deleted == 0;
    delete b; g_deleted = 0;
    ld.handle = 0; W(name, "x.lut");
    ok = ok && sys.LoadBank(name, q) == 0 && g_deleted == 1;
    ld.ok = false; W(name, "x.met");
    ok = ok && sys.LoadBank(name, p) == 0 && g_deleted == 2 && g_lockDepth == 0;
    if (!ok) { std::printf("00c08d40_TEST FAIL\n"); return 1; }
    std::printf("00c08d40_TEST PASS\n");
    return 0;
}