// Behaviour test for CEAXManager::SetFXSlotProperty (0x00C51EB0): the slot setter gets the four arguments and an empty change
// list to fill; the commit then runs with whatever list it returned and the voice; the setter's result is returned.
#include <stdio.h>
struct _CHANGES { int v; };
struct CEAXVoice { int v; };
static _CHANGES g_list;
struct CEAXFXSlots {
    unsigned long slot, id, size; void* data; bool listWasEmpty; long result; bool fill;
    long SetSlotProperty(unsigned long s, unsigned long i, void* d, unsigned long n, _CHANGES** changes)
    { slot = s; id = i; data = d; size = n; listWasEmpty = (*changes == 0); if (fill) *changes = &g_list; return result; }
};
struct CEAXManager {
    CEAXFXSlots* m_pFXSlots; _CHANGES* committed; CEAXVoice* committedVoice; int commits;
    void QueueCommit(_CHANGES* changes, CEAXVoice* voice) { committed = changes; committedVoice = voice; ++commits; }
    long SetFXSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, CEAXVoice* voice);
};
long CEAXManager::SetFXSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, CEAXVoice* voice)
{
    _CHANGES* changes = 0;
    long result = m_pFXSlots->SetSlotProperty(slot, id, data, size, &changes);
    QueueCommit(changes, voice);
    return result;
}
int main(void)
{
    CEAXFXSlots slots; CEAXManager m; CEAXVoice voice; char data[4];
    m.m_pFXSlots = &slots; m.commits = 0;
    slots.result = 0; slots.fill = true;
    long a = m.SetFXSlotProperty(2, 7, data, 4, &voice);
    bool ok = a == 0 && slots.slot == 2 && slots.id == 7 && slots.data == data && slots.size == 4 && slots.listWasEmpty &&
              m.commits == 1 && m.committed == &g_list && m.committedVoice == &voice;
    slots.result = -3; slots.fill = false;
    long b = m.SetFXSlotProperty(9, 1, 0, 0, 0);
    ok = ok && b == -3 && m.commits == 2 && m.committed == 0 && m.committedVoice == 0;
    if (!ok) { printf("00c51eb0_TEST FAIL\n"); return 1; }
    printf("00c51eb0_TEST PASS\n");
    return 0;
}