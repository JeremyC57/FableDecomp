// CThingManager::DeleteAllThings() -- kills the main player's hero (if alive) and every other living thing, processes the deaths and empties the dead list
// Leaf: DeleteAllThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051FBA0, length 125 bytes (+ int3 padding).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (manifest "Catch@0040d959" is a propagated label), CPlayer::GetHeroThing @ 0x00487DC0
// (manifest "_Cons_val<...>" propagated; `add ecx,2Ch ; jmp GetPItem`), CThing::Kill(bool) @ 0x004C9B80 (false),
// CThingManager::UpdateDeaths @ 0x0051F070, memmove @ 0x00BFEAE6 (the inline STLport clear).
// Layout (retail, local model): CThingManager all-things list (STLport list<CThing*>, node data at +8) at +0x18, PlayerManager at
// retail +0x30 (PDB +0x3c), DeadThingList (12-byte STLport vector<CThing*>) at retail +0x48 (PDB +0x58); CThing SectionName
// (CCharString) at retail +0x58, the dead flag bit 0 of +0x91 (as IS_THING_ALIVE tests it).
// Shape: the hero is a named local tested for null and the dead bit; the list walk keeps the node in edi and re-reads the
// sentinel each pass; the walk's alive test is an inline `m_dead ? false : true` (`not al ; test al,1`), the hero's a plain `!m_dead`.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
class CCharStringData { public:
    const char*   m_pChars;                     // +0x00
    unsigned long m_length;                     // +0x04
};
class CCharString { public:
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
    bool IsAlive() const { return m_dead ? false : true; }   // IS_THING_ALIVE: `not al ; test al,1`
};
class CThingPlayerCreature;
class CPlayer { public:
    CThing* GetHeroThing() const;               // 0x00487DC0 (CIntelligentPointer at +0x2c)
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
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
class CThingManager { public:
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

void CThingManager::DeleteAllThings()
{
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero && !hero->m_dead)
        hero->Kill(false);
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        if (node->_M_data->IsAlive())
            node->_M_data->Kill(false);
    }
    UpdateDeaths();
    DeadThingList.clear();
}