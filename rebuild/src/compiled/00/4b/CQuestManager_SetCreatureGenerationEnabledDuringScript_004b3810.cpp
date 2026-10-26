// CQuestManager::SetCreatureGenerationEnabledDuringScript(const CCharString&, bool) -- keep the script in the creature-generation-disabled list exactly when generation is disabled for it (add it when disabling, erase it when enabling)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B3810, length 76 bytes.
// Callees: the SGI __find for CCharString ranges @ 0x004B8E40 (`__fastcall`), vector<CCharString>::push_back
// @ 0x0044BFF0, vector<CCharString>::erase(iterator) @ 0x004B8770.
// Identity: FableWin.pdb ?SetCreatureGenerationEnabledDuringScript@CQuestManager@@QAEXABVCCharString@@_N@Z (the
// manifest's "SetCreatureGeneratorState" label is a guess).
// Layout (retail): CreatureGenerationDisabledQuests (vector<CCharString>) at +0x40 (PDB +0x5c).
// Shape: the argument pointer in ebx, `last` cached in esi, the vector's address in edi; the empty tag temporary
// is homed on the (live) `enabled` argument slot (`lea eax,[esp+14h]`), which is then re-read for both branches;
// the push_back branch returns through its own epilogue.
class CCharString { public:
    void* PStringData;
};

struct SRandomAccessIteratorTag {                  // empty tag: passed by const reference, homed in a dead slot
    SRandomAccessIteratorTag() { }
};

// SGI __find(first, last, value, tag) for CCharString ranges (`__fastcall`: first in ecx, last in edx)
CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag);   // 0x004B8E40
void* FindRef_free_FindString_004b8e40_2();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag)
{
    void* p = FindRef_free_FindString_004b8e40_2();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)&value = p;

    return 0;
}
#pragma auto_inline(on)

struct CCharStringVector {                         // retail vector<CCharString>: three words
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    void         push_back(const CCharString& value);   // 0x0044BFF0
    CCharString* erase(CCharString* where);             // 0x004B8770
};
void* FindRef_CCharStringVector_push_back_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CCharStringVector::push_back(const CCharString& value)
{
    void* p = FindRef_CCharStringVector_push_back_1();
    if (!p)
        return;
    *(void**)&value = p;
    
}
#pragma auto_inline(on)
void* FindRef_CCharStringVector_erase_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CCharString* CCharStringVector::erase(CCharString* where)
{
    void* p = FindRef_CCharStringVector_erase_0();
    if (!p)
        return 0;
    *(void**)where = p;
    return 0;
}
#pragma auto_inline(on)

class CQuestManager { public:
    unsigned char     _pad_0x00[0x40];
    CCharStringVector CreatureGenerationDisabledQuests;   // +0x40
    void SetCreatureGenerationEnabledDuringScript(const CCharString& script, bool enabled);
};

void CQuestManager::SetCreatureGenerationEnabledDuringScript(const CCharString& script, bool enabled)
{
    CCharString* last = CreatureGenerationDisabledQuests.m_pLast;
    CCharStringVector* quests = &CreatureGenerationDisabledQuests;
    CCharString* it = FindString_004b8e40(quests->m_pFirst, last, script, SRandomAccessIteratorTag());
    if (it == last) {
        if (!enabled)
            quests->push_back(script);
    } else if (enabled) {
        quests->erase(it);
    }
}