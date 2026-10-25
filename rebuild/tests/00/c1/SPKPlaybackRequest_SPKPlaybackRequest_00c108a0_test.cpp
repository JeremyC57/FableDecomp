#include <cstdio>
#include <cstring>
#include <new>
// Behaviour model of SPKPlaybackRequest::SPKPlaybackRequest() @ 0x00C108A0 (libpk, descriptive name): zero event id,
// handle constructed by SPKHandle() (stand-in), user data and callback zeroed; volume/pitch/flags left to the caller.
static int gHandleCtorCalls = 0;
struct SPKHandle {
    void*         m_pObject;
    unsigned int  m_id;
    unsigned int  m_extra;
    bool          m_valid;
    SPKHandle() : m_pObject(0), m_id(0xFFFFFFFFu), m_extra(0), m_valid(false) { ++gHandleCtorCalls; }   // stand-in for 0x00C20390
};
struct SPKPlaybackRequest {
    unsigned int m_eventId;
    SPKHandle    m_handle;
    float        m_volume;
    float        m_pitch;
    unsigned int m_flags;
    unsigned int m_userData;
    unsigned int m_callback;
    SPKPlaybackRequest();
};
SPKPlaybackRequest::SPKPlaybackRequest() : m_eventId(0), m_userData(0), m_callback(0)
{
}
int main()
{
    union { double align; unsigned char bytes[sizeof(SPKPlaybackRequest)]; } buf;
    std::memset(buf.bytes, 0xAB, sizeof(buf.bytes));
    SPKPlaybackRequest* r = new (buf.bytes) SPKPlaybackRequest;
    unsigned int untouched;
    std::memset(&untouched, 0xAB, sizeof(untouched));
    bool ok = r->m_eventId == 0 && r->m_userData == 0 && r->m_callback == 0 && gHandleCtorCalls == 1 &&
              r->m_handle.m_id == 0xFFFFFFFFu && r->m_handle.m_pObject == 0 && r->m_flags == untouched;
    if (!ok) { std::printf("00c108a0_TEST FAIL\n"); return 1; }
    std::printf("00c108a0_TEST PASS\n");
    return 0;
}