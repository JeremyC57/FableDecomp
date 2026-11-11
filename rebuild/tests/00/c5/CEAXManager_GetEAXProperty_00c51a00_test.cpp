// Behaviour test for CEAXManager::GetEAXProperty (0x00C51A00): id, data and size reach the listener getter, the property set and the
// voice are ignored, and the result is always 0 whatever the getter returned.
#include <stdio.h>
struct GUIDish { int v; };
struct CEAXVoice { int v; };
struct CEAXManager {
    unsigned long seenId; void* seenData; unsigned long seenSize; int calls; long getterResult;
    long GetListenerProperty(unsigned long id, void* data, unsigned long size) { seenId = id; seenData = data; seenSize = size; ++calls; return getterResult; }
    long GetEAXProperty(const GUIDish* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size);
};
long CEAXManager::GetEAXProperty(const GUIDish* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size)
{
    GetListenerProperty(id, data, size);
    return 0;
}
int main(void)
{
    CEAXManager m; m.calls = 0; m.getterResult = -9;
    GUIDish set; CEAXVoice voice; char buffer[8];
    long r = m.GetEAXProperty(&set, 5, &voice, buffer, sizeof(buffer));
    if (r != 0 || m.calls != 1 || m.seenId != 5 || m.seenData != buffer || m.seenSize != sizeof(buffer)) { printf("00c51a00_TEST FAIL\n"); return 1; }
    printf("00c51a00_TEST PASS\n");
    return 0;
}