// Behaviour test for CBankProcessor::AddSampleNameTriggers (0x00C2E610): for each driver record whose number equals its sample
// number, a trigger named after the sample file (no extension, upper-cased) gets the driver appended; a missing driver and a
// duplicate name are logged instead; nothing happens without a registry or with the option off.
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
struct SSampleHeader { unsigned long m_number; std::string m_fileName; };
struct SDriverRecord { unsigned long m_number; unsigned long m_sample; };
struct CDriver { int id; };
static std::string StripExtension(const std::string& s) { std::string::size_type dot = s.rfind('.'); return dot == std::string::npos ? s : s.substr(0, dot); }
static void MakeUpper(std::string& s) { for (std::string::size_type i = 0; i < s.size(); ++i) s[i] = (char)toupper((unsigned char)s[i]); }
struct CBankProcessor {
    std::vector<SSampleHeader> samples; std::vector<SDriverRecord> records; std::map<unsigned long, CDriver*> drivers;
    std::map<std::string, std::vector<CDriver*> >* criteriaTriggers; bool sampleNameTriggers;
    std::vector<std::string> log;
    SSampleHeader* FindSample(unsigned long n) { for (unsigned i = 0; i < samples.size(); ++i) if (samples[i].m_number == n) return &samples[i]; return 0; }
    void AddSampleNameTriggers();
};
void CBankProcessor::AddSampleNameTriggers()
{
    if (!criteriaTriggers || !sampleNameTriggers)
        return;
    for (unsigned long i = 0; i < records.size(); ++i) {
        SDriverRecord* record = &records[i];
        CDriver* driver = drivers.count(record->m_number) ? drivers[record->m_number] : 0;
        if (!driver) {
            log.push_back("Could not find driver number");
        }
        else if (record->m_number == record->m_sample) {
            SSampleHeader* sample = FindSample(record->m_sample);
            std::string name = StripExtension(sample->m_fileName);
            MakeUpper(name);
            if (criteriaTriggers->count(name))
                log.push_back("Duplicate event trigger for string '" + name + "'");
            else
                (*criteriaTriggers)[name].push_back(driver);
        }
    }
}
int main(void)
{
    CDriver d1, d2, d3, d4; d1.id = 1; d2.id = 2; d3.id = 3; d4.id = 4;
    CBankProcessor p; std::map<std::string, std::vector<CDriver*> > registry;
    SSampleHeader s; s.m_number = 1; s.m_fileName = "sword_hit.wav"; p.samples.push_back(s);
    s.m_number = 2; s.m_fileName = "Sword_Hit.ogg"; p.samples.push_back(s);
    s.m_number = 4; s.m_fileName = "door"; p.samples.push_back(s);
    SDriverRecord r;
    r.m_number = 1; r.m_sample = 1; p.records.push_back(r);     // own sample: trigger SWORD_HIT
    r.m_number = 2; r.m_sample = 2; p.records.push_back(r);     // same name after stripping + upper-casing: duplicate
    r.m_number = 3; r.m_sample = 1; p.records.push_back(r);     // plays another driver's sample: skipped silently
    r.m_number = 4; r.m_sample = 4; p.records.push_back(r);     // no extension
    r.m_number = 9; r.m_sample = 9; p.records.push_back(r);     // unknown driver: logged
    p.drivers[1] = &d1; p.drivers[2] = &d2; p.drivers[3] = &d3; p.drivers[4] = &d4;
    p.criteriaTriggers = 0; p.sampleNameTriggers = true; p.AddSampleNameTriggers();
    bool ok = registry.empty() && p.log.empty();
    p.criteriaTriggers = &registry; p.sampleNameTriggers = false; p.AddSampleNameTriggers();
    ok = ok && registry.empty() && p.log.empty();
    p.sampleNameTriggers = true; p.AddSampleNameTriggers();
    ok = ok && registry.size() == 2 && registry["SWORD_HIT"].size() == 1 && registry["SWORD_HIT"][0] == &d1 &&
         registry["DOOR"].size() == 1 && registry["DOOR"][0] == &d4 && p.log.size() == 2 &&
         p.log[0] == "Duplicate event trigger for string 'SWORD_HIT'" && p.log[1] == "Could not find driver number";
    if (!ok) { printf("00c2e610_TEST FAIL\n"); return 1; }
    printf("00c2e610_TEST PASS\n");
    return 0;
}