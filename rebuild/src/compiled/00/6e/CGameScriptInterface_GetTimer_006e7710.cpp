// Leaf: ?GetTimer@CGameScriptInterface@@UBEJJ@Z

struct Node { char pad[0x14]; long value; /* +0x14 */ };

struct Iter { Node* p; };

struct TimerMap {
    Node* begin;                 // +0x0  m_pBegin
    Node* end;                   // +0x4
    Iter LowerBound(const long* key);
};

class CGameScriptInterface { public:
    char pad[0x38];
    TimerMap map;                // +0x3c
    virtual long GetTimer(long key) const;
};

long CGameScriptInterface::GetTimer(long key) const
{
    const TimerMap* m = &this->map;
    Iter it = ((TimerMap*)m)->LowerBound(&key);
    if (it.p != m->begin)
        return it.p->value;
    return 0;
}