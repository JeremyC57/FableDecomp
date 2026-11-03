// Test for 0x00520670 CThingManager::LoadHeroStateBinary(CPersistContext&): a saved-script count, then per script a size;
// an empty script is skipped, a non-empty one is opened as a sub-stream whose own context gives the section count, the
// compressed and uncompressed sizes and the compressed block; the decompressed block holds level id, name, a section
// (whose first word is the entity size) and the entity, which is loaded through LoadSingleThing(entity_context, 1);
// three progress messages bracket the work and the trailing script text is transferred from the script context.
// Self-contained model (C++98; VC7.1 /Od and g++): little-endian 4-byte wire words on every host; the "compressor" is a
// byte copy recorded for inspection. The function body below is the reconstruction's text.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CCharString {
public:
    char Text[64];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { strncpy(Text, s, 63); Text[63] = 0; }
};

class CMemoryDataInputStream {
public:
    const unsigned char* Data; unsigned long Length; unsigned long Pos;
    CMemoryDataInputStream() : Data(0), Length(0), Pos(0) { }
    CMemoryDataInputStream(const void* p, unsigned long n) : Data((const unsigned char*)p), Length(n), Pos(0) { }
    void OpenMemorySource(const void* p, unsigned long n) { Data = (const unsigned char*)p; Length = n; Pos = 0; }
    unsigned long ReadULONG() {
        unsigned long v = (unsigned long)Data[Pos] | ((unsigned long)Data[Pos + 1] << 8) |
                          ((unsigned long)Data[Pos + 2] << 16) | ((unsigned long)Data[Pos + 3] << 24);
        Pos += 4; return v & 0xFFFFFFFFul;
    }
    long ReadSLONG() { unsigned long v = ReadULONG(); return v >= 0x80000000ul ? -(long)(0xFFFFFFFFul - v) - 1 : (long)v; }
    const unsigned char* ReadConstData(unsigned long n) { const unsigned char* p = Data + Pos; Pos += n; return p; }
    CCharString ReadNullTerminatedString() { CCharString s((const char*)(Data + Pos)); Pos += (unsigned long)strlen(s.Text) + 1; return s; }
};

class CPersistContext {
public:
    CMemoryDataInputStream* PLoadStream;
    CPersistContext(CMemoryDataInputStream& is, bool) : PLoadStream(&is) { }
    CMemoryDataInputStream* GetPLoadStream() { return PLoadStream; }
    void Transfer(long& v) { v = PLoadStream->ReadSLONG(); }
    void Transfer(unsigned long& v) { v = PLoadStream->ReadULONG(); }
    void Transfer(CCharString& v) { v = PLoadStream->ReadNullTerminatedString(); }
};

template<class T> class CArray {
public:
    T* Items;
    explicit CArray(unsigned int n) : Items((T*)calloc(n ? n : 1, sizeof(T))) { }
    ~CArray() { free(Items); }
    T* GetAsCArray() { return Items; }
};

static int g_decompress_calls = 0; static unsigned long g_decompress_in = 0;
class CCompressorZlib {
public:
    unsigned long InternalDecompress(const void* in, unsigned long in_size, void* out, unsigned long out_size) {
        ++g_decompress_calls; g_decompress_in = in_size;
        memcpy(out, in, in_size < out_size ? in_size : out_size);
        return in_size;
    }
};

class CWorldMap { };
class CWorld { public: CWorldMap Map; const CWorldMap& PeekWorldMap() const { return Map; } };
class CMainGameComponent { public: CWorld World; const CWorld& PeekWorld() const { return World; } };

namespace NProgressDisplay {
    static char g_log[256];
    void DisplayProgress(const CCharString& s, float, bool, bool) { strcat(g_log, s.Text + 29); }
}

class CThing;
static int g_loads = 0; static long g_load_flag = 0; static unsigned long g_entity_word = 0;
class CThingManager {
public:
    CMainGameComponent& Component;
    CThingManager(CMainGameComponent& c) : Component(c) { }
    CThing* LoadSingleThing(CPersistContext& context, long flag) {
        ++g_loads; g_load_flag = flag; g_entity_word = context.GetPLoadStream()->ReadULONG(); return 0;
    }
    void LoadHeroStateBinary(CPersistContext& context);
};

void CThingManager::LoadHeroStateBinary(CPersistContext& context) {
    const CWorldMap& world_map=Component.PeekWorld().PeekWorldMap();
    long no_scripts=0;
    context.Transfer(no_scripts);
    for(long c1=0;c1<no_scripts;++c1) {
        long script_size=0;
        context.Transfer(script_size);
        if(script_size>0) {
            NProgressDisplay::DisplayProgress("Loading entities from script 1",-1.0f,false,false);
            CMemoryDataInputStream script_is;
            script_is.OpenMemorySource(context.GetPLoadStream()->ReadConstData(script_size),script_size);
            CPersistContext script_context(script_is,true);
            unsigned long num_sections=0;
            script_context.Transfer(num_sections);
            unsigned long data_size=0;
            script_context.Transfer(data_size);
            unsigned long uncompressed_size=0;
            script_context.Transfer(uncompressed_size);
            CArray<unsigned char> uncompressed_data(uncompressed_size);
            const unsigned char* compressed_data=script_context.GetPLoadStream()->ReadConstData(data_size);
            CCompressorZlib compressor_zlib;
            unsigned long output_size=compressor_zlib.InternalDecompress(compressed_data,data_size,uncompressed_data.GetAsCArray(),uncompressed_size);
            CMemoryDataInputStream is(uncompressed_data.GetAsCArray(),uncompressed_size);
            unsigned long level_id=is.ReadULONG();
            CCharString name=is.ReadNullTerminatedString();
            unsigned long section_size=is.ReadULONG();
            CMemoryDataInputStream section_is(is.ReadConstData(section_size),section_size);
            unsigned long entity_size=section_is.ReadULONG();
            CMemoryDataInputStream entity_is(section_is.ReadConstData(entity_size),entity_size);
            CPersistContext entity_context(entity_is,true);
            NProgressDisplay::DisplayProgress("Loading entities from script 2",-1.0f,false,false);
            LoadSingleThing(entity_context,1);
            NProgressDisplay::DisplayProgress("Loading entities from script 3",-1.0f,false,false);
            CCharString script;
            script_context.Transfer(script);
        }
    }
}

static unsigned char g_buf[512]; static unsigned long g_len = 0;
static void put32(unsigned char* b, unsigned long& n, unsigned long v) {
    b[n] = (unsigned char)v; b[n + 1] = (unsigned char)(v >> 8); b[n + 2] = (unsigned char)(v >> 16); b[n + 3] = (unsigned char)(v >> 24); n += 4;
}
static void puts0(unsigned char* b, unsigned long& n, const char* s) { memcpy(b + n, s, strlen(s) + 1); n += (unsigned long)strlen(s) + 1; }

int main()
{
    // decompressed block: level id, name, section (entity size, entity)
    unsigned char block[64]; unsigned long bn = 0;
    put32(block, bn, 7); puts0(block, bn, "Hero"); put32(block, bn, 12); put32(block, bn, 8); put32(block, bn, 0xCAFEF00Dul); put32(block, bn, 0);
    // script sub-stream: sections, compressed size, uncompressed size, compressed bytes, script text
    unsigned char script[128]; unsigned long sn = 0;
    put32(script, sn, 1); put32(script, sn, bn); put32(script, sn, bn); memcpy(script + sn, block, bn); sn += bn; puts0(script, sn, "TheScript");
    put32(g_buf, g_len, 3);            // three saved scripts
    put32(g_buf, g_len, 0);            // empty: skipped
    put32(g_buf, g_len, sn); memcpy(g_buf + g_len, script, sn); g_len += sn;
    put32(g_buf, g_len, 0xFFFFFFFFul); // negative size: skipped
    put32(g_buf, g_len, 0x55555555ul); // next field, must stay unread
    CMemoryDataInputStream in(g_buf, g_len);
    CPersistContext context(in, true);
    CMainGameComponent component; CThingManager manager(component);
    manager.LoadHeroStateBinary(context);
    bool ok = g_loads == 1 && g_load_flag == 1 && g_entity_word == 0xCAFEF00Dul && g_decompress_calls == 1 &&
              g_decompress_in == bn && strcmp(NProgressDisplay::g_log, "123") == 0 && in.Pos == g_len - 4;
    if (ok) printf("00520670_TEST PASS\n");
    else printf("00520670_TEST FAIL loads=%d flag=%ld word=%lx dec=%d log=%s pos=%lu/%lu\n", g_loads, g_load_flag,
                g_entity_word, g_decompress_calls, NProgressDisplay::g_log, in.Pos, g_len);
    return ok ? 0 : 1;
}