// CThingPlayerCreature::PreOnKill() -- for the main player: use a resurrection item from the inventory when one is there, else open the quest-completion UI in "death" mode; either way the kill is vetoed
// Leaf: PreOnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB300, length 302 bytes (+ int3 padding).
// Callees: CThing::GetPlayer() const @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (interfaces 0x11 = CTCInventoryBase, bit 17 of +0x20; 0xe1 = CTCResurrectionItem and
// 0xfc = CTCQuestCompletionUI, bits 1 / 28 of +0x3c), CTCInventoryBase::GetResurrectionItemInInventory @ 0x005BF77D,
// CTCResurrectionItem::OnItemUsed(CThingCreatureBase&) @ 0x007F04C0 (landed), CWideString(const wchar_t*) @ 0x0099B6B0 /
// ~CWideString @ 0x0099B510, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CTCQuestCompletionUI::Activate(CompletionType, const CCharString&, bool, const CWideString&, bool) @ 0x005E4740.
// Strings: 0x0122D70C L"", 0x0122D70E "".
// Layout: CThing interface bitset +0x20, map +0x44, Dead bit +0x91 (retail, local model).
// Shape: the inventory lookup is the plain entry-pick GetTC used straight as `this`; the resurrection item and the UI are
// `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the zero preload); both Activate strings are implicit conversion
// temporaries (wide first, by right-to-left evaluation); a same-TU LowerBound stand-in keeps every key in one frame word;
// the main-player test wraps the body so the `return true` exit is laid out last.
typedef unsigned short wchar_t;                                // VC7.1 without /Zc:wchar_t
class CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const wchar_t* text);                          // 0x0099B6B0
    ~CWideString();                                            // 0x0099B510
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CPlayer {
public:
    bool IsMainPlayer() const;                                 // 0x00487520
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                    // +0x91 bit 0
    CPlayer* GetPlayer() const;                                // 0x004C7A30
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing { };
class CThingPlayerCreature : public CThingCreatureBase {
public:
    bool PreOnKill();
};
class CTCInventoryBase {
public:
    CThing* GetResurrectionItemInInventory();                  // 0x005BF77D
};
class CTCResurrectionItem {
public:
    void OnItemUsed(CThingCreatureBase& creature);             // 0x007F04C0
};
enum CompletionType { COMPLETION_TYPE_3 = 3 };
class CTCQuestCompletionUI {
public:
    void Activate(CompletionType type, const CCharString& questName, bool a, const CWideString& text, bool b);   // 0x005E4740
};

extern const wchar_t g_emptyWideString_0122d70c[];             // 0x0122D70C (L"")
extern const char g_emptyString_0122d70e[];                    // 0x0122D70E ("")

bool CThingPlayerCreature::PreOnKill()
{
    if (GetPlayer()->IsMainPlayer()) {
        if (HasTC(0x11)) {
            CThing* item = ((CTCInventoryBase*)GetTC(0x11))->GetResurrectionItemInInventory();
            if (item && !item->Dead) {
                CTCResurrectionItem* resurrection = 0;
                if (item->HasTC(0xe1))
                    resurrection = (CTCResurrectionItem*)item->GetTC(0xe1);
                resurrection->OnItemUsed(*this);
                return false;
            }
        }
        CTCQuestCompletionUI* ui = 0;
        if (HasTC(0xfc))
            ui = (CTCQuestCompletionUI*)GetTC(0xfc);
        ui->Activate(COMPLETION_TYPE_3, g_emptyString_0122d70e, true, g_emptyWideString_0122d70c, true);
        return false;
    }
    return true;
}
