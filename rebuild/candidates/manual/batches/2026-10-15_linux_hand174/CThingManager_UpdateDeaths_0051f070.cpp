// CThingManager::UpdateDeaths() -- unlinks and deletes every thing queued on DeadThingList, then empties the list
// Leaf: UpdateDeaths
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051F070, length 101 bytes (+ int3 padding).
// Callees: CThingManager::RemoveFromUpdateList @ 0x0051ECC0, the thing's virtual scalar deleting dtor (slot 0, `push 1`),
// memmove @ 0x00BFEAE6 (the STLport clear's __copy_trivial path).
// Layout (retail, local model): CThingManager all-things list (STLport list<CThing*>, node data at +8) at +0x18, PlayerManager at
// retail +0x30 (PDB +0x3c), DeadThingList (12-byte STLport vector<CThing*>) at retail +0x48 (PDB +0x58); CThing SectionName
// (CCharString) at retail +0x58, the dead flag bit 0 of +0x91 (as IS_THING_ALIVE tests it).
// Shape: `for (unsigned i = 0; i < size(); ++i)` with operator[] as `*(begin() + i)` (size re-read every pass, `jb`);
// `delete thing` (null test + virtual deleting dtor); the inline STLport clear as in the landed CTCLight::OnAppearanceClear 0x00641210.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct CCharStringData {
    const char*   m_pChars;                     // +0x00
    unsigned long m_length;                     // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
};
int __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pStringData;
    CCharStringData* b = sb.m_pStringData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}
class CThing {
public:
    virtual ~CThing();                          // slot 0 (scalar deleting dtor)
    unsigned char _pad04[0x58 - 0x04];
    CCharString   SectionName;                  // +0x58 (retail)
    unsigned char _pad5c[0x91 - 0x5c];
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91_1 : 7;
    void Kill(bool immediately);                // 0x004C9B80
};
class CThingPlayerCreature;
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0 (CIntelligentPointer at +0x2c)
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;             // 0x00449970
};
struct SThingListNode {                         // STLport list<CThing*> node
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;                    // +0x08
};
struct CThingVector {                           // STLport vector<CThing*> (12 bytes in retail)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThing** begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    CThing*& operator[](unsigned int n) { return *(begin() + n); }
    void clear()
    {
        CThing** i = (CThing**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};
struct CThingManager {
    unsigned char   _pad00[0x18];
    SThingListNode* Things;                     // +0x18 STLport list<CThing*> sentinel (all things)
    unsigned char   _pad1c[0x30 - 0x1c];
    CPlayerManager* PlayerManager;              // +0x30 (PDB +0x3c)
    unsigned char   _pad34[0x48 - 0x34];
    CThingVector    DeadThingList;              // +0x48 (PDB +0x58)
    void RemoveFromUpdateList(CThing* thing);   // 0x0051ECC0
    void UpdateDeaths();                        // 0x0051F070
    void DeleteAllThings();
    void DeleteThingsWithSectionName(const CCharString& sectionName);
};

void CThingManager::UpdateDeaths()
{
    for (unsigned int i = 0; i < DeadThingList.size(); ++i) {
        CThing* thing = DeadThingList[i];
        RemoveFromUpdateList(thing);
        delete thing;
    }
    DeadThingList.clear();
}
