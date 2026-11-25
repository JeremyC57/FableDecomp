// Behaviour test for 0x00ABEC70 (C3DMeshFileVerticesChunk::GetChunkDescription, stand-in self C3DMeshFileVerticesChunk_Methods): links the row object (the method is
// declared here, not defined; the row's own same-TU CCharString(const char*, long) body is linked too) and asserts the text the row
// passes: that body stores text + 1 for a nonzero length, so Data - 1 is the literal, retail's "Vertex List".
#include <cstdio>
#include <string.h>

class CCharString { public:
    char* Data;
    CCharString(const char* text, long length);   // defined in the row object
};

struct C3DMeshFileVerticesChunk_Methods {
    CCharString GetChunkDescription() const;   // definition linked from the row object
};

int main() {
    C3DMeshFileVerticesChunk_Methods c;
    CCharString r = c.GetChunkDescription();
    // len=-1 (nonzero) -> Data == text + 1
    if (r.Data != 0 && strcmp(r.Data - 1, "Vertex List") == 0) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("FAIL %p\n", r.Data);
    return 1;
}
