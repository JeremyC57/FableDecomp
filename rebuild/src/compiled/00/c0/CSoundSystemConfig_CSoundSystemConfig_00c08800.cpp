// CSoundSystemConfig::CSoundSystemConfig() -- the 0x1c-byte voice configuration record: the two channel mask words zeroed, the defaults 4 / 0x1000 / 2 / 0x800 / 3 / 1
// Leaf: ??0CSoundSystemConfig@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08800, 44 bytes (thiscall, returns `this`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the record the landed CSoundManagerLH ctor 0x00A3EF00 builds, fills with
//   SetChannel 0x00C087C0 and hands to CSoundSystem::Initialise (slot 4); its layout is the landed 0x00A3EF00 model. Was a `C::Init` stub.
class CSoundSystemConfig { public:
    unsigned short ChannelMask;                 // +0x00
    unsigned short ChannelTouched;              // +0x02
    long           Long04;                      // +0x04 default 4
    long           Long08;                      // +0x08 default 0x1000
    long           Long0C;                      // +0x0c default 2
    long           Long10;                      // +0x10 default 0x800
    long           Long14;                      // +0x14 default 3
    long           Long18;                      // +0x18 default 1
    CSoundSystemConfig();
};
CSoundSystemConfig::CSoundSystemConfig()
{
    ChannelMask = 0; ChannelTouched = 0; Long04 = 4; Long08 = 0x1000; Long0C = 2; Long10 = 0x800; Long14 = 3; Long18 = 1;
}
