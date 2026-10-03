#include "format.hpp"

#include <cstdio>
#include <cwchar>

namespace host {

namespace {

// The host's string type for a character type (host::wstring is UTF-16 on every platform).
template <class C> using Str = std::conditional_t<std::is_same_v<C, wchar_t>, wstring, std::string>;

template <class C> size_t slen(const C* s) {
    size_t n = 0;
    while (s[n]) ++n;
    return n;
}

template <class C> Str<C> fromAscii(const char* s) {
    Str<C> r;
    for (; *s; ++s) r.push_back(static_cast<C>(static_cast<unsigned char>(*s)));
    return r;
}

// Converts a string of the other width for output.
std::string toOut(const wchar_t* s, size_t n, char*) {
    wstring w(s, n);
    return narrow(w.c_str());
}
wstring toOut(const char* s, size_t n, wchar_t*) {
    std::string a(s, n);
    return widen(a.c_str());
}
std::string toOut(const char* s, size_t n, char*) { return std::string(s, n); }
wstring toOut(const wchar_t* s, size_t n, wchar_t*) { return wstring(s, n); }

template <class C> Str<C> format(const C* f, uint32_t& ap) {
    Str<C> out;
    const bool wide = sizeof(C) == 2;
    while (*f) {
        if (*f != '%') { out.push_back(*f++); continue; }
        const C* start = f++;
        if (*f == '%') { out.push_back('%'); ++f; continue; }
        std::string flags;
        while (*f == '-' || *f == '+' || *f == ' ' || *f == '#' || *f == '0') flags.push_back(static_cast<char>(*f++));
        int width = -1, prec = -1;
        if (*f == '*') {
            width = static_cast<int32_t>(rd32(ap)); ap += 4; ++f;
            if (width < 0) { flags.push_back('-'); width = -width; }
        } else if (*f >= '0' && *f <= '9') {
            width = 0;
            while (*f >= '0' && *f <= '9') width = width * 10 + (*f++ - '0');
        }
        if (*f == '.') {
            ++f;
            prec = 0;
            if (*f == '*') { prec = static_cast<int32_t>(rd32(ap)); ap += 4; ++f; }
            else while (*f >= '0' && *f <= '9') prec = prec * 10 + (*f++ - '0');
        }
        enum { SZ_DEF, SZ_H, SZ_L, SZ_64, SZ_W } sz = SZ_DEF;
        for (;;) {
            if (*f == 'h') { sz = SZ_H; ++f; if (*f == 'h') ++f; }
            else if (*f == 'l') { ++f; if (*f == 'l') { sz = SZ_64; ++f; } else sz = SZ_L; }
            else if (*f == 'L' || *f == 'q') { sz = SZ_64; ++f; }
            else if (*f == 'w') { sz = SZ_W; ++f; }
            else if (*f == 'I') {
                if (f[1] == '6' && f[2] == '4') { sz = SZ_64; f += 3; }
                else if (f[1] == '3' && f[2] == '2') { f += 3; }
                else ++f;  // I = pointer-sized (32-bit guest)
            } else break;
        }
        const C conv = *f;
        if (!conv) break;
        ++f;
        std::string spec = "%" + flags;
        if (width >= 0) spec += std::to_string(width);
        if (prec >= 0) spec += "." + std::to_string(prec);
        char buf[512];
        switch (conv) {
        case 'd': case 'i': case 'o': case 'u': case 'x': case 'X': {
            if (sz == SZ_64) {
                const uint64_t v = rd64(ap); ap += 8;
                spec += "ll"; spec.push_back(static_cast<char>(conv));
                std::snprintf(buf, sizeof buf, spec.c_str(), (unsigned long long)v);
            } else {
                uint32_t v = rd32(ap); ap += 4;
                if (sz == SZ_H) v = (conv == 'd' || conv == 'i') ? static_cast<uint32_t>(static_cast<int16_t>(v)) : (v & 0xFFFF);
                spec.push_back(static_cast<char>(conv));
                std::snprintf(buf, sizeof buf, spec.c_str(), v);
            }
            out += fromAscii<C>(buf);
            break;
        }
        case 'p': {
            const uint32_t v = rd32(ap); ap += 4;
            std::snprintf(buf, sizeof buf, "%08X", v);
            out += fromAscii<C>(buf);
            break;
        }
        case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A': {
            const double v = rdf64(ap); ap += 8;
            spec.push_back(static_cast<char>(conv));
            std::snprintf(buf, sizeof buf, spec.c_str(), v);
            out += fromAscii<C>(buf);
            break;
        }
        case 'c': case 'C': {
            const uint32_t v = rd32(ap); ap += 4;
            bool argWide = wide;
            if (conv == 'C') argWide = !wide;
            if (sz == SZ_H) argWide = false;
            if (sz == SZ_L || sz == SZ_W) argWide = true;
            Str<C> s;
            if (argWide) { wchar_t w = static_cast<wchar_t>(v); s = toOut(&w, 1, (C*)nullptr); }
            else { char a = static_cast<char>(v); s = toOut(&a, 1, (C*)nullptr); }
            if (width > 0 && static_cast<int>(s.size()) < width) {
                if (flags.find('-') != std::string::npos) s.append(width - s.size(), ' ');
                else s.insert(0, width - s.size(), ' ');
            }
            out += s;
            break;
        }
        case 's': case 'S': case 'Z': {
            const uint32_t v = rd32(ap); ap += 4;
            bool argWide = wide;
            if (conv == 'S') argWide = !wide;
            if (sz == SZ_H) argWide = false;
            if (sz == SZ_L || sz == SZ_W) argWide = true;
            Str<C> s;
            if (!v) s = fromAscii<C>("(null)");
            else if (argWide) {
                const wchar_t* w = gp<wchar_t>(v);
                size_t n = prec >= 0 ? 0 : slen(w);
                if (prec >= 0) while (n < static_cast<size_t>(prec) && w[n]) ++n;
                s = toOut(w, n, (C*)nullptr);
            } else {
                const char* a = gp<char>(v);
                size_t n = prec >= 0 ? 0 : slen(a);
                if (prec >= 0) while (n < static_cast<size_t>(prec) && a[n]) ++n;
                s = toOut(a, n, (C*)nullptr);
            }
            if (width > 0 && static_cast<int>(s.size()) < width) {
                if (flags.find('-') != std::string::npos) s.append(width - s.size(), ' ');
                else s.insert(0, width - s.size(), ' ');
            }
            out += s;
            break;
        }
        case 'n': {
            const uint32_t v = rd32(ap); ap += 4;
            if (v) wr32(v, static_cast<uint32_t>(out.size()));
            break;
        }
        default:
            out.append(start, f);  // unknown: emit verbatim
            break;
        }
    }
    return out;
}

}  // namespace

std::string formatA(const char* fmt, uint32_t& ap) { return format<char>(fmt, ap); }
wstring formatW(const wchar_t* fmt, uint32_t& ap) { return format<wchar_t>(fmt, ap); }

int scanA(const char* input, const char* fmt, uint32_t ap) {
    void* p[24] = {};
    int n = 0;
    for (const char* f = fmt; *f && n < 24; ++f) {
        if (*f != '%') continue;
        ++f;
        if (*f == '%') continue;
        if (*f == '*') continue;
        p[n] = reinterpret_cast<void*>(static_cast<uintptr_t>(rd32(ap + 4u * n)));
        ++n;
        // skip to the conversion character
        while (*f && (*f == '[' ? false : !std::strchr("diouxXeEfgGaAcsSpn[", *f))) ++f;
        if (*f == '[') { ++f; if (*f == ']') ++f; while (*f && *f != ']') ++f; }
        if (!*f) break;
    }
    return std::sscanf(input, fmt, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13],
                       p[14], p[15], p[16], p[17], p[18], p[19], p[20], p[21], p[22], p[23]);
}

int copyTruncA(char* dst, uint32_t n, const std::string& s) {
    if (s.size() < n) { std::memcpy(dst, s.c_str(), s.size() + 1); return static_cast<int>(s.size()); }
    if (s.size() == n) { std::memcpy(dst, s.data(), n); return static_cast<int>(n); }
    std::memcpy(dst, s.data(), n);
    return -1;
}
int copyTruncW(wchar_t* dst, uint32_t n, const wstring& s) {
    if (s.size() < n) { std::memcpy(dst, s.c_str(), (s.size() + 1) * 2); return static_cast<int>(s.size()); }
    if (s.size() == n) { std::memcpy(dst, s.data(), n * 2); return static_cast<int>(n); }
    std::memcpy(dst, s.data(), n * 2);
    return -1;
}

}  // namespace host
