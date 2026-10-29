// Reconstruction of CVolume::~CVolume (retail 0x009fccf0)
// Leaf: ??1CVolume@
// Pattern: set vtable, call a member cleanup helper with arg 1, tail-call base dtor.

class CBaseClass {
public:
    CBaseClass();
    virtual ~CBaseClass();               // 0x0099a2e0 (base destructor, tail-called)
};

class CVolume : public CBaseClass {
public:
    CVolume();
    ~CVolume();
    void ReleaseResources(int flag);   // 0x00a67460 (member helper, __fastcall + stack arg)
};

CVolume::~CVolume()
{
    ReleaseResources(1);
}