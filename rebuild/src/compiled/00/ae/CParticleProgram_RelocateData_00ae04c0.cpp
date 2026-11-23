// Leaf: ?RelocateData@CParticleProgram@@UAEXJPAX0J@Z
// CParticleProgram::RelocateData  0x00ae04c0
// void __fastcall(CParticleProgram*, long a, void* oldBase, void* newBase, long d)

class CParticleProgram { public:
    char pad[0xbc];
    char* data;   // +0xc0
    virtual void RelocateData(long a, void* oldBase, void* newBase, long d);
};

void CParticleProgram::RelocateData(long a, void* oldBase, void* newBase, long d)
{
    if (this->data != 0) {
        this->data = this->data - (long)oldBase + (long)newBase;
    }
}