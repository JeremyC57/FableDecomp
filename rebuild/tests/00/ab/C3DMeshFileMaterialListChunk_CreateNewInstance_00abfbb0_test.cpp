// behaviour test for 0x00abfbb0 C3DMeshFileMaterialListChunk::CreateNewInstance(): a fresh "MTLS" chunk of the same class is handed to
// InitNewInstance and that call's result is returned
#include <cstdio>
#include <cstring>
struct Chunk {
    char type[5];
    Chunk* initArg;
    Chunk(const char* t) : initArg(0) { std::strncpy(type, t, 4); type[4] = 0; }
    virtual ~Chunk() {}
    virtual Chunk* InitNewInstance(Chunk* c) const { c->initArg = c; return c; }
    virtual int Kind() const { return 0; }
};
struct Derived : Chunk {
    Derived() : Chunk("MTLS") {}
    virtual int Kind() const { return 1; }
    Chunk* CreateNewInstance() const { return InitNewInstance(new Derived); }
};
int main()
{
    int fails = 0;
    Derived proto;
    Chunk* c = proto.CreateNewInstance();
    if (!c || c == &proto) fails |= 1;
    else {
        if (c->Kind() != 1) fails |= 2;
        if (std::strcmp(c->type, "MTLS") != 0) fails |= 4;
        if (c->initArg != c) fails |= 8;
        delete c;
    }
    if (fails) { std::printf("00abfbb0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00abfbb0_TEST PASS\n");
    return 0;
}