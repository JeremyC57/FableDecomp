// CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing(set<CCharString>& entries) const -- clear the entries, then list the text entries of the debug narrator when one is set, else of the hero's current target's text bank (retrying with the "_1_5" suffix when the plain bank name yields nothing)
// Leaf: DebugGetAllTextEntriesForTargetedThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A0CB0, 454 bytes, `ret 4`.
// Callees: the tree erase 0x004CF810 (STLport _Rb_tree::_M_erase), CBasicString<char>::operator==(const char*) 0x004115A0,
//   the text data bank's GetAllTextEntriesForNarrator 0x009C8990, the player-manager / player accessors 0x00449960 /
//   0x00487DD0, the const component-map LowerBound 0x004365B0 (TC 8 targeting, TC 15 text), the target accessor (slot 33),
//   CBankFile::GetBankHandle 0x004FAEE0 (by value), CCharString compare 0x0099E960, operator+(const CCharString&, const char*)
//   0x0099F600 (fastcall), ~CCharString 0x0099EAE0. Globals: the main game component 0x013B86A0, the debug narrator name
//   0x013B86F8, the text language 0x013B86C0 (descriptive names).
// Shape: the target's text bank is a named local taken before the first narrator call (retail keeps it in edi, pushed at
//   entry, and reuses it for the "_1_5" retry); each call re-fetches the bank handle by value.
#include <string.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                      // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const;             // 0x0099E960
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct CStringSet {                                      // STLport set<CCharString> (retail 12 bytes)
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    long                _M_compare;
    void _M_erase(_Rb_tree_node_base* x);                // 0x004CF810
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
struct CTextDataBank {
    void GetAllTextEntriesForNarrator(const CCharString& narrator, CStringSet& entries, long language);   // 0x009C8990
};
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
class CThing { public:
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  State[0x20];
    unsigned long  ThingTypeFlags;                       // +0x6c
    unsigned char  Metadata[0x21];
    unsigned char  LifeFlags;                            // +0x91
};
class CThingPlayerCreature : public CThing { };
struct CTCTargeting {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032();
    virtual CThing* GetTarget() const;   // slot 33 (+0x84)
};
struct CTCText { CCharString GetBankHandle() const; };  // 0x004FAEE0
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };      // 0x00487DD0
class CPlayerManager { public: public:
    const CPlayer& GetMainPlayer() const;   // 0x00449960
};   // 0x00449960
class CMainGameComponent { public:
    unsigned char   Data[0x14];
    CTextDataBank*  TextBank;                            // +0x14
    unsigned char   Data2[4];
    CPlayerManager* PlayerManager;                       // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
extern CCharString g_DebugNarrator;                      // 0x013B86F8
extern long g_TextLanguage;                              // 0x013B86C0
template <class T, long TYPE> __forceinline T* GetComponent(const CThing* thing)
{
    const ComponentMap* map = &thing->Components;
    const ComponentEntry* it = map->LowerBound(TYPE);
    const ComponentEntry* end = map->Finish;
    return (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
}
class CGameScriptInterface { public:
    virtual void v000();
    void DebugGetAllTextEntriesForTargetedThing(CStringSet& entries) const;
};

void CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing(CStringSet& entries) const
{
    entries.clear();
    const CThing* hero;
    CThing* target;
    if (g_DebugNarrator == "" && (hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().PeekPControlledCreature()) != 0
        && !(hero->LifeFlags & 1) && (hero->UsingComponents[0] & 0x100)
        && (target = GetComponent<CTCTargeting, 8>(hero)->GetTarget()) != 0 && (target->ThingTypeFlags & 0x10)) {
        if (target->UsingComponents[0] & 0x8000) {
            CTCText* text = GetComponent<CTCText, 0xf>(target);
            if (text->GetBankHandle() != "") {
                CTextDataBank* bank = g_pMainGameComponent->TextBank;
                bank->GetAllTextEntriesForNarrator(text->GetBankHandle(), entries, g_TextLanguage);
                if (entries._M_node_count == 0)
                    bank->GetAllTextEntriesForNarrator(text->GetBankHandle() + "_1_5", entries, g_TextLanguage);
            }
        }
    } else {
        g_pMainGameComponent->TextBank->GetAllTextEntriesForNarrator(g_DebugNarrator, entries, g_TextLanguage);
    }
}