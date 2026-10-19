// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing*) -- initialise the icicle-assault animation filter and effect slots
// Retail: Fable.exe @ 0x00743BE0, 0x70 bytes. Base mode and intelligent pointer are followed by ICICLE_ASSAULT.
// Shape: this is the standard one-argument creature-mode ctor; the seven post-filter words are zeroed in declaration order.
// Leaf: ??0CModeIceTrollGroundSmashIcicleAssault
struct CThing;
struct CCharStringData { const char* text; unsigned long length; };
struct CCharString { CCharStringData* data; CCharString(); CCharString(const char*, long=-1); ~CCharString(); CCharString& operator=(const CCharString&); };
unsigned long __fastcall Calc(unsigned long, const void*, unsigned long); // 0x004014A0
struct CAnimFilterBase { virtual void v00(); };
struct CAnimFilterOfGroup : CAnimFilterBase {
    virtual void v00(); CCharString group; unsigned long crc;
    CAnimFilterOfGroup(const CCharString& s): group() { group=s; const char* p; unsigned long n; if(!s.data){p="";n=0;} else {p=s.data->text;n=s.data->length;} crc=Calc(0,p,n); }
};
struct CBaseIntelligentPointer { virtual void v00(); void* item; CBaseIntelligentPointer(); };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00(); CIntelligentPointer() {} };
struct CModeBase { virtual void v00(); unsigned char body[0x64]; CModeBase(CThing*); };
struct CModeIceTrollGroundSmashIcicleAssault : CModeBase {
    CIntelligentPointer target; CAnimFilterOfGroup filter; unsigned long pad7c; unsigned long x80,x84,x88,x8c,x90,x94,x98;
    virtual void v00();
    CModeIceTrollGroundSmashIcicleAssault(CThing*);
};
CModeIceTrollGroundSmashIcicleAssault::CModeIceTrollGroundSmashIcicleAssault(CThing* thing)
 : CModeBase(thing), target(), filter("ICICLE_ASSAULT"), x80(0),x84(0),x88(0),x8c(0),x90(0),x94(0),x98(0) {}
