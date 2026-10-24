// Behaviour test for 0x00AECB80: the connection record destroys its two strings (last first), its owner pointer, its node vector (elements, then storage), then the keyed table at +0.
#include <cstdio>
#include <cstdlib>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void DeleteData(void* data) { (void)data; g_log[g_n++] = 50; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
struct SNonscalarPtrIteratorTag { };
struct CRegionConnectionNode;
struct SNodeEntry { long first; CCountedPointer<CRegionConnectionNode> second; };
static void DestroyRange(SNodeEntry* first, SNodeEntry* last, const SNonscalarPtrIteratorTag&)
{
    g_log[g_n++] = 60;
    for (; first != last; ++first) first->~SNodeEntry();
}
static void Free(void* block) { g_log[g_n++] = 61; std::free(block); }
struct CNodeEntryVector {
    SNodeEntry* _M_start; SNodeEntry* _M_finish; SNodeEntry* _M_end_of_storage;
    ~CNodeEntryVector() { DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag()); if (_M_start) Free(_M_start); }
};
struct CStringKeyedTable { unsigned char _body[0x20]; ~CStringKeyedTable() { g_log[g_n++] = 70; } };
struct CConnectionOwner;
struct CRegionConnectionRecord {
    CStringKeyedTable m_table; long m_value20; long m_value24; CNodeEntryVector m_nodes; long m_value34;
    CCountedPointer<CConnectionOwner> m_owner; long m_value40; CCharString m_name; CCharString m_scriptName;
    ~CRegionConnectionRecord() { }
};
int main()
{
    {
        CRegionConnectionRecord r;
        r.m_name.id = 1; r.m_scriptName.id = 2;
        r.m_nodes._M_start = (SNodeEntry*)std::malloc(sizeof(SNodeEntry));
        r.m_nodes._M_start->first = 4; r.m_nodes._M_start->second.Data = 0; r.m_nodes._M_start->second.Info = NewInfo(2);
        CCPPointerInfo* keep = r.m_nodes._M_start->second.Info;
        r.m_nodes._M_finish = r.m_nodes._M_start + 1; r.m_nodes._M_end_of_storage = r.m_nodes._M_finish;
        r.m_owner.Data = 0; r.m_owner.Info = NewInfo(1);
        (void)keep;
    }
    // script name (2), name (1), owner deleted (50), range (60; shared element survives), storage (61), table (70)
    bool ok = g_n == 6 && g_log[0] == 2 && g_log[1] == 1 && g_log[2] == 50 && g_log[3] == 60 && g_log[4] == 61 && g_log[5] == 70;
    if (!ok) { std::printf("00aecb80_TEST FAIL\n"); return 1; }
    std::printf("00aecb80_TEST PASS\n");
    return 0;
}