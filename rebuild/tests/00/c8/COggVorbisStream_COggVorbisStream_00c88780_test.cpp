// Behaviour test for 00c88780: COggVorbisStream::COggVorbisStream -- open the reader as Ogg Vorbis; valid only when seekable and every link is stereo
#include <cstdio>
#include <cstring>

typedef struct vorbis_info { int version; int channels; long rate; } vorbis_info;
typedef struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; } vorbis_comment;
typedef struct OggVorbis_File { void* datasource; int seekable; int links; } OggVorbis_File;

static int g_openResult, g_seekable, g_links, g_channels[4], g_infoCalls, g_commentCalls;
static char* g_comments[3];
static vorbis_comment g_vc;
static vorbis_info g_vi[4], g_viAll;
static void* g_openSource;

int ov_open(void* datasource, OggVorbis_File* vf, char* initial, long ibytes)
{
    g_openSource = datasource;
    if (initial != 0 || ibytes != 0) return -99;
    vf->links = g_links;
    vf->seekable = g_seekable;
    return g_openResult;
}
long ov_seekable(OggVorbis_File* vf) { return vf->seekable; }
vorbis_comment* ov_comment(OggVorbis_File*, int link) { ++g_commentCalls; return link == -1 ? &g_vc : 0; }
vorbis_info* ov_info(OggVorbis_File*, int link)
{
    ++g_infoCalls;
    if (link < 0) return &g_viAll;
    g_vi[link].channels = g_channels[link];
    return &g_vi[link];
}

struct SStreamSegment { unsigned long m_word0, m_word4, m_word8; };
static int g_readerBuilt;
class CStreamReader {
public:
    CStreamReader(void* source, const SStreamSegment& segment) { m_source = source; m_size = segment.m_word4; ++g_readerBuilt; }
    ~CStreamReader() {}
    void* m_source;
    unsigned long m_size;
};

class IAudioStream {
public:
    virtual ~IAudioStream() {}
};

class COggVorbisStream : public IAudioStream {
public:
    COggVorbisStream(void* source, const SStreamSegment& segment);
    CStreamReader m_reader;
    long m_valid;
    long m_opened;
    OggVorbis_File m_vf;
};

COggVorbisStream::COggVorbisStream(void* source, const SStreamSegment& segment)
    : m_reader(source, segment)
{
    m_valid = 0;
    m_opened = 1;
    if (ov_open(&m_reader, &m_vf, 0, 0) < 0)
        return;
    m_opened = 1;
    if (!ov_seekable(&m_vf))
        return;
    vorbis_comment* vc = ov_comment(&m_vf, -1);
    char** comment = vc->user_comments;
    vorbis_info* vi = ov_info(&m_vf, -1);
    for (; *comment; ++comment) {
    }
    int i;
    for (i = 0; i < m_vf.links; i++) {
        vi = ov_info(&m_vf, i);
        if (vi->channels != 2)
            return;
    }
    m_valid = 1;
}

static void reset(int open, int seek, int links, int c0, int c1)
{
    g_openResult = open; g_seekable = seek; g_links = links; g_channels[0] = c0; g_channels[1] = c1;
    g_infoCalls = 0; g_commentCalls = 0;
}

int main()
{
    static char a[] = "TITLE=x", b[] = "ARTIST=y";
    g_comments[0] = a; g_comments[1] = b; g_comments[2] = 0;
    g_vc.user_comments = g_comments;
    int src = 0;
    SStreamSegment seg = { 1, 2, 3 };
    reset(0, 1, 2, 2, 2);
    COggVorbisStream s1(&src, seg);
    int ok = s1.m_valid == 1 && s1.m_opened == 1 && g_openSource == (void*)&s1.m_reader && s1.m_reader.m_size == 2
        && g_infoCalls == 3 && g_commentCalls == 1;
    reset(0, 1, 2, 2, 1);
    COggVorbisStream s2(&src, seg);
    ok = ok && s2.m_valid == 0 && s2.m_opened == 1 && g_infoCalls == 3;
    reset(-1, 1, 2, 2, 2);
    COggVorbisStream s3(&src, seg);
    ok = ok && s3.m_valid == 0 && s3.m_opened == 1 && g_infoCalls == 0 && g_commentCalls == 0;
    reset(0, 0, 2, 2, 2);
    COggVorbisStream s4(&src, seg);
    ok = ok && s4.m_valid == 0 && g_commentCalls == 0;
    reset(0, 1, 0, 1, 1);
    COggVorbisStream s5(&src, seg);
    ok = ok && s5.m_valid == 1 && g_infoCalls == 1 && g_readerBuilt == 5;
    if (!ok) { std::printf("00c88780_TEST FAIL\n"); return 1; }
    std::printf("00c88780_TEST PASS\n");
    return 0;
}