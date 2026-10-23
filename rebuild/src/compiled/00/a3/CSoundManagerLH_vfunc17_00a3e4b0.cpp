// CSoundManagerLH::vfunc_17(const CCharString& name, long e, long d, long b, long a, long c) -- when the sound system exists and the manager is enabled, fill a 24-byte request {a, 4, b, c, d, e} and hand it with the name's characters to the system's player (slot 5 then slot 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3E4B0, length 107 bytes (`ret 18h`). Slot 17 of vtable 0x0129D2C4 (CSoundManagerLH).
// Callees: CCharString::operator const char* @ 0x0099AF10, system virtual slot 5 (+0x14), player virtual slot 1 (+0x04).
// Layout: local model as the landed CSoundManagerLH slots (enabled byte +0x25, system pointer +0x40); the request words'
// meaning is not recovered, so the parameters are named by their slot in the request.
// Shape: the request is filled by six assignments in retail store order (b, d, e, c, the constant 4, a); member order or
// a constructor schedules the loads differently. The system pointer is re-read for the call.
// Leaf: vfunc_17
namespace N_0099af10 { struct CCharString {
    void* PStringData;
    operator const char*() const;                   // 0x0099AF10
}; } using N_0099af10::CCharString;

struct SSoundParams {                               // 24-byte request block
    long m_a;                                       // +0x00 = arg 5
    long m_type;                                    // +0x04 = 4
    long m_b;                                       // +0x08 = arg 4
    long m_c;                                       // +0x0c = arg 6
    long m_d;                                       // +0x10 = arg 3
    long m_e;                                       // +0x14 = arg 2
};

struct CSoundPlayer {
    virtual void v00();
    virtual void Play(const char* name, const SSoundParams* params);   // slot 1
};
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CSoundPlayer* GetPlayer();                                 // slot 5
};

struct CSoundManagerLH {                            // vtable 0x0129D2C4
    virtual void v00();
    unsigned char _04[0x21];
    bool          m_enabled;                        // +0x25
    unsigned char _26[0x1a];
    CSoundSystem* m_pSystem;                        // +0x40
    void vfunc_17(const CCharString& name, long e, long d, long b, long a, long c);
};

void CSoundManagerLH::vfunc_17(const CCharString& name, long e, long d, long b, long a, long c)
{
    if (m_pSystem && m_enabled) {
        SSoundParams p;
        p.m_b = b;
        p.m_d = d;
        p.m_e = e;
        p.m_c = c;
        p.m_type = 4;
        p.m_a = a;
        m_pSystem->GetPlayer()->Play(name, &p);
    }
}