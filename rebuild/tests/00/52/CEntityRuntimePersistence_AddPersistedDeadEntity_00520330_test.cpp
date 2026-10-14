#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_inserts = 0; static u64 g_key = 0; static u64 g_uid = 0; static bool g_gp = false; static unsigned long g_crc = 0;
struct CPersistedDeadEntity { u64 UID; bool GamePersistent; unsigned long SectionNameCRC; };
struct SDeadEntityPair { u64 first; CPersistedDeadEntity second; SDeadEntityPair(u64 key, const CPersistedDeadEntity& value) : first(key), second(value) { } };
struct SDeadEntityIterator { void* m_pNode; SDeadEntityIterator() { } };
struct SDeadEntityInsertResult { SDeadEntityIterator first; bool second; };
struct CRetailDeadEntityMap { void* m_pHead; unsigned long m_size;
    SDeadEntityInsertResult insert(const SDeadEntityPair& v) { ++g_inserts; g_key = v.first; g_uid = v.second.UID; g_gp = v.second.GamePersistent; g_crc = v.second.SectionNameCRC; SDeadEntityInsertResult r; r.first.m_pNode = 0; r.second = true; return r; } };
struct CEntityRuntimePersistence { unsigned char _pad0[0x18]; CRetailDeadEntityMap PersistedDeadEntities; long SaveFormatVersion;
    void AddPersistedDeadEntity(u64 uid, bool gamePersistent, unsigned long sectionNameCRC) {
        if (uid != 0) { CPersistedDeadEntity entity; entity.UID = uid; entity.GamePersistent = gamePersistent; entity.SectionNameCRC = sectionNameCRC; PersistedDeadEntities.insert(SDeadEntityPair(uid, entity)); } } };
int main()
{
    CEntityRuntimePersistence p; std::memset(&p, 0, sizeof(p));
    p.AddPersistedDeadEntity(0, true, 5);
    bool ok = g_inserts == 0 && sizeof(CPersistedDeadEntity) == 16 && sizeof(SDeadEntityPair) == 24;
    u64 uid = ((u64)0x12345678 << 32) | 0x9abcdef0u;
    p.AddPersistedDeadEntity(uid, true, 0xdeadbeef);
    ok = ok && g_inserts == 1 && g_key == uid && g_uid == uid && g_gp && g_crc == 0xdeadbeef;
    if (!ok) { std::printf("00520330_TEST FAIL\n"); return 1; }
    std::printf("00520330_TEST PASS\n"); return 0;
}