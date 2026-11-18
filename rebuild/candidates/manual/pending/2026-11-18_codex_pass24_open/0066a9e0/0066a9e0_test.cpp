#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
struct C3DVector { float X,Y,Z; };
typedef std::string CCharString;
struct CDefString {
    CCharString name;
    explicit CDefString(const char* s):name(s) {}
    const CCharString& GetString() const {return name;}
};
CCharString operator+(const CCharString& first,const CDefString& second){return first+second.name;}
struct CMaterialDef {
    CDefString name; mutable unsigned reads;
    explicit CMaterialDef(const char* s):name(s),reads(0) {}
    const CDefString& GetInstantiationName() const {++reads;return name;}
};
template<class T> struct CDefPointer {
    T* data;
    CDefPointer(T* value):data(value) {}
    bool IsNull()const{return !data;}
    T* operator->()const{return data;}
};
struct CASoundBank {
    std::string playable;
    std::vector<std::string> attempts;
    const void* received_pos;
};
struct CThing {
    CASoundBank bank;
    CASoundBank& GetSampleBank(){return bank;}
};
bool g_bSoundEnabled;
const unsigned long g_InvalidSoundHandle=~0UL;
struct CTCSoundPlayer {
    CThing thing;
    CThing& GetParentThing(){return thing;}
    unsigned long InternalPlaySoundEventAtPos(CASoundBank& bank,const C3DVector& pos,const CCharString& name,bool loop,bool attached) {
        assert(!loop && !attached);
        bank.received_pos=&pos; bank.attempts.push_back(name);
        return bank.playable==name ? 1 : g_InvalidSoundHandle;
    }
    void PlayCollisionSound(const C3DVector&,const CDefPointer<const CMaterialDef>&,const CDefPointer<const CMaterialDef>&);
};
void CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& pmaterial1,
                                        const CDefPointer<const CMaterialDef>& pmaterial2)
{
    if (g_bSoundEnabled)
    {
        CASoundBank& bank = GetParentThing().GetSampleBank();
        CCharString first_part("SE_COLLISION;");
        if (pmaterial1.IsNull())
        {
            if (!pmaterial2.IsNull())
                InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial2->GetInstantiationName(), false, false);
        }
        else if (pmaterial2.IsNull())
        {
            InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false);
        }
        else if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName() + ";" + pmaterial2->GetInstantiationName(), false, false) == g_InvalidSoundHandle)
        {
            if (InternalPlaySoundEventAtPos(bank, pos, first_part + pmaterial1->GetInstantiationName(), false, false) == g_InvalidSoundHandle) {
                const CCharString& name = pmaterial2->GetInstantiationName().GetString();
                InternalPlaySoundEventAtPos(bank, pos, first_part + name, false, false);
            }
        }
    }
}

int main()
{
    const char* names[] = {"SE_COLLISION;wood;stone", "SE_COLLISION;wood", "SE_COLLISION;stone", ""};
    const C3DVector pos={1.0f,-2.0f,3.0f};
    for (int enabled = 0; enabled < 2; ++enabled) {
        for (int mask = 0; mask < 4; ++mask) {
            for (int sound = 0; sound < 4; ++sound) {
                CTCSoundPlayer tc;
                CASoundBank& bank=tc.thing.bank;
                bank.received_pos = 0;
                bank.playable = names[sound];
                CMaterialDef wood("wood"), stone("stone");
                g_bSoundEnabled=enabled!=0;
                tc.PlayCollisionSound(pos,
                                   mask & 1 ? &wood : 0, mask & 2 ? &stone : 0);
                std::vector<std::string> expected;
                if (enabled) {
                    if (mask == 3) {
                        expected.push_back(names[0]);
                        if (sound != 0)
                            expected.push_back(names[1]);
                        if (sound >= 2)
                            expected.push_back(names[2]);
                    } else if (mask == 1 || mask == 2) {
                        expected.push_back(names[mask]);
                    }
                }
                assert(bank.attempts == expected);
                assert(bank.received_pos == (expected.empty() ? 0 : &pos));
                unsigned first_reads = 0, second_reads = 0;
                for (unsigned i = 0; i < expected.size(); ++i) {
                    if (expected[i].find("wood") != std::string::npos)
                        ++first_reads;
                    if (expected[i].find("stone") != std::string::npos)
                        ++second_reads;
                }
                assert(wood.reads == first_reads && stone.reads == second_reads);
            }
        }
    }
    std::puts("0066a9e0_TEST PASS");
    return 0;
}
