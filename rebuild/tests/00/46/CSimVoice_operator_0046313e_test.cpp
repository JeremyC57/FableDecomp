// Behaviour test for CSimVoice::operator= (0x0046313E): a model of the row -- the base Valid flag, the Voices entries and the
// map's Dirty flag are copied and the assignment returns the target.
#include <stdio.h>
#include <vector>
struct CInitBaseClass { bool Valid; };
struct CVoiceMap {
    std::vector<std::pair<int, int> > Data; char Compare; bool Dirty;
    CVoiceMap& operator=(const CVoiceMap& o) { Data = o.Data; Compare = o.Compare; Dirty = o.Dirty; return *this; }
};
struct CSimVoice : CInitBaseClass {
    CVoiceMap Voices;
    CSimVoice& operator=(const CSimVoice& o) { CInitBaseClass::operator=(o); Voices = o.Voices; return *this; }
};
int main()
{
    CSimVoice a, b;
    a.Valid = true; a.Voices.Data.push_back(std::make_pair(2, 11)); a.Voices.Dirty = true;
    b.Valid = false; b.Voices.Dirty = false;
    CSimVoice* r = &(b = a);
    int ok = r == &b && b.Valid && b.Voices.Data.size() == 1 && b.Voices.Data[0].second == 11 && b.Voices.Dirty;
    printf(ok ? "0046313e_TEST PASS\n" : "0046313e_TEST FAIL\n");
    return ok ? 0 : 1;
}