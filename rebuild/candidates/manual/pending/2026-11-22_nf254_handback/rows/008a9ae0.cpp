// CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long, CCharString> >& boasts) const -- append (boast id, text) for every "boasts made" message (type 0x29) seen or heard since the previous frame; true when there was at least one
// Leaf: ?MsgOnBoastsMade@CGameScriptInterface@@UBE_NAAV?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9AE0, length 344 bytes (`ret 4`, bool in al, 8-aligned ebp frame).
// Callees: the frame getters 0x006E7510 / 0x006E7530, CMessageEventManager::FindAllMessages<CFilter_Bind2<IsOfType,
//   CanBeSeenOrHeard>> @ 0x006178F0 (fills a std::list of message pointers) through CWorld+0x60, malloc @ 0x00BFEA0E /
//   free @ 0x00BFEA14 (the list sentinel and nodes), CCharString copy ctor/dtor @ 0x0099EC30 / 0x0099EAE0,
//   vector<pair<long, CCharString> >::_M_insert_overflow @ 0x0074FCE0.
// Layout: the message payload (+0x3C) points at {long id; ...; CCharString text at +0xC}.
// Shape: the Msg-family filter (the landed MsgOnRegionPreunload 0x008934F0 model, without the frame - 1); push_back
//   inlined (null-tested placement copy of the pair, finish += 8, else the overflow); the result is `list.size() != 0`
//   with the list destructor inlined into both returns.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
// NF254 (2026-11-22, CODEXP27 L4 / L5 family): the boast vector, its element and CCharString were the row-local CBoastVector / CBoastPair /
//   CCharString stand-ins (0x0074FCE0 bound as ?_M_insert_overflow@CBoastVector@@..., the row's own leaf QBE over CBoastVector&); they now
//   come from the shared headers (fable_string.h CCharString, stlport/fable_containers.h vector<pair<long, CCharString> > with the overflow
//   declaration only: FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY), so the call binds the protected STLport spelling the self row 0x0074FCE0
//   defines, and the row's own leaf is the PDB one: Ego_r.pdb and FableWin.pdb both spell
//   ?MsgOnBoastsMade@CGameScriptInterface@@UBE_NAAV?$vector@U?$pair@JVCCharString@@@std@@...@std@@@Z (public virtual const: retail vtable
//   0x01260F0C slot 34 holds 0x008A9AE0). The `virtual` is an in-place respell of the row's local CGameScriptInterface model (nf248 ruling 1 /
//   nf250 ruling 4); the other local models are as at HEAD.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_string.h"
struct CBoastPayload {
    long        m_id;                                   // +0x00
    long        _pad04[2];
    CCharString m_text;                                 // +0x0C
};
struct CMessageEvent_Retail {
    unsigned char  _pad[0x3c];
    CBoastPayload* m_extraData;                         // +0x3c
};
struct SMessageNode {
    SMessageNode*         _M_next;
    SMessageNode*         _M_prev;
    CMessageEvent_Retail* _M_data;
};
struct CMessageList {                                   // STLport list<CMessageEvent*>
    SMessageNode* _M_node;
    CMessageList()
    {
        SMessageNode* node = (SMessageNode*)malloc(sizeof(SMessageNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CMessageList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SMessageNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SMessageNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SMessageNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager_Retail {
    void FindAllMessages(const CSeenOrHeardFilter& filter, CMessageList& out);   // 0x006178F0
};
void AppendMatchingMessages(CMessageEventManager_Retail* manager, long type, long frame, long maxFrame, SMessageNode* sentinel);
// Same-TU stand-in (the template instance is emitted in this TU too; the linker kept 0x006178F0): it reads the bound
// filters and only links nodes behind the sentinel, so VC7.1 keeps the sentinel pointer in esi across the call.
#pragma auto_inline(off)
void CMessageEventManager_Retail::FindAllMessages(const CSeenOrHeardFilter& filter, CMessageList& out)
{
    AppendMatchingMessages(this, filter.m_pFirst->m_type, filter.m_pSecond->m_frame, filter.m_pSecond->m_maxFrame, out._M_node);
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char                _pad00[0x60];
    CMessageEventManager_Retail* PMessageEventManager_Data;   // +0x60 (engine/CWorld.h)
};
class CGameScriptInterface { public:
    virtual void v000();
    CWorld* World;                                  // +0x04 (engine/CGameScriptInterface.h)
protected:
    long GetMinWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
public:
    virtual bool MsgOnBoastsMade(std::vector<std::pair<long, CCharString> >& boasts) const;   // vtable 0x01260F0C slot 34
};

bool CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long, CCharString> >& boasts) const
{
    long frame = GetMinWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    CMessageList found;
    messages->FindAllMessages(CMessageEventFilter_IsOfType(0x29) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame), found);
    for (SMessageNode* it = found._M_node->_M_next; it != found._M_node; it = it->_M_next) {
        CBoastPayload* boast = it->_M_data->m_extraData;
        boasts.push_back(std::pair<long, CCharString>(boast->m_id, boast->m_text));
    }
    if (found.size() != 0)
        return true;
    return false;
}