// Behaviour test for 0075dc00 (CTCCameraPointTrack::vfunc_17): without the interface-0x44 component it returns false and leaves the text; with it, it sets the error text and returns the track lookup.
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* m_text;
    const CCharString& operator=(const char* text) { m_text = text; return *this; }
};

struct CCameraTrack { long m_id; };

struct CTCCameraTrackOwner {
    CCameraTrack* m_track;
    bool GetTrack(CCameraTrack** out)
    {
        if (!m_track) return false;
        *out = m_track;
        return true;
    }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCCameraTrackOwner* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCBase { CThing* ParentThing; };

struct CTCCameraPointTrack : CTCBase {
    bool vfunc_17(CCharString* errorText)
    {
        CTCCameraTrackOwner* owner;
        if (ParentThing->GetInterface(0x44, &owner)) {
            *errorText = "There is a CAMERA_POINT_TRACK with no track associated with it. Have a nice day.";
            CCameraTrack* track;
            return owner->GetTrack(&track);
        }
        return false;
    }
};

int main()
{
    CCameraTrack track = { 1 };
    CTCCameraTrackOwner owner; owner.m_track = 0;
    CInterfaceMap::SEntry entries[1];
    entries[0].type = 0x44; entries[0].component = &owner;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCCameraPointTrack cp; cp.ParentThing = &thing;
    CCharString text; text.m_text = "";
    bool ok = !cp.vfunc_17(&text) && text.m_text[0] == 0;
    thing.m_presentBits[2] |= 1UL << 4;
    ok = ok && !cp.vfunc_17(&text) && std::strncmp(text.m_text, "There is a CAMERA_POINT_TRACK", 29) == 0;
    owner.m_track = &track;
    ok = ok && cp.vfunc_17(&text);
    if (!ok) { std::printf("0075dc00_TEST FAIL\n"); return 1; }
    std::printf("0075dc00_TEST PASS\n");
    return 0;
}