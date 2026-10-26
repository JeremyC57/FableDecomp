#include "engine/C3DMeshFilePartitionChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class C3DMeshPartitionElement { public: void N(void* stream); };
struct C3DMeshFilePartitionChunk_Methods : C3DMeshFilePartitionChunk {
    void WriteToFile(void* stream, int saver);
};
void C3DMeshFilePartitionChunk_Methods::WriteToFile(void* stream, int saver){ this->PartitionTree_Data->N(stream); }