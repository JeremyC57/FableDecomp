#include <cstdio>
// Behaviour test for CManagedTextureData::Uninitialise (0x00A69DA0).
static int g_deleted = 0, g_uninit = 0; static void* g_uninitThis = (void*)1;
struct Info { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void Del(void*) { ++g_deleted; }
class CTexture { public: void Uninitialise() { ++g_uninit; g_uninitThis = this; } };
struct CP { CTexture* Data; Info* Info_; };
class CManagedTextureData { public:
    CP Texture; void* D3DHeader; void* Data; unsigned long AllocSize;
    void Uninitialise()
    {
        if (Texture.Info_) {
            --Texture.Info_->RefCount;
            if (Texture.Info_->RefCount == 0) { Texture.Info_->DeleteFunc(Texture.Info_->Data); delete Texture.Info_; }
        }
        Texture.Info_ = 0; Texture.Data = 0;
        D3DHeader = 0; Data = 0; AllocSize = 0;
        if (Texture.Data) Texture.Data->Uninitialise(); else { ++g_uninit; g_uninitThis = 0; }   // retail: null this
    }
};
int main()
{
    CTexture t; Info* i = new Info; i->RefCount = 1; i->DeleteFunc = Del; i->Data = &t;
    CManagedTextureData d = { { &t, i }, (void*)4, (void*)8, 16 };
    d.Uninitialise();
    bool ok = g_deleted == 1 && d.Texture.Data == 0 && d.Texture.Info_ == 0 && d.D3DHeader == 0 && d.Data == 0
           && d.AllocSize == 0 && g_uninit == 1 && g_uninitThis == 0;
    Info* j = new Info; j->RefCount = 2; j->DeleteFunc = Del; j->Data = &t;
    CManagedTextureData e = { { &t, j }, 0, 0, 0 };
    e.Uninitialise();
    ok = ok && g_deleted == 1 && j->RefCount == 1 && e.Texture.Info_ == 0;
    delete j;
    std::printf(ok ? "00a69da0_TEST PASS\n" : "00a69da0_TEST FAIL\n");
    return ok ? 0 : 1;
}