class CCharString { public:
    void* copyCtor(char* src);   // default thiscall: ecx=this, src on stack
};

class CCachedFile { public:
    char pad[0x18];
    char m_pathname[4];
};

void* __fastcall GetPathname(CCachedFile* file, int /*edx*/, CCharString* dst)
{
    dst->copyCtor(file->m_pathname);
    return dst;
}