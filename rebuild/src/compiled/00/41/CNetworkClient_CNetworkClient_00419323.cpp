// CNetworkClient::CNetworkClient() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00419323, length 47 bytes.
// Manifest label "CAIStateGroup_GoToBed" is propagated; the vtable stored at +0, 0x0122EDAC, is CNetworkClient
// and the one stored at +8, 0x0122ED30, is CNetworkServer (rtti-vtables.tsv): the client embeds a server
// object. Callees: CInitBaseClass::CInitBaseClass @ 0x0099A310 (own base, and again as the embedded
// server's base through its inlined constructor), CGameEventPackage::CGameEventPackage @ 0x00419220 for
// the member at +0x2010.
//
// Shape: base ctor, own vtable, then the members in declaration order: the embedded CNetworkServer at
// +8 (inline ctor: base CInitBaseClass + its vtable; its body of 0x2008 bytes is not touched here) and
// the CGameEventPackage at +0x2010 constructed out of line. Member names are descriptive.
struct CInitBaseClass {
    virtual void v00();
    CInitBaseClass();                           // 0x0099A310
};

struct CNetworkServer : CInitBaseClass {        // vtable 0x0122ED30
    unsigned char _body[0x2004];                // +0x04 server state (socket table), untouched here
    virtual void v00();
    CNetworkServer() : CInitBaseClass() { }
};

struct CGameEventPackage {
    CGameEventPackage();                        // 0x00419220
};

struct CNetworkClient : CInitBaseClass {        // vtable 0x0122EDAC
    long m_connection;                          // +0x04 (uninitialised here)
    CNetworkServer m_server;                    // +0x08
    CGameEventPackage m_package;                // +0x2010
    virtual void v00();
    CNetworkClient();
};

CNetworkClient::CNetworkClient()
    : CInitBaseClass()
    , m_server()
    , m_package()
{
}