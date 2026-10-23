struct BObj {
    unsigned char pad[0x6c];
    unsigned char flag6c;
    void consume(BObj* b);   // real member => thiscall, arg on stack
};
struct Node { int key; BObj* value; };
struct MapT {
    Node* first;
    Node* end;
    Node* find(const unsigned long& key);    // the map lower bound (0x0040F020), key by address
    __forceinline BObj* Find(long kind)   // the inlined map find: end when the key is absent
    {
        Node* it = find((unsigned long)kind);
        if (it == end || kind < it->key)
            it = end;
        return it->value;
    }
};
struct AObj {
    unsigned char pad0[0x24];
    unsigned int field24; // read as [esi+0x24]
    unsigned char pad2[0x1c];
    MapT map44;           // at +0x44
};
template <class T> __forceinline bool GetTCOut(AObj* thing, long kind, T** out)
{
    if (thing->field24 & 0x20000000) {
        *out = (T*)thing->map44.Find(kind);
        return true;
    }
    return false;
}
struct Host { void** vt; };

typedef AObj* (__fastcall *GetA_t)(void*);
typedef BObj* (__fastcall *GetB_t)(void*);

void __stdcall CGameScriptInterface_CreatureGeneratorRemoveTriggerer(Host* p0, Host* p1)
{
    AObj* a = ((GetA_t)(p0->vt[0x0b]))(p0);
    BObj* b = ((GetB_t)(p1->vt[0x0b]))(p1);

    BObj* val;
    if (GetTCOut(a, 0x3d, &val) && (b->flag6c & 8))
        val->consume(b);
}
