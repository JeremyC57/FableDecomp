#include <cstdio>
#include <cstdlib>
#include <cstring>
class CRegion; struct CList; static char g_seq[128] = ""; struct CANode { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual bool MakeRegion(const CRegion*, CList&) { return false; } virtual bool AddNav(const CRegion*, CList&) { return false; } virtual void v014() { } virtual void v015() { } virtual void v016() { } virtual void SetNeighbour(long, CANode*) { } virtual void Calc() { } };
struct CLeaf : CANode { char m_id; bool m_make; CLeaf(char id, bool make) : m_id(id), m_make(make) { } virtual bool MakeRegion(const CRegion*, CList&) { char s[2] = {m_id, 0}; std::strcat(g_seq, s); return m_make; } virtual bool AddNav(const CRegion*, CList&) { char s[2] = {m_id, 0}; std::strcat(g_seq, s); return m_make; }
  virtual void SetNeighbour(long side, CANode* n) { char s[4] = {m_id, (char)('0' + side), ((CLeaf*)n)->m_id, 0}; std::strcat(g_seq, s); } virtual void Calc() { char s[3] = {'c', m_id, 0}; std::strcat(g_seq, s); } };
struct CQT : CANode { unsigned char _pad[0xc]; CANode* ch[4];
  virtual bool MakeRegion(const CRegion* r, CList& l) { if (ch[0]->MakeRegion(r, l)) return true; if (ch[1]->MakeRegion(r, l)) return true; if (ch[2]->MakeRegion(r, l)) return true; return ch[3]->MakeRegion(r, l); }
  virtual bool AddNav(const CRegion* r, CList& l) { if (ch[0]->AddNav(r, l)) return true; if (ch[1]->AddNav(r, l)) return true; if (ch[2]->AddNav(r, l)) return true; return ch[3]->AddNav(r, l); }
  virtual void Calc() { ch[0]->Calc(); ch[1]->Calc(); ch[2]->Calc(); ch[3]->Calc(); ch[0]->SetNeighbour(2, ch[1]); ch[1]->SetNeighbour(0, ch[3]); ch[3]->SetNeighbour(3, ch[2]); ch[2]->SetNeighbour(1, ch[0]); } };

int main() {
    CLeaf a('a', false), b('b', false), c('c', true), d('d', false); CQT* q = new CQT; q->ch[0] = &a; q->ch[1] = &b; q->ch[2] = &c; q->ch[3] = &d; CList* l = 0; if (!q->AddNav(0, *l) || std::strcmp(g_seq, "abc")) { std::printf("00a76010_TEST FAIL\n"); return 1; } g_seq[0] = 0; c.m_make = false; if (q->AddNav(0, *l) || std::strcmp(g_seq, "abcd")) { std::printf("00a76010_TEST FAIL\n"); return 1; } g_seq[0] = 0; d.m_make = true; if (!q->AddNav(0, *l)) { std::printf("00a76010_TEST FAIL\n"); return 1; } delete q;
    std::printf("00a76010_TEST PASS\n"); return 0;
}