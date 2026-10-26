// CWorld::GetThingWithUID @ 0x0051ed80  (__fastcall, returns DWORD)
typedef unsigned long DWORD;

struct Node {
    Node* _00;
    char pad[0x14];
    DWORD _18;     // value at +0x18
};

struct Sub {
    Node* head;                 // [esi]
    Node* Find(void* key);      // __fastcall member @ 0x527d80 (this in ecx, key on stack)
};

class CWorld { public:
    char pad[0x58];
    Sub sub;                    // at +0x58
    DWORD GetThingWithUID(int a1, int a2);
};

DWORD CWorld::GetThingWithUID(int a1, int a2)
{
    Sub* s = &this->sub;
    Node* r = s->Find(&a1);
    if (r != s->head)
        return r->_18;
    return 0;
}