// Behaviour test for 0x00AA7230 (C3DMeshFileXVertexDataChunk::GetChunkDescription, stand-in self C3DMeshFileXVertexDataChunk_Methods): links the row object (the method is
// declared here, not defined; the row's own same-TU CCharString(const char*, long) body is linked too) and asserts the text the row
// passes: that body stores text + 1 for a nonzero length, so Data - 1 is the literal, retail's "Vertex Buffer Data".
#include <cstdio>
#include <string.h>

class CCharString { public:
    char* Data;
    CCharString(const char* text, long length);   // defined in the row object
};

struct C3DMeshFileXVertexDataChunk_Methods {
    CCharString GetChunkDescription() const;   // definition linked from the row object
};

int main() {
    C3DMeshFileXVertexDataChunk_Methods c;
    CCharString r = c.GetChunkDescription();
    // len=-1 (nonzero) -> Data == text + 1
    if (r.Data != 0 && strcmp(r.Data - 1, "Vertex Buffer Data") == 0) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("FAIL %p\n", r.Data);
    return 1;
}
