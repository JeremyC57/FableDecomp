// Behaviour test for 0x00A3F250 CSoundManagerLH::AddSound (descriptive name, vtable slot 39): a model of the row; false when
// the (bank, sound) pair is already filed, the bank name is unknown, the sample bank id is unknown or the bank's slot 1
// returns null; otherwise the returned object is filed under the name pair (map then sorted) and true is returned.
#include <cstdio>
#include <string>
#include <map>
#include <utility>
struct Sample { long Id; };
struct Bank {
    int starts; long lastId, lastA, lastB; Bank* result;
    Bank* StartSound(long id, long a, long b) { ++starts; lastId = id; lastA = a; lastB = b; return result; }
};
struct Manager {
    std::map<std::pair<std::string, std::string>, Bank*> NamePairMap;
    std::map<std::string, Bank*> Banks;
    std::map<long, Sample*> Samples;
    int sorts;
    Bank* FindByNamePair(const std::string& a, const std::string& b)
    {
        std::map<std::pair<std::string, std::string>, Bank*>::iterator it = NamePairMap.find(std::make_pair(a, b));
        return it == NamePairMap.end() ? 0 : it->second;
    }
    Bank* FindByName(const std::string& n) { std::map<std::string, Bank*>::iterator it = Banks.find(n); return it == Banks.end() ? 0 : it->second; }
    Sample* GetBank(long id) { std::map<long, Sample*>::iterator it = Samples.find(id); return it == Samples.end() ? 0 : it->second; }
    bool AddSound(const std::string& bankName, const std::string& soundName, long sample, long a, long b)
    {
        if (FindByNamePair(bankName, soundName))
            return false;
        Bank* bank = FindByName(bankName);
        if (!bank)
            return false;
        Sample* info = GetBank(sample);
        if (!info)
            return false;
        long sampleId = info->Id;
        Bank* sound = bank->StartSound(sampleId, a, b);
        if (!sound)
            return false;
        NamePairMap[std::make_pair(bankName, soundName)] = sound;
        ++sorts;
        return true;
    }
};
int main()
{
    Manager m; m.sorts = 0;
    Bank bank = { 0, 0, 0, 0, 0 }; Bank started = { 0, 0, 0, 0, 0 };
    Sample s = { 77 };
    m.Banks["bank"] = &bank; m.Samples[5] = &s;
    int ok = 1;
    ok &= !m.AddSound("nobank", "x", 5, 1, 2);
    ok &= !m.AddSound("bank", "x", 6, 1, 2);                         // unknown sample
    ok &= !m.AddSound("bank", "x", 5, 1, 2) && bank.starts == 1;     // slot 1 returned null
    bank.result = &started;
    ok &= m.AddSound("bank", "x", 5, 3, 4) && bank.lastId == 77 && bank.lastA == 3 && bank.lastB == 4 && m.sorts == 1;
    ok &= m.FindByNamePair("bank", "x") == &started;
    ok &= !m.AddSound("bank", "x", 5, 3, 4) && bank.starts == 2;     // already filed
    std::printf(ok ? "00a3f250_TEST PASS\n" : "00a3f250_TEST FAIL\n");
    return ok ? 0 : 1;
}