#include <cstdio>
class CThing { public: int id; };
class CTCMapwho;
class CTCBase { public:
    CThing* m_pThing; long m_baseWord;
    CTCBase(CThing* owner) : m_pThing(owner), m_baseWord(0) { }
    virtual ~CTCBase() { }
};
struct CMapwhoNode {
    CTCMapwho* PMapwho; CMapwhoNode* PNext; CMapwhoNode* PPrevious; void* PCell;
    CMapwhoNode() : PMapwho(0), PNext(0), PPrevious(0), PCell(0) { }
};
class CTCMapwho : public CTCBase { public:
    CMapwhoNode TLNode, TRNode, BLNode, BRNode; unsigned long SearchID; unsigned char InMapwho : 1; unsigned char Other : 7;
    CTCMapwho(CThing* owner) : CTCBase(owner), SearchID(0)
    {
        InMapwho = 0; TLNode.PMapwho = this; BRNode.PMapwho = this; TRNode.PMapwho = this; BLNode.PMapwho = this;
    }
};
int main()
{
    CThing t; t.id = 3;
    CTCMapwho* m = new CTCMapwho(&t);
    if (m->m_pThing != &t || m->SearchID != 0 || m->InMapwho != 0) { std::printf("0071a670_TEST FAIL\n"); return 1; }
    CMapwhoNode* nodes[4] = { &m->TLNode, &m->TRNode, &m->BLNode, &m->BRNode };
    for (int i = 0; i < 4; ++i)
        if (nodes[i]->PMapwho != m || nodes[i]->PNext != 0 || nodes[i]->PPrevious != 0 || nodes[i]->PCell != 0) { std::printf("0071a670_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("0071a670_TEST PASS\n");
    return 0;
}