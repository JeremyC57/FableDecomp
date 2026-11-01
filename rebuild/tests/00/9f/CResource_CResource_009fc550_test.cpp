#include <cstdio>

// Independent behaviour model of ~CResource @ 0x009fc550:
//   - write derived vtable at entry
//   - if the +8 member pointer is non-null, invoke its RemoveResource(self)
//   - write base vtable at end
// Verifies the conditional-cleanup gate and the final vtable state.

static int g_removed = 0;

struct CResource;
struct CResourceList {
    void RemoveResource(CResource* self);
};

struct CResource {
    unsigned int      vptr;
    int               pad4;
    CResourceList* ResourceList;
};

void CResourceList::RemoveResource(CResource* self) { g_removed++; (void)self; }

static const unsigned int kA = 0x012354a4u;
static const unsigned int kB = 0x012354d0u;

static void dtor(CResource* self) {
    self->vptr = kA;
    if (self->ResourceList != 0) {
        self->ResourceList->RemoveResource(self);
    }
    self->vptr = kB;
}

int main() {
    CResourceList owner;

    // member8 non-null -> RemoveResource runs once.
    CResource a; a.vptr = 0; a.pad4 = 0; a.ResourceList = &owner;
    dtor(&a);

    // member8 null -> RemoveResource skipped.
    CResource b; b.vptr = 0; b.pad4 = 0; b.ResourceList = 0;
    dtor(&b);

    if (g_removed == 1 && a.vptr == kB && b.vptr == kB) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL g=%d a=%x b=%x\n", g_removed, a.vptr, b.vptr);
    }
    return 0;
}