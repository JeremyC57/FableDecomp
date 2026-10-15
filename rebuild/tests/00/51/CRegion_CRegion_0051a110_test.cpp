// Behaviour test for CRegion::~CRegion (0x0051A110): member teardown order and effects.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_freed = 0;
static void FreeBlock(void* p) { if (p) ++g_freed; }

struct CCharString {
    const char* tag;
    ~CCharString() { Log(tag); }
};

struct CDefPointeeBase {
    long RefCount; int destroyed;
    void Destroy() { ++destroyed; }
};

struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object != 0) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
            Log("D");
        }
    }
};

struct CLongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage; const char* tag;
    ~CLongVector() { if (_M_start != 0) { FreeBlock(_M_start); Log(tag); } }
};

struct SExitTextNode { long _M_color; SExitTextNode* _M_parent; SExitTextNode* _M_left; SExitTextNode* _M_right; };

struct CExitTextOffsetMap {
    SExitTextNode* _M_header; unsigned long _M_node_count; int erased;
    void _M_erase(SExitTextNode* x) { if (x) ++erased; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CExitTextOffsetMap() { clear(); Log("M"); }
};

struct CRegion {
    CLongVector Maps; CLongVector VisMaps;
    CCharString Name; CCharString DisplayName; CCharString RegionDefName;
    CDefPointer PDef; CCharString MiniMapGraphic;
    CExitTextOffsetMap MiniMapRegionExitTextOffsets;
};

int main()
{
    long buf1[2], buf2[2];
    SExitTextNode header, root;
    header._M_parent = &root; header._M_left = &root; header._M_right = &root;
    CDefPointeeBase def; def.RefCount = 1; def.destroyed = 0;
    CExitTextOffsetMap* mapSeen = 0;
    {
        CRegion r;
        r.Maps._M_start = buf1; r.Maps.tag = "0";
        r.VisMaps._M_start = buf2; r.VisMaps.tag = "1";
        r.Name.tag = "N"; r.DisplayName.tag = "d"; r.RegionDefName.tag = "R";
        r.PDef.Object = &def; r.MiniMapGraphic.tag = "G";
        r.MiniMapRegionExitTextOffsets._M_header = &header;
        r.MiniMapRegionExitTextOffsets._M_node_count = 3;
        r.MiniMapRegionExitTextOffsets.erased = 0;
        mapSeen = &r.MiniMapRegionExitTextOffsets;
        (void)mapSeen;
    }
    // reverse member order: map, MiniMapGraphic, PDef, RegionDefName, DisplayName, Name, VisMaps, Maps
    if (std::strcmp(g_log, "MGDRdN10") != 0 || def.destroyed != 1 || def.RefCount != 0 || g_freed != 2
        || header._M_parent != 0 || header._M_left != &header || header._M_right != &header) {
        std::printf("0051a110_TEST FAIL\n");
        return 1;
    }
    std::printf("0051a110_TEST PASS\n");
    return 0;
}