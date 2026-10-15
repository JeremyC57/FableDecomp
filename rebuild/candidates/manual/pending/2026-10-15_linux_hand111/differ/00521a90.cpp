// CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name) -- NPC name map: creates the entry with an empty script name when the uid is new, then stores the display name
// Grade: DIFFER(ours v retail) -- 78v78: register choice only -- retail `lea eax` for the first operator[] key and `mov ecx,[name] ; push ecx ; lea edx ; mov ecx,esi ; push edx` for the second; the lane uses `lea ecx` then `mov edx ; push edx ; lea eax ; push eax ; mov ecx,esi`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00521A90, length 77 bytes.
// Callees: map::find(const unsigned __int64&) @ 0x00527E00 (iterator returned in eax), map::operator[](const unsigned __int64&)
// @ 0x0052B8B0 (x2, returns pair<CCharString, CWideString>&), CCharString::operator=(const char*) @ 0x0099EFE0,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0.
// Layout: NPCNameMap (map<unsigned __int64, pair<CCharString, CWideString>>) at +0xcc in retail
// (PDB +0xf4); the tree's head node pointer is its first word, so end() is `[esi]`.
// Shape: the find result is a named local (an inline `find(uid) == end()` loads the head
// before the call); the map's address lives in esi; the uid is passed by address from its own argument slot;
// each assignment pushes its right-hand side BEFORE operator[] is evaluated (VC evaluates the
// arguments before the object expression), the CWideString half at +4 of the pair.
struct CCharString {
    void* m_pData;
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct SNPCName {
    CCharString first;                                  // +0x00
    CWideString second;                                 // +0x04
};
struct SNPCNameNode;
struct CNPCNameMap {
    SNPCNameNode* m_pHead;                              // +0x00
    unsigned char _body[0x0c];
    SNPCNameNode* end() const { return m_pHead; }
    SNPCNameNode* find(const unsigned __int64& key);    // 0x00527E00
    SNPCName& operator[](const unsigned __int64& key);  // 0x0052B8B0
};
struct CThingManager {
    unsigned char _pad00[0xcc];
    CNPCNameMap   NPCNameMap;                           // +0xcc
    void SetNameForThing(unsigned __int64 uid, const CWideString& name);
};

void CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name)
{
    SNPCNameNode* it = NPCNameMap.find(uid);
    if (it == NPCNameMap.end())
        NPCNameMap[uid].first = "";
    NPCNameMap[uid].second = name;
}
