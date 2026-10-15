// CManager::RemoveThingActionParam(long thingUID, const CCharString& name, const CCharString& value) -- (despite the propagated name) record a thing-action parameter once: when no entry of the parameter list at +0x58 carries this thing UID, append {name, value, false, thingUID} and flag the list as changed (+0x8f)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B14F0, length 152 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2), CCharString::operator=(const CCharString&) @ 0x0099EFB0 (x2), the STLport
//   list insert(iterator, const T&) @ 0x004B99E0 (iterator returned through a hidden pointer homed in the dead thingUID slot),
//   CCharString::~CCharString @ 0x0099EAE0 (x2).
// Identity: the label is propagated and does not fit the body (it only ever appends); kept as the catalog name, see NOTES.
// Layout: list (STLport, header node pointer) +0x58 with 16-byte entries {name, value, flag byte, thing UID} (node data at +8, the UID at
//   node +0x14); the changed flag byte +0x8f.
// Shape: the search is an inline Find that RETURNS the stop node (`while (it != end && it->uid != uid)`), so the caller's `== end`
//   compare is emitted after both loop exits (`cmp eax,ecx`, the end node kept in ecx); push_back = insert(end(), x) with the iterator
//   a 4-byte class with a user copy constructor built in its argument slot (`push ecx ; mov eax,esp ; mov [eax],ecx`).
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SThingActionParam {
    CCharString   m_name;
    CCharString   m_value;
    bool          m_flag;
    long          m_thingUID;
};
struct SParamNode {
    SParamNode*       _M_next;
    SParamNode*       _M_prev;
    SThingActionParam _M_data;
};
struct SParamIterator {
    SParamNode* _M_node;
    SParamIterator(SParamNode* n) : _M_node(n) { }
    SParamIterator(const SParamIterator& o) : _M_node(o._M_node) { }
};
struct CParamList {
    SParamNode* _M_node;
    SParamIterator insert(SParamIterator pos, const SThingActionParam& x);   // 0x004B99E0
    void push_back(const SThingActionParam& x) { insert(SParamIterator(_M_node), x); }
    __forceinline SParamNode* Find(long thingUID)
    {
        SParamNode* end = _M_node;
        SParamNode* it = end->_M_next;
        while (it != end && it->_M_data.m_thingUID != thingUID)
            it = it->_M_next;
        return it;
    }
};
struct CManager {
    unsigned char _pad00[0x58];
    CParamList    m_params;                        // +0x58
    unsigned char _pad5c[0x8f - 0x5c];
    bool          m_paramsChanged;                 // +0x8f
    void RemoveThingActionParam(long thingUID, const CCharString& name, const CCharString& value);
};
void CManager::RemoveThingActionParam(long thingUID, const CCharString& name, const CCharString& value)
{
    if (m_params.Find(thingUID) == m_params._M_node) {
        SThingActionParam param;
        param.m_name = name;
        param.m_value = value;
        param.m_flag = false;
        param.m_thingUID = thingUID;
        m_params.push_back(param);
        m_paramsChanged = true;
    }
}