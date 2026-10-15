#include <cstdio>
#include <stdlib.h>
// Behaviour test for CSoundManagerLH::vfunc_5 (0x00A3EB60).
static int g_deleted = 0, g_stopped = 0;
struct CSoundObjectLH {
    virtual ~CSoundObjectLH() { ++g_deleted; }
    virtual void Stop() { ++g_stopped; }
};
struct SSoundNode { SSoundNode* Next; SSoundNode* Prev; CSoundObjectLH* Value; };
struct CSTLportSoundList {
    SSoundNode* Node;
    SSoundNode* Begin() { return Node->Next; }
    SSoundNode* End() { return Node; }
    SSoundNode* Erase(SSoundNode* pos)
    {
        SSoundNode* next = pos->Next;
        SSoundNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
        return next;
    }
};
struct CSoundManagerLH {
    CSTLportSoundList m_entries;
    void vfunc_5(CSoundObjectLH* sound);
};
void CSoundManagerLH::vfunc_5(CSoundObjectLH* sound)
{
    sound->Stop();
    for (SSoundNode* it = m_entries.Begin(); it != m_entries.End(); ) {
        if (it->Value == sound) {
            delete sound;
            it = m_entries.Erase(it);
        } else {
            it = it->Next;
        }
    }
}
int main()
{
    SSoundNode* s = (SSoundNode*)malloc(sizeof(SSoundNode)); s->Next = s->Prev = s;
    CSoundObjectLH keep;
    CSoundObjectLH* victim = new CSoundObjectLH;
    CSoundObjectLH* values[3] = { &keep, victim, &keep };
    for (int i = 0; i < 3; ++i) {
        SSoundNode* n = (SSoundNode*)malloc(sizeof(SSoundNode)); n->Value = values[i];
        n->Prev = s->Prev; n->Next = s; s->Prev->Next = n; s->Prev = n;
    }
    CSoundManagerLH m; m.m_entries.Node = s;
    m.vfunc_5(victim);
    int count = 0; for (SSoundNode* n = s->Next; n != s; n = n->Next) { ++count; if (n->Value != &keep) count += 100; }
    if (g_stopped != 1 || g_deleted != 1 || count != 2) { std::printf("00a3eb60_TEST FAIL\n"); return 1; }
    std::printf("00a3eb60_TEST PASS\n");
    return 0;
}
