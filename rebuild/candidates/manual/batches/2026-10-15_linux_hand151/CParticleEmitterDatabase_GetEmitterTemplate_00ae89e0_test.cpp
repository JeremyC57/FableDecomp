// behaviour test for 0x00AE89E0 CParticleEmitterDatabase::GetEmitterTemplate: bank path (null handle -> 0) or map lookup
#include <cstdio>
struct CEmitter { int id; };
static CEmitter g_bankEmitter = { 99 };
struct CPtr { CEmitter* Object; CPtr() : Object(0) {} };
struct CInfo { bool GetEmitterTemplate(CPtr& out, int index) { if (index < 0) return false; out.Object = &g_bankEmitter; return true; } };
struct CBank { CInfo info; CInfo* GetEmitterInfo(int) { return &info; } };
struct SEntry { int first; CEmitter* second; };
struct CDb {
    SEntry* first; SEntry* last; CBank* EmitterBank;
    SEntry* LowerBound(const int* key) { SEntry* p = first; while (p != last && p->first < *key) ++p; return p; }
    CEmitter* GetEmitterTemplate(int handle, int index)
    {
        if (EmitterBank) {
            if (handle) { CInfo* info = EmitterBank->GetEmitterInfo(handle); CPtr e; if (info->GetEmitterTemplate(e, index)) return e.Object; }
            return 0;
        }
        SEntry* it = LowerBound(&handle);
        if (it == last || handle < it->first) it = last;
        if (it == last) return 0;
        return it->second;
    }
};
int main()
{
    CEmitter a = { 1 }, b = { 2 };
    SEntry e[2] = { { 4, &a }, { 9, &b } };
    CDb db = { e, e + 2, 0 };
    CBank bank;
    bool ok = db.GetEmitterTemplate(9, 0) == &b && db.GetEmitterTemplate(5, 0) == 0 && db.GetEmitterTemplate(4, 0) == &a;
    db.EmitterBank = &bank;
    ok = ok && db.GetEmitterTemplate(0, 0) == 0 && db.GetEmitterTemplate(3, 0) == &g_bankEmitter && db.GetEmitterTemplate(3, -1) == 0;
    if (!ok) { std::printf("00ae89e0_TEST FAIL\n"); return 1; }
    std::printf("00ae89e0_TEST PASS\n");
    return 0;
}
