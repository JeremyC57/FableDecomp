#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00ae8a80: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// NParticleEngine::CParticleEmitterDatabase::GetEmitterTemplate_00ae8a80(long, long) -- with a particle data bank, fetch the handle's emitter info and return its template (loaded on demand, forced when asked) or null; without one, look the handle up in the emitter map and return its emitter or null
// Leaf: GetEmitterTemplate_00ae8a80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE8A80, length 208 bytes. Named apart: the manifest's
// "GetEmitterTemplate(long, bool)" is also the label of 0x00AE89E0 (the landed callers' target, which additionally
// rejects a null handle and destroys its local out of line); this twin has no direct callers or pointer refs.
// Callees: CParticleDataBank::GetEmitterInfo @ 0x00AF1280 (manifest label "CTCInventoryBase::GetCategoryWithIndex" is
//          a byte-twin), CParticleEmitterInfo::GetEmitterTemplate @ 0x00AF1480, CVectorMap<long,
//          CCountedPointer<CParticleEmitter>>::LowerBound @ 0x00AE9580, operator delete @ 0x00BFE9BC (count block),
//          the count block's destroy member-function pointer.
// Layout (PDB less the 16-byte CSymbolMap shrink): the emitter map (a sorted 12-byte vector of
// pair<long, CCountedPointer<CParticleEmitter>>, then its comparator) at +0x28, the bank counted pointer at +0x38.
// Shape: the template comes back in a counted-pointer local whose inline destructor (the count block's
// release) runs on both returns; find() is lower_bound plus the "key < found" test, compared against end().
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CCountedItem { };
struct SCountBlock {                               // the shared count block
    long m_count;                                  // +0x00
    void (CCountedItem::*m_pfnDestroy)();          // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                         // +0x08
};

namespace NParticleEngine { struct CParticleEmitter; }
typedef NParticleEngine::CParticleEmitter CEmitter;

struct CEmitterPointer {                           // CCountedPointer<CParticleEmitter> (names as the landed twin 0x00AE89E0)
    CEmitter*    Object;
    SCountBlock* Block;
    CEmitterPointer() : Object(0), Block(0) { }
    ~CEmitterPointer()                             // inline here (the twin calls it out of line @ 0x00AE94A0)
    {
        if (Block) {
            if (--Block->m_count == 0) {
                (Block->m_pItem->*Block->m_pfnDestroy)();
                operator delete(Block);
            }
        }
    }
};

struct CParticleEmitterInfo {
    bool GetEmitterTemplate(CEmitterPointer& out, long forceLoad);   // 0x00AF1480 (bool in the PDB; the twin's spelling)
};

namespace NParticleEngine {
struct CParticleDataBank {
    CParticleEmitterInfo* GetEmitterInfo(long handle);           // 0x00AF1280
};
}

struct CBankPointer {                              // CCountedPointer<CParticleDataBank>
    NParticleEngine::CParticleDataBank* Object;
    SCountBlock*                        Block;
    NParticleEngine::CParticleDataBank* operator->() const { return Object; }
};

struct SEmitterEntry {                             // pair<long, CCountedPointer<CParticleEmitter>>
    long            first;
    CEmitterPointer second;
};

struct CEmitterMap {                               // CVectorMap<long, CCountedPointer<CParticleEmitter>, ...>
    SEmitterEntry* m_first;                        // +0x00
    SEmitterEntry* m_last;                         // +0x04
    SEmitterEntry* m_end;                          // +0x08
    unsigned long  m_comp;                         // +0x0c
    SEmitterEntry* LowerBound(const long& key);    // 0x00AE9580 (the twin's map uses the sibling 0x00AE9520)
    SEmitterEntry* end() { return m_last; }
    SEmitterEntry* find(const long& key)
    {
        SEmitterEntry* it = LowerBound(key);
        SEmitterEntry* last = end();
        if (it == last || key < it->first)
            it = last;
        return it;
    }
};

struct CParticleEmitterDatabase {
    unsigned char SymbolMap[0x28];                 // +0x00 CSymbolMap + UsingSymbolMap
    CEmitterMap   Emitters;                        // +0x28
    CBankPointer  EmitterBank;                     // +0x38
    CEmitter* GetEmitterTemplate_00ae8a80(long handle, long forceLoad);
};

CEmitter* CParticleEmitterDatabase::GetEmitterTemplate_00ae8a80(long handle, long forceLoad)
{
    if (EmitterBank.Object) {
        CParticleEmitterInfo* info = EmitterBank->GetEmitterInfo(handle);
        CEmitterPointer emitter;
        if (info->GetEmitterTemplate(emitter, forceLoad))
            return emitter.Object;
        return 0;
    }
    SEmitterEntry* it = Emitters.find(handle);
    if (it == Emitters.end())
        return 0;
    return it->second.Object;
}

// ---- stubs and driver ----
static int g_destroyed = 0, g_deleted = 0;
static bool g_armed = false;
void __cdecl operator delete(void* p) { if (g_armed) ++g_deleted; }
struct CItem : CCountedItem { void Destroy() { ++g_destroyed; } };
static SCountBlock g_block;
static CEmitter* g_tpl = (CEmitter*)0x5678;
static long g_lastHandle = 0, g_lastForce = -1;
static bool g_have = true;
bool CParticleEmitterInfo::GetEmitterTemplate(CEmitterPointer& out, long forceLoad)
{
    g_lastForce = forceLoad;
    if (!g_have) return false;
    out.Object = g_tpl; out.Block = &g_block; ++g_block.m_count;
    return true;
}
static CParticleEmitterInfo g_info;
CParticleEmitterInfo* NParticleEngine::CParticleDataBank::GetEmitterInfo(long handle) { g_lastHandle = handle; return &g_info; }
static SEmitterEntry g_entries[3];
SEmitterEntry* CEmitterMap::LowerBound(const long& key)
{
    SEmitterEntry* p = m_first;
    while (p != m_last && p->first < key) ++p;
    return p;
}
int main()
{
    static CItem item;
    g_block.m_count = 0;
    g_block.m_pfnDestroy = static_cast<void (CCountedItem::*)()>(&CItem::Destroy);
    g_block.m_pItem = &item;
    static NParticleEngine::CParticleDataBank bank;
    CParticleEmitterDatabase db;
    std::memset(&db, 0, sizeof(db));
    db.EmitterBank.Object = &bank;
    g_armed = true;
    CEmitter* r1 = db.GetEmitterTemplate_00ae8a80(7, 1);   // count 0 -> 1 -> 0: destroyed + deleted
    bool ok = r1 == g_tpl && g_lastHandle == 7 && g_lastForce == 1 && g_destroyed == 1 && g_deleted == 1;
    g_have = false;
    CEmitter* r2 = db.GetEmitterTemplate_00ae8a80(8, 0);
    ok = ok && r2 == 0 && g_lastForce == 0 && g_destroyed == 1;
    g_armed = false;
    // the map path
    db.EmitterBank.Object = 0;
    g_entries[0].first = 2; g_entries[0].second.Object = (CEmitter*)0x10; g_entries[0].second.Block = 0;
    g_entries[1].first = 5; g_entries[1].second.Object = (CEmitter*)0x20; g_entries[1].second.Block = 0;
    db.Emitters.m_first = g_entries; db.Emitters.m_last = g_entries + 2;
    ok = ok && db.GetEmitterTemplate_00ae8a80(5, 0) == (CEmitter*)0x20 && db.GetEmitterTemplate_00ae8a80(2, 0) == (CEmitter*)0x10;
    ok = ok && db.GetEmitterTemplate_00ae8a80(3, 0) == 0 && db.GetEmitterTemplate_00ae8a80(9, 0) == 0;
    std::memset(g_entries, 0, sizeof(g_entries));
    if (!ok) { std::printf("00ae8a80_TEST FAIL\n"); return 1; }
    std::printf("00ae8a80_TEST PASS\n");
    return 0;
}