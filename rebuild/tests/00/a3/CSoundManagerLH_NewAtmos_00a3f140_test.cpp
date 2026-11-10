#include <cstdio>

// CSoundManagerLH::AddBank (0x00A3F140) through a copy of the model: no device / taken name / failed creation -> false;
// otherwise every set channel bit is switched on (lowest first) and the bank is filed under its name, map kept sorted.
static int StrLess(const char* a, const char* b)
{
    for (;;) {
        if (*a == 0 && *b == 0) return 0;
        if (*a < *b) return 1;
        if (*a > *b) return 0;
        ++a;
        ++b;
    }
}
class CCharString { public:
    const char* PData;
    bool operator<(const CCharString& o) const { return StrLess(PData, o.PData) != 0; }
};

static int FindFirstSetBit(unsigned int mask, unsigned int numBytes)
{
    for (unsigned int i = 0; i < numBytes * 8; ++i)
        if (mask & (1u << i))
            return (int)i;
    return -1;
}

struct CSoundChannelSet {
    int Calls[8];
    int CallCount;
    CSoundChannelSet() : CallCount(0) { }
    virtual void SetChannel(int channel, bool on) { if (on && CallCount < 8) Calls[CallCount++] = channel; }
};
struct CSoundBankLH {
    CSoundChannelSet Channels;
    virtual CSoundChannelSet* GetChannels() { return &Channels; }
};
struct CSoundBankFactory {
    CSoundBankLH Banks[4];
    int Used;
    bool Fail;
    CSoundBankFactory() : Used(0), Fail(false) { }
    virtual CSoundBankLH* CreateBank() { return Fail || Used == 4 ? 0 : &Banks[Used++]; }
};
struct CSoundDevice {
    CSoundBankFactory Factory;
    virtual CSoundBankFactory* GetFactory() { return &Factory; }
};

struct SEntry { CCharString first; CSoundBankLH* second; };
struct CNameMap {                                   // CVectorMap<CCharString, CSoundBankLH*>
    SEntry Data[8];
    int Size;
    bool Dirty;
    CNameMap() : Size(0), Dirty(false) { }
    void Add(const CCharString& key, CSoundBankLH* value) { Data[Size].first = key; Data[Size].second = value; ++Size; Dirty = true; }
    void Sort()
    {
        if (Dirty) {
            for (int i = 1; i < Size; ++i)
                for (int j = i; j > 0 && Data[j].first < Data[j - 1].first; --j) {
                    SEntry t = Data[j]; Data[j] = Data[j - 1]; Data[j - 1] = t;
                }
            Dirty = false;
        }
    }
    CSoundBankLH* Find(const CCharString& key) const
    {
        for (int i = 0; i < Size; ++i)
            if (!(Data[i].first < key) && !(key < Data[i].first)) return Data[i].second;
        return 0;
    }
};

struct CSoundManagerLH {
    CSoundDevice* Device;
    CNameMap NameMap;
    CSoundBankLH* FindByName(const CCharString& name) { return NameMap.Find(name); }
    bool AddBank(const CCharString& name, unsigned int channels)
    {
        if (Device == 0)
            return false;
        if (FindByName(name) != 0)
            return false;
        CSoundBankLH* bank = Device->GetFactory()->CreateBank();
        if (bank == 0)
            return false;
        while (channels) {
            int channel = FindFirstSetBit(channels, 16);
            bank->GetChannels()->SetChannel(channel, true);
            channels ^= 1u << channel;
        }
        NameMap.Add(name, bank);
        NameMap.Sort();
        return true;
    }
};

static int Fail()
{
    std::printf("00a3f140_TEST FAIL\n");
    return 1;
}

int main()
{
    CSoundManagerLH manager;
    manager.Device = 0;
    CCharString music = { "music" }, ambient = { "ambient" }, voice = { "voice" };
    if (manager.AddBank(music, 1)) return Fail();               // no device
    CSoundDevice device;
    manager.Device = &device;
    if (!manager.AddBank(music, 0x29)) return Fail();          // channels 0, 3, 5
    CSoundChannelSet& set = device.Factory.Banks[0].Channels;
    if (set.CallCount != 3 || set.Calls[0] != 0 || set.Calls[1] != 3 || set.Calls[2] != 5) return Fail();
    if (manager.AddBank(music, 2)) return Fail();              // name taken
    if (!manager.AddBank(ambient, 0x80000000u)) return Fail(); // channel 31
    if (device.Factory.Banks[1].Channels.CallCount != 1 || device.Factory.Banks[1].Channels.Calls[0] != 31) return Fail();
    if (manager.NameMap.Size != 2 || manager.NameMap.Dirty) return Fail();
    if (manager.NameMap.Data[0].second != &device.Factory.Banks[1]) return Fail();   // "ambient" sorted first
    device.Factory.Fail = true;
    if (manager.AddBank(voice, 1)) return Fail();              // creation failed
    if (manager.NameMap.Size != 2) return Fail();
    std::printf("00a3f140_TEST PASS\n");
    return 0;
}