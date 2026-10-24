// CQuestThingEntry_00768bc0::CQuestThingEntry_00768bc0() -- default-construct a quest/thing record: two CCharStrings (name, tag) and an empty intelligent thing pointer at +0x10
// Leaf: ??0CQuestThingEntry_00768bc0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768BC0, length 38 bytes (+ int3 padding), returns this; no call or address
// references (an unreferenced COMDAT). Its destructor is the next function, 0x00768BF0 (~CBaseIntelligentPointer +0x10,
// ~CCharString +4, ~CCharString +0 tail); the one before, 0x00768B80, is QuestData's constructor (Ego_r.pdb QuestData =
// {CCharString name, CCharString tag}). No Ego_r.pdb class has this exact layout, so the name is descriptive; the manifest
// label (_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>>) is propagated and wrong.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10;
// vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv; landed sources call it CIntelligentPointer<CThing>).
// Shape: members in declaration order; the inline CIntelligentPointer<T> ctor runs the out-of-line base ctor on a named
// `&m_thing` (edi) and then stores its own vtable; +8/+0xc are left unset.
class CCharString {
public:
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
};

struct CBaseIntelligentPointer {
    virtual void v000();
    void* PItem;                                             // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer() {}
    virtual void v000();
};

class CThing;

struct CQuestThingEntry_00768bc0 {                           // 0x18 bytes
    CCharString                 m_name;                      // +0x00 (QuestData::name)
    CCharString                 m_tag;                       // +0x04 (QuestData::tag)
    long                        m_value08;                   // +0x08
    long                        m_value0c;                   // +0x0c
    CIntelligentPointer<CThing> m_thing;                     // +0x10
    CQuestThingEntry_00768bc0();
};

CQuestThingEntry_00768bc0::CQuestThingEntry_00768bc0()
{
}