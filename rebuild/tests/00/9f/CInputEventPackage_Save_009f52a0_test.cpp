// Behaviour test for CInputEventPackage::Save (0x009F52A0): the header fields and each event (rebased start time,
// cleared current time) are written in order.
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
class CInputEvent {                                // 0x34 bytes
public:
    unsigned char _data[0x20];
    int           DeviceType;                      // +0x20 (int: 4 bytes on the 64-bit host too)
    int           DeviceNumber;                    // +0x24
    int           Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    CInputEvent() {}
    CInputEvent(const CInputEvent& other) { memcpy(this, &other, sizeof(*this)); }
    ~CInputEvent() {}
};
double __fastcall GFGetTime() { return 100.0; }
struct CChecksum { unsigned long Words[4]; };      // 16 bytes
struct C2DVector { float X, Y; };
class CDataOutputStream {
public:
    unsigned char m_buf[1024]; long m_pos; int m_badFlags;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void Write(const void* data, long size, long flags) { if (flags) ++m_badFlags; memcpy(m_buf + m_pos, data, size); m_pos += size; }
};
template <class T> struct CRetailVector {          // STLport vector: first / last / end
    T* m_first; T* m_last; T* m_end;
    long size() const { return m_last - m_first; }
    T& operator[](long n) const { return *(m_first + n); }
};
class CInputEventPackage {
public:
    CRetailVector<CInputEvent> Events;             // +0x00
    long                       TimeStamp;          // +0x0c
    CChecksum                  Checksum;           // +0x10
    C2DVector                  MousePos;           // +0x20
    C2DVector                  JoystickPos;        // +0x28
    void Save(CDataOutputStream& stream) const;
};

void CInputEventPackage::Save(CDataOutputStream& stream) const
{
    long count = Events.size();
    stream.Write(&count, 4, 0);
    stream.Write(&TimeStamp, 4, 0);
    stream.Write(&Checksum, 0x10, 0);
    stream.Write(&MousePos, 8, 0);
    stream.Write(&JoystickPos, 8, 0);
    for (long i = 0; i < count; ++i) {
        CInputEvent event(Events[i]);
        double startTime = event.StartTime;
        event.CurrentTime = 0.0f;
        event.StartTime = (float)GFGetTime() - startTime;
        stream.Write(&event, 0x34, 0);
    }
}

int main()
{
    static CInputEvent ev[2];
    memset(ev, 0, sizeof(ev));
    ev[0].Type = 7; ev[0].StartTime = 40.0f; ev[0].CurrentTime = 3.0f;
    ev[1].Type = 9; ev[1].StartTime = 75.0f; ev[1].CurrentTime = 4.0f;
    CInputEventPackage p; p.Events.m_first = ev; p.Events.m_last = ev + 2; p.Events.m_end = ev + 2;
    p.TimeStamp = 1234; memset(&p.Checksum, 0x5a, sizeof(p.Checksum)); p.MousePos.X = 1.0f; p.MousePos.Y = 2.0f;
    p.JoystickPos.X = 3.0f; p.JoystickPos.Y = 4.0f;
    static CDataOutputStream s; s.m_pos = 0; s.m_badFlags = 0;
    p.Save(s);
    int bad = 0;
    int v; float f;
    if (s.m_pos != 4 + 4 + 16 + 8 + 8 + 2 * 0x34 || s.m_badFlags) bad = 1;
    memcpy(&v, s.m_buf, 4); if (v != 2) bad = 2;
    memcpy(&v, s.m_buf + 4, 4); if (v != 1234) bad = 3;
    if (s.m_buf[8] != 0x5a || s.m_buf[23] != 0x5a) bad = 4;
    memcpy(&f, s.m_buf + 24, 4); if (f != 1.0f) bad = 5;
    memcpy(&f, s.m_buf + 36, 4); if (f != 4.0f) bad = 6;
    const unsigned char* e0 = s.m_buf + 40;
    memcpy(&v, e0 + 0x28, 4); if (v != 7) bad = 7;
    memcpy(&f, e0 + 0x30, 4); if (f != 60.0f) bad = 8;
    memcpy(&f, e0 + 0x2c, 4); if (f != 0.0f) bad = 9;
    memcpy(&f, e0 + 0x34 + 0x30, 4); if (f != 25.0f) bad = 10;
    if (ev[0].StartTime != 40.0f) bad = 11;          // the package itself is untouched
    if (bad) { std::printf("009f52a0_TEST FAIL\n"); return 1; }
    std::printf("009f52a0_TEST PASS\n");
    return 0;
}