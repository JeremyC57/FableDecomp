// Behaviour of CCountedPointer<T>::DeleteData for a non-polymorphic T (0x00812150): null-guarded `delete static_cast<T*>(data)`,
// i.e. the destructor then operator delete; a null pointer does nothing. Self-contained model (no row symbol linked).
#include <stdio.h>
#include <stdlib.h>

static int g_dtor = 0;
static int g_freed = 0;

struct SHitModel {
    int marker;
    ~SHitModel() { g_dtor++; }
    static void operator delete(void* p) { g_freed++; free(p); }
};

static void __fastcall DeleteDataModel(void* data)
{
    delete static_cast<SHitModel*>(data);
}

int main()
{
    DeleteDataModel(0);
    if (g_dtor != 0 || g_freed != 0) { printf("FAIL null\n"); return 1; }

    SHitModel* m = (SHitModel*)malloc(sizeof(SHitModel));
    m->marker = 7;
    DeleteDataModel(m);
    if (g_dtor != 1 || g_freed != 1) { printf("FAIL live\n"); return 1; }

    printf("OK_00812150\n");
    return 0;
}
