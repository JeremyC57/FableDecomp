// Test for 0x0075DC00 CTCCameraPointTrack::Validate: links the source object; without the interface-0x44 shape manager
// it returns false and leaves the text alone; with it, it sets the error text and returns GetBestTrack's answer.
#include <stdio.h>
#include <string.h>
static const char* g_text;
static int g_assigns;
class CCharString { public: const CCharString& operator=(const char* text); };
const CCharString& CCharString::operator=(const char* text) { g_text = text; ++g_assigns; return *this; }
namespace NShape { class CTrack { public: int id; }; }
static NShape::CTrack g_track;
class CTCShapeManager { public: bool has; bool GetBestTrack(const NShape::CTrack** out) const; };
bool CTCShapeManager::GetBestTrack(const NShape::CTrack** out) const { if (!has) return false; *out = &g_track; return true; }
struct CInterfaceMap {
    struct SEntry { long type; CTCShapeManager* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const;
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
};
class CTCBase { public: virtual void v00(); CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
void CTCBase::v00() { }
class CTCCameraPointTrack : public CTCBase { public: virtual bool Validate(CCharString& error_text) const; };
int main()
{
    CThing t; memset(&t, 0, sizeof(t));
    CTCShapeManager sm; sm.has = true;
    CInterfaceMap::SEntry e[2] = { { 0x10, 0 }, { 0x44, &sm } };
    t.m_interfaces.first = e; t.m_interfaces.last = e + 2;
    CTCCameraPointTrack tc; tc.ParentThing = &t;
    CCharString s;
    bool ok = !tc.Validate(s) && g_assigns == 0;                       // bit 0x44 absent
    t.m_presentBits[0x44 >> 5] |= 1UL << (0x44 & 31);
    ok = ok && tc.Validate(s) && g_assigns == 1 && g_text && strstr(g_text, "CAMERA_POINT_TRACK") != 0;
    sm.has = false;
    ok = ok && !tc.Validate(s) && g_assigns == 2;
    if (!ok) { puts("0075dc00_TEST FAIL"); return 1; }
    puts("0075dc00_TEST PASS");
    return 0;
}
