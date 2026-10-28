#include <stdio.h>
#include <string.h>

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long len);
};

__declspec(noinline) CCharString::CCharString(const char* s, long len) {
    m_data = (char*)s;
    if (len == -2) m_data = 0;
}

class C3DMeshFileEngineMaterialChunk { public:
    void* vtbl;
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileEngineMaterialChunk::GetChunkDescription() const {
    return CCharString("Engine material chunk", -1);
}

int main() {
    C3DMeshFileEngineMaterialChunk c;
    c.vtbl = 0;
    CCharString r = c.GetChunkDescription();
    if (r.m_data && strcmp(r.m_data, "Engine material chunk") == 0) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("GETCHUNKDESC_FAIL\n");
    return 1;
}