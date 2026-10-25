// Behaviour test for 0x007B3760 CTCGuard::OnSerialise(CPersistContext&, bool): the pending-crime deque round-trips
// through a vector, then six members are transferred with 0 defaults in a fixed key order.
#include <cstdio>
#include <cstring>
#include <deque>
#include <vector>
#include <string>

struct Crime { int DeedHandle; int Type; };

struct Context {
    int Mode;                                               // 1 / 3 save, 0 / 2 load
    std::vector<Crime> stored;
    std::string keys;
    int values[8];
    int nValues;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void TransferVector(const char* name, std::vector<Crime>& v)
    {
        keys += name; keys += ";";
        if (IsSaving()) stored = v; else v = stored;
    }
    void Transfer(const char* name, int& value, int def)
    {
        keys += name; keys += ";";
        if (IsSaving()) { if (nValues < 8) values[nValues] = value; ++nValues; }
        else { value = nValues < 8 ? values[nValues] : def; ++nValues; }
    }
};

struct Guard {
    int BribePool;
    std::deque<Crime> PendingCrimes;
    int FramePendingCrimesAdded, FrameLastBribeAdded, LastCrimeSeenSeverity, FrameLastCrimeSeen, FrameLastReceivedApology;
    void OnSerialise(Context& c)
    {
        if (c.IsSaving()) {
            std::vector<Crime> crimes;
            for (std::deque<Crime>::iterator it = PendingCrimes.begin(); it != PendingCrimes.end(); ++it) crimes.push_back(*it);
            c.TransferVector("PendingCrimes", crimes);
        } else {
            std::vector<Crime> crimes;
            c.TransferVector("PendingCrimes", crimes);
            for (unsigned int i = 0; i < crimes.size(); ++i) PendingCrimes.push_back(crimes[i]);
        }
        c.Transfer("FramePendingCrimesAdded", FramePendingCrimesAdded, 0);
        c.Transfer("FrameLastBribeAdded", FrameLastBribeAdded, 0);
        c.Transfer("FrameLastCrimeSeen", FrameLastCrimeSeen, 0);
        c.Transfer("FrameLastReceivedApology", FrameLastReceivedApology, 0);
        c.Transfer("BribePool", BribePool, 0);
        c.Transfer("LastCrimeSeenSeverity", LastCrimeSeenSeverity, 0);
    }
};

int main()
{
    int ok = 1;
    Guard g;
    for (int i = 0; i < 70; ++i) { Crime c; c.DeedHandle = 100 + i; c.Type = i % 3; g.PendingCrimes.push_back(c); }
    g.BribePool = 500; g.FramePendingCrimesAdded = 11; g.FrameLastBribeAdded = 22; g.LastCrimeSeenSeverity = 2;
    g.FrameLastCrimeSeen = 33; g.FrameLastReceivedApology = 44;
    Context c;
    c.Mode = 3; c.nValues = 0;
    g.OnSerialise(c);
    if (c.stored.size() != 70 || c.stored[69].DeedHandle != 169 || c.nValues != 6) ok = 0;
    if (c.values[0] != 11 || c.values[1] != 22 || c.values[2] != 33 || c.values[3] != 44 || c.values[4] != 500 ||
        c.values[5] != 2)
        ok = 0;
    if (c.keys != "PendingCrimes;FramePendingCrimesAdded;FrameLastBribeAdded;FrameLastCrimeSeen;"
                  "FrameLastReceivedApology;BribePool;LastCrimeSeenSeverity;")
        ok = 0;
    Guard h;
    h.BribePool = 0;
    Context l = c;
    l.Mode = 2; l.nValues = 0; l.keys = "";
    h.OnSerialise(l);                                       // load appends the stored records to the deque
    if (h.PendingCrimes.size() != 70 || h.PendingCrimes[64].DeedHandle != 164 || h.BribePool != 500 ||
        h.LastCrimeSeenSeverity != 2 || h.FrameLastReceivedApology != 44)
        ok = 0;
    if (!ok) { std::printf("007b3760_TEST FAIL\n"); return 1; }
    std::printf("007b3760_TEST PASS\n");
    return 0;
}