// The program binary cache (gl_renderer.cpp includes this inside its namespace, after Program).
//
// A new shader pair used to cost the game thread a queue round trip plus the Mali compiler on
// the A53 (the shaders' compile, the link and ~145 uniform queries): ~40-50 ms of game-thread
// GL time per new program in the v25 device log, which grew from 4 to 117 programs over a
// session, first visits to areas being 100-400 ms frames. Program binaries make that a load:
//
//  - a program linked from source is read back (glGetProgramBinary) and appended to one file
//    in the save folder (FABLE_RECOMP_DATA/gles_programs.bin) by the cache's own thread;
//  - that thread reads the file at start (never the game thread) and publishes the table; a
//    draw before that compiles as before (its record is not stored twice). The programs used
//    in the last three sessions (the 256 most recent at most) are then created from their
//    binaries on the render thread a slice per frame (at most ~2 ms of its time,
//    FABLE_GLES_PROGRAM_WARM_MS), most recently used first, so a draw that needs one takes it
//    with no round trip at all. Any other stored program, or one whose turn has not come, is
//    loaded from its binary (read from the file) in the draw's one round trip; another
//    ProgramKey with the same GLSL loads the binary read back from the first program;
//  - entries are keyed by SHA-256 (first 128 bits) of the translated vertex and fragment GLSL
//    (attribute bindings are layout qualifiers in that text); the file header holds the driver
//    (GL_VENDOR | GL_RENDERER | GL_VERSION | GL_SHADING_LANGUAGE_VERSION): another driver
//    starts the file over;
//  - every record carries a checksum; a damaged or truncated file is cut back to its last good
//    record, a binary the driver refuses (link status after glProgramBinary) is compiled again;
//    each session's first use of an entry is recorded, and beyond 1024 programs the least
//    recently used are dropped when the file is next read (rewritten after the table is
//    published, from the records of the file as read).
//
// FABLE_GLES_PROGRAM_CACHE=0 turns it off (=/abs/path moves the file); FABLE_GLES_PROGRAM_WARM=0
// creates each program only when a draw first needs it.
namespace progcache {

constexpr uint32_t kFileMagic = 0x43504746u;   // "FGPC"
constexpr uint32_t kFileVersion = 2;
constexpr uint32_t kEntryMagic = 0x544E4550u;  // "PENT": magic, format, vsLen, fsLen, binLen, session, key (16), binary, pad, checksum
constexpr uint32_t kTouchMagic = 0x48435554u;  // "TUCH": magic, session, key (16), checksum
constexpr size_t kEntryHeader = 40;
constexpr size_t kTouchBytes = 32;
constexpr size_t kMaxBinary = 8u << 20;        // a program's binary (Mali: tens of KB)
constexpr size_t kMaxFile = 48u << 20;         // read no further (the rest is cut)
constexpr size_t kKeep = 1024;                 // programs kept: beyond kKeep + kKeep / 8 the least recently used are dropped
constexpr size_t kWarmMax = 256;               // pre-linked after start: the most recently used,
constexpr uint32_t kWarmSessions = 3;          // of those used in the last three sessions

PFNGLPROGRAMBINARYPROC programBinary = nullptr;  // driver entry points (render thread only:
PFNGLGETPROGRAMBINARYPROC getProgramBinary = nullptr;  // not in the queue's wrapper set)
PFNGLPROGRAMPARAMETERIPROC programParameteri = nullptr;

// Runs `fn` on the render thread and waits (runOnRenderThread, usable before Device).
template <class F> void sync(F&& fn) {
    if (gq::direct()) { fn(); return; }
    gq::Sync done;
    gq::call([&fn, &done] { fn(); gq::signal(done); });
    gq::wait(done);
}

// ---- SHA-256 (the key) ----
struct Sha256 {
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    uint8_t buf[64];
    size_t used = 0;
    uint64_t total = 0;
    static uint32_t ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
    void block(const uint8_t* p) {
        static const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be,
            0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
            0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
            0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
            0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) w[i] = (uint32_t(p[4 * i]) << 24) | (uint32_t(p[4 * i + 1]) << 16) | (uint32_t(p[4 * i + 2]) << 8) | p[4 * i + 3];
        for (int i = 16; i < 64; ++i) {
            const uint32_t s0 = ror(w[i - 15], 7) ^ ror(w[i - 15], 18) ^ (w[i - 15] >> 3), s1 = ror(w[i - 2], 17) ^ ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; ++i) {
            const uint32_t t1 = hh + (ror(e, 6) ^ ror(e, 11) ^ ror(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
            const uint32_t t2 = (ror(a, 2) ^ ror(a, 13) ^ ror(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            hh = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
        }
        h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
    }
    void update(const void* data, size_t n) {
        const uint8_t* p = static_cast<const uint8_t*>(data);
        total += n;
        if (used) {
            const size_t take = std::min(n, 64 - used);
            std::memcpy(buf + used, p, take);
            used += take, p += take, n -= take;
            if (used < 64) return;
            block(buf), used = 0;
        }
        for (; n >= 64; p += 64, n -= 64) block(p);
        std::memcpy(buf, p, n);
        used = n;
    }
    void finish(uint8_t out[32]) {
        const uint64_t bits = total * 8;
        uint8_t pad[72] = {0x80};
        const size_t padLen = (used < 56 ? 56 : 120) - used;
        update(pad, padLen);
        uint8_t len[8];
        for (int i = 0; i < 8; ++i) len[i] = static_cast<uint8_t>(bits >> (56 - 8 * i));
        update(len, 8);
        for (int i = 0; i < 8; ++i) out[4 * i] = h[i] >> 24, out[4 * i + 1] = h[i] >> 16, out[4 * i + 2] = h[i] >> 8, out[4 * i + 3] = h[i];
    }
};

struct Key {
    uint64_t a = 0, b = 0;
    bool operator==(const Key&) const = default;
};
struct KeyHash { size_t operator()(const Key& k) const { return static_cast<size_t>(k.a ^ (k.b >> 7)); } };
Key keyOf(const std::string& vs, const std::string& fs) {
    static const char tag[] = "fable gles program v1";  // a new meaning of the same text (e.g. how the renderer links) changes this
    Sha256 s;
    s.update(tag, sizeof tag);
    const uint64_t lens[2] = {vs.size(), fs.size()};
    s.update(lens, sizeof lens);
    s.update(vs.data(), vs.size());
    s.update(fs.data(), fs.size());
    uint8_t d[32];
    s.finish(d);
    Key k;
    std::memcpy(&k.a, d, 8);
    std::memcpy(&k.b, d + 8, 8);
    return k;
}

// ---- GL side (render thread) ----
// The uniform locations a draw needs, and the samplers' units (the program is left bound).
void queryLocations(Program& p) {
    p.uC = glGetUniformLocation(p.gl, "c");
    p.uColorBgra = glGetUniformLocation(p.gl, "uColorBgra");
    p.uPc = glGetUniformLocation(p.gl, "pc");
    p.uBump = glGetUniformLocation(p.gl, "uBump");
    p.uProjMask = glGetUniformLocation(p.gl, "uProjMask");
    p.uFogColor = glGetUniformLocation(p.gl, "uFogColor");
    p.uFogEnable = glGetUniformLocation(p.gl, "uFogEnable");
    p.uAlphaFunc = glGetUniformLocation(p.gl, "uAlphaFunc");
    p.uAlphaRef = glGetUniformLocation(p.gl, "uAlphaRef");
    p.uViewport = glGetUniformLocation(p.gl, "uViewport");
    p.uTFactor = glGetUniformLocation(p.gl, "uTFactor");
    p.uHalfPixel = glGetUniformLocation(p.gl, "uHalfPixel");
    glUseProgram(p.gl);
    for (int i = 0; i < 6; ++i) {
        const std::string n = "s" + std::to_string(i);
        p.uS[i] = glGetUniformLocation(p.gl, n.c_str());
        if (p.uS[i] >= 0) glUniform1i(p.uS[i], i);
    }
    for (int k = 0; k < 128; ++k) p.uCElem[k] = p.uC >= 0 ? glGetUniformLocation(p.gl, ("c[" + std::to_string(k) + "]").c_str()) : -1;
    for (int k = 0; k < 8; ++k) p.uPcElem[k] = p.uPc >= 0 ? glGetUniformLocation(p.gl, ("pc[" + std::to_string(k) + "]").c_str()) : -1;
}
// A program from a binary; 0 when the driver refuses it (another driver build, damage).
GLuint fromBinary(GLenum format, const std::vector<uint8_t>& bin) {
    if (!programBinary || bin.empty()) return 0;
    const GLuint p = glCreateProgram();
    if (programParameteri) programParameteri(p, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);  // another key's draw may read it back
    programBinary(p, format, bin.data(), static_cast<GLsizei>(bin.size()));
    GLint ok = 0;  // a binary the driver cannot load leaves the program unlinked (also on a bad format)
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) { glDeleteProgram(p); return 0; }
    return p;
}
// Links `v` + `f` (as Device::linkProgram) with the hint that its binary will be read back.
GLuint link(GLuint v, GLuint f, bool retrievable) {
    if (!v || !f) return 0;
    const GLuint p = glCreateProgram();
    if (retrievable && programParameteri) programParameteri(p, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[2048] = {};
        glGetProgramInfoLog(p, sizeof buf, nullptr, buf);
        log("gles: program link failed:\n%s", buf);
        glDeleteProgram(p);
        return 0;
    }
    return p;
}
bool readBinary(GLuint p, GLenum& format, std::vector<uint8_t>& out) {
    out.clear();
    if (!getProgramBinary || !p) return false;
    GLint len = 0;
    glGetProgramiv(p, GL_PROGRAM_BINARY_LENGTH, &len);
    if (len <= 0 || static_cast<size_t>(len) > kMaxBinary) return false;
    out.resize(static_cast<size_t>(len));
    GLsizei got = 0;
    format = 0;
    getProgramBinary(p, len, &got, &format, out.data());
    if (got <= 0 || got > len) { out.clear(); return false; }
    out.resize(static_cast<size_t>(got));
    return true;
}

// ---- entries ----
enum State { kIdle, kQueued, kReady, kRefused, kClaimed };  // kQueued: in a warm-up slice; kClaimed: built by a draw
struct Entry {
    Key key;
    uint32_t vsLen = 0, fsLen = 0, format = 0, session = 0;  // session: the last one that used it (file)
    uint64_t offset = 0;                // its record in the file as read (0: stored this session)
    uint32_t binLen = 0;
    std::vector<uint8_t> bin;           // the warm set's, read with the table; freed once a program exists
    std::atomic<int> state{kIdle};
    gq::Sync ready;                     // signalled when its slice handled it
    std::unique_ptr<Program> prog;      // the slice's program (render thread, before `state`), until taken
    bool taken = false;                 // game thread: a ProgramKey has the program built from this entry
    bool touched = false;               // game thread: its use this session is on file
    GLuint donor = 0;                   // game thread: a live program from these sources (binary for another key)
};

inline size_t pad8(size_t n) { return (n + 7) & ~static_cast<size_t>(7); }
inline void put32(std::vector<uint8_t>& v, uint32_t x) { const size_t at = v.size(); v.resize(at + 4); std::memcpy(&v[at], &x, 4); }
inline void put64(std::vector<uint8_t>& v, uint64_t x) { const size_t at = v.size(); v.resize(at + 8); std::memcpy(&v[at], &x, 8); }
inline uint32_t get32(const uint8_t* p) { uint32_t x; std::memcpy(&x, p, 4); return x; }
inline uint64_t get64(const uint8_t* p) { uint64_t x; std::memcpy(&x, p, 8); return x; }
inline size_t entryBytes(size_t binLen) { return pad8(kEntryHeader + binLen) + 8; }

std::vector<uint8_t> fileHeader(const std::string& driver) {
    std::vector<uint8_t> v;
    put32(v, kFileMagic), put32(v, kFileVersion), put32(v, static_cast<uint32_t>(driver.size())), put32(v, 0);
    v.insert(v.end(), driver.begin(), driver.end());
    v.resize(pad8(v.size()), 0);
    return v;
}
void entryRecord(std::vector<uint8_t>& v, const Key& k, uint32_t vsLen, uint32_t fsLen, uint32_t format, uint32_t session, const std::vector<uint8_t>& bin) {
    v.clear();
    put32(v, kEntryMagic), put32(v, format), put32(v, vsLen), put32(v, fsLen), put32(v, static_cast<uint32_t>(bin.size())), put32(v, session);
    put64(v, k.a), put64(v, k.b);
    v.insert(v.end(), bin.begin(), bin.end());
    v.resize(pad8(v.size()), 0);
    put64(v, hashBytes(v.data(), v.size()));
}
void touchRecord(std::vector<uint8_t>& v, const Key& k, uint32_t session) {
    v.clear();
    put32(v, kTouchMagic), put32(v, session), put64(v, k.a), put64(v, k.b);
    put64(v, hashBytes(v.data(), v.size()));
}
// The entry record of `binLen` bytes of binary at `offset` of `fd`, whole and checked (magic,
// length, checksum); any thread (pread).
bool readEntry(int fd, uint64_t offset, size_t binLen, std::vector<uint8_t>& rec) {
    const size_t bytes = entryBytes(binLen);
    rec.resize(bytes);
    return fd >= 0 && pread(fd, rec.data(), bytes, static_cast<off_t>(offset)) == static_cast<ssize_t>(bytes) && get32(rec.data()) == kEntryMagic &&
           get32(rec.data() + 16) == binLen && hashBytes(rec.data(), bytes - 8) == get64(rec.data() + bytes - 8);
}

// ---- the cache's thread: reads the file first, then appends what the game sends -------------
// A record is serialized there, stamped with this session; an SD card write never stalls a frame.
// It lives until the process exits (a write cut short leaves a torn last record: cut at the
// next read).
struct Loaded {
    std::vector<std::unique_ptr<Entry>> entries;  // kept, most recently used first (the warm set first)
    uint32_t session = 1;
    size_t fileBytes = 0, readBytes = 0, warmBytes = 0;
    unsigned warm = 0;
    int fd = -1;
    const char* note = "";
    double ms = 0;
};
struct Writer {
    std::mutex m;
    std::condition_variable cv;
    struct Job { bool touch, replace; Key key; uint32_t vsLen, fsLen, format; std::vector<uint8_t> bin; };
    std::deque<Job> jobs;
    std::string path, driver;
    bool warm = true;
    int fd = -1;  // the file as read, kept open: draws load binaries from it, a rewrite copies from it
    struct Kept { uint64_t offset; uint32_t binLen, session; };
    std::vector<Kept> kept;  // the rewrite's records, in order
    bool rewriteWanted = false;
    std::unordered_map<Key, bool, KeyHash> onFile;  // keys with an entry record in the file (as rewritten / appended)
    std::atomic<bool> loadedReady{false};
    Loaded loaded;  // the reader's result (under m), taken once by the game thread
    void push(Job j) {
        { std::lock_guard<std::mutex> l(m); jobs.push_back(std::move(j)); }
        cv.notify_one();
    }
    Loaded read();
    void rewrite(const std::vector<uint8_t>& header);
    void run() {
        Loaded r = read();
        const uint32_t session = r.session;
        { std::lock_guard<std::mutex> l(m); loaded = std::move(r); }
        loadedReady.store(true, std::memory_order_release);  // the table first: lookups hit while the file is rewritten
        if (rewriteWanted) rewrite(fileHeader(driver));
        FILE* f = nullptr;
        bool warned = false;
        std::vector<uint8_t> rec;
        for (;;) {
            Job j;
            {
                std::unique_lock<std::mutex> l(m);
                cv.wait(l, [&] { return !jobs.empty(); });
                j = std::move(jobs.front());
                jobs.pop_front();
            }
            // linked before the table was published, but on file already: its use is recorded
            // instead (one entry record per key; a replacement for a refused binary is stored)
            if (!j.touch && !j.replace && onFile.count(j.key)) j.touch = true;
            if (j.touch) touchRecord(rec, j.key, session);
            else entryRecord(rec, j.key, j.vsLen, j.fsLen, j.format, session, j.bin), onFile[j.key] = true;
            if (!f) f = std::fopen(path.c_str(), "ab");
            const bool ok = f && std::fwrite(rec.data(), 1, rec.size(), f) == rec.size() && std::fflush(f) == 0;
            if (!ok && !warned) warned = true, log("gles: program cache: cannot write %s (errno %d): programs are not kept for the next run", path.c_str(), errno);
        }
    }
};

// A new file next to the old one, then renamed over it: the kept records copied one at a time
// from the file as read (still open), each stamped with its last session.
void Writer::rewrite(const std::vector<uint8_t>& header) {
    const uint64_t t0 = nowNs();
    const std::string tmp = path + ".tmp";
    FILE* t = std::fopen(tmp.c_str(), "wb");
    bool ok = t && std::fwrite(header.data(), 1, header.size(), t) == header.size();
    size_t bytes = header.size(), n = 0;
    std::vector<uint8_t> rec;
    for (size_t i = 0; ok && i < kept.size(); ++i) {
        if (!readEntry(fd, kept[i].offset, kept[i].binLen, rec)) continue;  // changed under us: left out
        std::memcpy(rec.data() + 20, &kept[i].session, 4);
        const uint64_t sum = hashBytes(rec.data(), rec.size() - 8);
        std::memcpy(rec.data() + rec.size() - 8, &sum, 8);
        ok = std::fwrite(rec.data(), 1, rec.size(), t) == rec.size();
        bytes += rec.size(), ++n;
    }
    if (t && std::fclose(t) != 0) ok = false;
    if (ok) ok = std::rename(tmp.c_str(), path.c_str()) == 0;
    else ::unlink(tmp.c_str());
    if (!ok) log("gles: program cache: cannot write %s (errno %d)", path.c_str(), errno);
    else if (n) log("gles: program cache: %s rewritten on the cache thread: %zu programs (%zu KB) in %.0f ms", path.c_str(), n, bytes >> 10, (nowNs() - t0) / 1e6);
    std::vector<Kept>().swap(kept);
}

Loaded Writer::read() {
    Loaded r;
    const uint64_t t0 = nowNs();
    const std::vector<uint8_t> header = fileHeader(driver);
    std::vector<std::unique_ptr<Entry>> all;  // file order
    std::unordered_map<Key, size_t, KeyHash> at;  // key -> index in `all` of its latest record
    size_t dupes = 0, touches = 0;
    bool damaged = false, fresh = false;
    fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    std::vector<uint8_t> h(header.size());
    if (fd < 0) fresh = true, r.note = " (new file)";
    else if (pread(fd, h.data(), h.size(), 0) != static_cast<ssize_t>(h.size()) || std::memcmp(h.data(), header.data(), 8) != 0) fresh = true, r.note = " (unknown format: started over)";
    else if (h != header) fresh = true, r.note = " (another driver: started over)";
    uint64_t pos = header.size();
    std::vector<uint8_t> rec;
    while (!fresh) {
        uint8_t head[kTouchBytes];  // a use record whole, or the start of an entry record
        const ssize_t got = pread(fd, head, kTouchBytes, static_cast<off_t>(pos));
        if (got == 0) break;  // a clean end
        if (got != static_cast<ssize_t>(kTouchBytes) || pos + kTouchBytes > kMaxFile) { damaged = true; break; }
        const uint32_t magic = get32(head);
        if (magic == kTouchMagic) {
            if (hashBytes(head, kTouchBytes - 8) != get64(head + kTouchBytes - 8)) { damaged = true; break; }
            const Key k{get64(head + 8), get64(head + 16)};
            if (auto it = at.find(k); it != at.end()) all[it->second]->session = std::max(all[it->second]->session, get32(head + 4));
            ++touches, pos += kTouchBytes;
            continue;
        }
        const size_t binLen = get32(head + 16);
        if (magic != kEntryMagic || binLen == 0 || binLen > kMaxBinary || pos + entryBytes(binLen) > kMaxFile || !readEntry(fd, pos, binLen, rec)) { damaged = true; break; }
        auto e = std::make_unique<Entry>();
        e->format = get32(rec.data() + 4), e->vsLen = get32(rec.data() + 8), e->fsLen = get32(rec.data() + 12), e->session = get32(rec.data() + 20);
        e->key = Key{get64(rec.data() + 24), get64(rec.data() + 32)};
        e->offset = pos, e->binLen = static_cast<uint32_t>(binLen);
        if (auto it = at.find(e->key); it != at.end()) {  // stored again (the driver refused the older one): the later wins
            e->session = std::max(e->session, all[it->second]->session);
            all[it->second].reset();
            it->second = all.size();
            ++dupes;
        } else {
            at.emplace(e->key, all.size());
        }
        all.push_back(std::move(e));
        pos += entryBytes(binLen);
    }
    r.readBytes = pos;
    if (damaged) r.note = " (damaged after the last good record: cut)";
    // most recently used first, file order within a session; beyond kKeep + kKeep / 8 the
    // least recently used are dropped (down to kKeep)
    std::vector<std::unique_ptr<Entry>> keep;
    for (auto& e : all) {
        if (!e) continue;
        r.session = std::max(r.session, e->session + 1);
        keep.push_back(std::move(e));
    }
    std::stable_sort(keep.begin(), keep.end(), [](const auto& a, const auto& b) { return a->session > b->session; });
    const size_t dropped = keep.size() > kKeep + kKeep / 8 ? keep.size() - kKeep : 0;
    if (dropped) keep.resize(kKeep);
    // the warm set: their binaries now (the others are read from the file when a draw needs them)
    for (size_t i = 0; warm && i < keep.size() && i < kWarmMax && keep[i]->session + kWarmSessions >= r.session; ++i) {
        Entry& e = *keep[i];
        if (!readEntry(fd, e.offset, e.binLen, rec)) break;
        e.bin.assign(rec.begin() + static_cast<std::ptrdiff_t>(kEntryHeader), rec.begin() + static_cast<std::ptrdiff_t>(kEntryHeader + e.binLen));
        r.warmBytes += e.binLen, ++r.warm;
    }
    // Rewritten (after the table is published) when it is new or damaged, holds duplicates or
    // dropped entries, or the use records have piled up; else appended to as it is.
    rewriteWanted = fresh || damaged || dupes || dropped || touches > std::max<size_t>(64, keep.size() / 2);
    if (rewriteWanted && !fresh && !damaged && (dupes || dropped)) r.note = " (duplicates / least recently used dropped)";
    r.fileBytes = rewriteWanted ? header.size() : pos;
    for (const auto& e : keep) {
        onFile[e->key] = true;
        if (rewriteWanted) kept.push_back({e->offset, e->binLen, e->session}), r.fileBytes += entryBytes(e->binLen);
    }
    r.fd = fd;
    r.entries = std::move(keep);
    r.ms = (nowNs() - t0) / 1e6;
    return r;
}

struct Cache {
    bool on = false, started = false, adopted = false, warmOn = true, warmLogged = false;
    std::string path;
    std::vector<std::unique_ptr<Entry>> entries;  // pointers stay valid for the session
    std::unordered_map<Key, Entry*, KeyHash> byKey;
    std::vector<Entry*> warmOrder;
    size_t warmNext = 0;
    uint32_t session = 1;
    Writer* writer = nullptr;
    int fd = -1;  // the file as read (the writer's): stored binaries outside the warm set
    size_t fileBytes = 0;
    bool full = false;
    uint64_t sliceNs = 2000000;   // render-thread budget per frame for the warm-up
    unsigned sliced = 0, frames = 0;
    // statistics (game thread; the warm-up's from the render thread)
    unsigned loaded = 0, warmHits = 0, binaryHits = 0, misses = 0;
    double hitMs = 0, missMs = 0;
    uint64_t lastLogNs = 0;
    std::atomic<unsigned> warmed{0}, refused{0};
    std::atomic<uint64_t> warmNs{0}, perEntryNs{1500000};  // render thread: time spent, estimate per entry

    void start();
    // The reader's result, once it is ready (game thread; a lookup before that is a miss).
    void adopt() {
        if (adopted || !writer || !writer->loadedReady.load(std::memory_order_acquire)) return;
        adopted = true;
        Loaded r;
        { std::lock_guard<std::mutex> l(writer->m); r = std::move(writer->loaded); }
        session = r.session;
        fd = r.fd;
        fileBytes += r.fileBytes;  // plus what was appended meanwhile
        for (auto& e : r.entries) {
            if (byKey.count(e->key)) continue;  // linked this session before the table was ready
            byKey.emplace(e->key, e.get());
            if (warmOn && !e->bin.empty()) warmOrder.push_back(e.get());
            entries.push_back(std::move(e));
            ++loaded;
        }
        log("gles: program cache: %s: %u programs (%zu KB) read in %.1f ms on the cache thread%s, session %u; %zu (%zu KB) to pre-link on the render thread a slice per frame, the rest loaded from the file when first drawn",
            path.c_str(), loaded, r.readBytes >> 10, r.ms, r.note, session, warmOrder.size(), r.warmBytes >> 10);
    }
    Entry* find(const Key& k, size_t vsLen, size_t fsLen) {
        if (!on) return nullptr;
        adopt();
        auto it = byKey.find(k);
        return it != byKey.end() && it->second->vsLen == vsLen && it->second->fsLen == fsLen ? it->second : nullptr;
    }
    void touch(Entry* e) {  // its first use this session goes on file (least recently used are dropped first)
        if (e->touched) return;
        e->touched = true;
        if (e->session < session && !full) writer->push({true, false, e->key, 0, 0, 0, {}}), fileBytes += kTouchBytes;
    }
    // The program a warm-up slice made from `e`, for a draw (game thread): no round trip. Waits
    // only if `e` sits in a slice the render thread has not reached yet.
    bool takeReady(Entry* e, Program& out) {
        if (!e || e->taken) return false;
        int s = e->state.load(std::memory_order_acquire);
        if (s == kQueued) gq::wait(e->ready), s = e->state.load(std::memory_order_acquire);
        if (s != kReady || !e->prog) return false;
        e->taken = true;
        out = *e->prog;
        e->prog.reset();
        e->donor = out.gl;
        touch(e);
        return true;
    }
    // What a draw's build can load instead of compiling: `from` an entry no slice has taken
    // (claimed: the warm-up skips it), or the binary of a program already made from the same
    // GLSL (`donor`).
    void source(Entry* e, const Entry*& from, GLuint& donor) {
        from = nullptr, donor = 0;
        if (!e) return;
        const int s = e->state.load(std::memory_order_acquire);
        if (s == kIdle && (!e->bin.empty() || e->offset)) {
            e->state.store(kClaimed, std::memory_order_relaxed);
            from = e;
        } else if (s != kRefused) {
            donor = e->donor;
        }
    }
    // Render thread, in the draw's build: the program from `from`'s binary (in memory, else read
    // from the file) or from the donor's; 0 when there is none or the driver refuses it.
    GLuint load(const Entry* from, GLuint donor) const {
        std::vector<uint8_t> b;
        GLenum f = 0;
        if (from) {
            if (!from->bin.empty()) return fromBinary(from->format, from->bin);
            if (!readEntry(fd, from->offset, from->binLen, b)) return 0;
            b.erase(b.begin() + static_cast<std::ptrdiff_t>(kEntryHeader + from->binLen), b.end());
            b.erase(b.begin(), b.begin() + static_cast<std::ptrdiff_t>(kEntryHeader));
            return fromBinary(from->format, b);
        }
        return donor && readBinary(donor, f, b) ? fromBinary(f, b) : 0;
    }
    // After the build: `claimed` the entry's binary was tried (`loaded`: the driver took it).
    void built(Entry* e, bool claimed, bool loaded, GLuint gl) {
        if (!e) return;
        if (claimed) {
            std::vector<uint8_t>().swap(e->bin);
            if (!loaded) e->state.store(kRefused, std::memory_order_relaxed);
            else e->taken = true;
        }
        if (gl && !e->donor) e->donor = gl;
        if (loaded) touch(e);
    }
    // A program linked from source this session (its binary goes to the file, not kept here).
    void add(Entry* old, const Key& k, size_t vsLen, size_t fsLen, GLenum format, std::vector<uint8_t> bin, GLuint gl) {
        if (!on || bin.empty()) return;
        if (old && old->state.load(std::memory_order_acquire) != kRefused) { if (!old->donor) old->donor = gl; return; }  // on file already
        const size_t bytes = entryBytes(bin.size());
        if (full || fileBytes + bytes > kMaxFile || byKey.size() >= kKeep + kKeep / 2) {
            if (!full) full = true, log("gles: program cache: %s is full (%zu programs, %zu KB): new programs are not stored", path.c_str(), byKey.size(), fileBytes >> 10);
            return;
        }
        auto e = std::make_unique<Entry>();
        e->key = k, e->vsLen = static_cast<uint32_t>(vsLen), e->fsLen = static_cast<uint32_t>(fsLen), e->format = format;
        e->state.store(kClaimed, std::memory_order_relaxed);
        e->taken = e->touched = true, e->donor = gl, e->session = session;
        byKey[k] = e.get();
        entries.push_back(std::move(e));
        fileBytes += bytes;
        // old: its binary refused (another driver build): this one replaces it on file
        writer->push({false, old != nullptr, k, static_cast<uint32_t>(vsLen), static_cast<uint32_t>(fsLen), format, std::move(bin)});
    }
    // Once a frame (Present): the next warm-up slice, sized to the render-thread budget from the
    // measured time per program. `bound`: the program the device has bound at this point of the
    // queue (its lastProgramGl), bound again after the slice.
    void tick(GLuint bound) {
        if (!on) return;
        adopt();
        ++frames;
        if (warmNext >= warmOrder.size()) {
            if (sliced && !warmLogged && warmed.load() + refused.load() == sliced) {
                warmLogged = true;
                log("gles: program cache: warm-up done: %u programs created from their binaries in %.0f ms of render-thread time over %u frames (%u refused by the driver: compiled again on first use)",
                    warmed.load(), warmNs.load() / 1e6, frames, refused.load());
            }
            return;
        }
        const uint64_t per = std::max<uint64_t>(perEntryNs.load(std::memory_order_relaxed), 100000);
        const size_t n = std::clamp<size_t>(sliceNs / per, 1, 32);
        std::vector<Entry*> slice;
        while (warmNext < warmOrder.size() && slice.size() < n) {
            Entry* e = warmOrder[warmNext++];
            if (e->state.load(std::memory_order_relaxed) != kIdle) continue;  // a draw built it already
            e->state.store(kQueued, std::memory_order_relaxed);
            slice.push_back(e);
        }
        if (slice.empty()) return;
        sliced += static_cast<unsigned>(slice.size());
        gq::call([this, bound, slice = std::move(slice)] {
            const uint64_t t0 = nowNs();
            for (Entry* e : slice) {
                auto prog = std::make_unique<Program>();
                prog->gl = fromBinary(e->format, e->bin);
                if (prog->gl) queryLocations(*prog), e->prog = std::move(prog), warmed.fetch_add(1, std::memory_order_relaxed);
                else refused.fetch_add(1, std::memory_order_relaxed);
                std::vector<uint8_t>().swap(e->bin);
                e->state.store(e->prog ? kReady : kRefused, std::memory_order_release);
                gq::signal(e->ready);
            }
            glUseProgram(bound);  // queryLocations bound each one; the device skips a glUseProgram of the program it has bound
            const uint64_t d = nowNs() - t0;
            warmNs.fetch_add(d, std::memory_order_relaxed);
            perEntryNs.store(d / slice.size(), std::memory_order_relaxed);
        });
    }
    void note(bool hit, bool warm, double ms) {
        if (warm) ++warmHits; else if (hit) ++binaryHits; else ++misses;
        (hit || warm ? hitMs : missMs) += ms;
        const uint64_t now = nowNs();
        if (host::g_logLevel >= 1 && (now - lastLogNs > 10000000000ull || (!hit && !warm && misses == 1))) {
            lastLogNs = now;
            report();
        }
    }
    void report() {
        if (!on) return;
        const unsigned hits = warmHits + binaryHits;
        log("gles: program cache: %u hits (%u pre-linked, %u from binary, %.1f ms avg), %u misses (compiled, %.1f ms avg), %zu programs in %s (%zu KB), %u read, %u pre-linked, %u refused",
            hits, warmHits, binaryHits, hits ? hitMs / hits : 0.0, misses, misses ? missMs / misses : 0.0, byKey.size(), path.c_str(), fileBytes >> 10, loaded,
            warmed.load(), refused.load());
    }
};
Cache g_cache;

void Cache::start() {
    if (started) {  // a second device: another context, the programs made so far are not its own
        if (on) on = false, log("gles: program cache off for this device (a second device)");
        return;
    }
    started = true;
    const char* env = getenv("FABLE_GLES_PROGRAM_CACHE");
    if (env && std::strcmp(env, "0") == 0) { log("gles: program cache off (FABLE_GLES_PROGRAM_CACHE=0)"); return; }
    programBinary = reinterpret_cast<PFNGLPROGRAMBINARYPROC>(getProc("glProgramBinary"));
    getProgramBinary = reinterpret_cast<PFNGLGETPROGRAMBINARYPROC>(getProc("glGetProgramBinary"));
    programParameteri = reinterpret_cast<PFNGLPROGRAMPARAMETERIPROC>(getProc("glProgramParameteri"));
    if (!programBinary || !getProgramBinary) { log("gles: program cache off: the driver has no glProgramBinary"); return; }
    GLint formats = 0;
    std::string driver;
    sync([&] {
        glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
        for (GLenum s : {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_SHADING_LANGUAGE_VERSION}) {
            const char* t = reinterpret_cast<const char*>(glGetString(s));
            driver += t ? t : "?";
            driver += '|';
        }
    });
    if (formats <= 0) { log("gles: program cache off: the driver offers no program binary format"); return; }
    path = env && env[0] == '/' ? std::string(env) : w32::dataDir() + "/gles_programs.bin";
    const char* w = getenv("FABLE_GLES_PROGRAM_WARM");
    warmOn = !(w && std::strcmp(w, "0") == 0);
    if (const char* ms = getenv("FABLE_GLES_PROGRAM_WARM_MS")) sliceNs = static_cast<uint64_t>(std::max(0.1, atof(ms)) * 1e6);
    on = true;
    writer = new Writer;  // never freed: its thread runs until the process exits
    writer->path = path, writer->driver = driver, writer->warm = warmOn;
    std::thread([wr = writer] { wr->run(); }).detach();
    log("gles: program cache on: %s, %d binary format%s, reading it on the cache thread", path.c_str(), formats, formats == 1 ? "" : "s");
}

}  // namespace progcache
