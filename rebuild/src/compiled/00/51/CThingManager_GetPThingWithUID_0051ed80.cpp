// CThingManager::GetPThingWithUID(unsigned __int64 uid) @ 0x0051ed80 -- the thing mapped to `uid`, or 0
// Leaf: GetPThingWithUID
class CThing;

struct Node {
    Node* _00;
    char pad[0x14];
    CThing* _18;   // value at +0x18
};

struct Sub {
    Node* head;                 // [esi]
    Node* Find(void* key);      // __fastcall member @ 0x527d80 (this in ecx, key on stack)
};

class CThingManager { public:
    char pad[0x58];
    Sub sub;                    // at +0x58
    CThing* GetPThingWithUID(unsigned __int64 uid);
};

CThing* CThingManager::GetPThingWithUID(unsigned __int64 uid)
{
    Sub* s = &this->sub;
    Node* r = s->Find(&uid);
    if (r != s->head)
        return r->_18;
    return 0;
}