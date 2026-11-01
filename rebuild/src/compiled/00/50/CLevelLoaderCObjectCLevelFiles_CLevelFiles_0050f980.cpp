// CLevelLoader::CObject::CLevelFiles::~CLevelFiles() -- release the three counted file pointers (PThingFile +0x14, PNavigationFile +0x0c, PTopologyFile +0x04, reverse declaration order): drop the reference, destroy the object and free its count block on the last one, then clear both words
// Leaf: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050F980, length 129 bytes.
// Identity (naming wave 20, chunk 688): Ego_r.pdb CLevelLoader::CObject::CLevelFiles {LevelID, PTopologyFile, PNavigationFile, PThingFile} (28 bytes), the
//   element of the vector the landed 0x005158B0 / 0x0051A430 / 0x0051AE60 / 0x005196A0 rows model; ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ is in
//   both PDB name sets. Was the global `CLevelFiles` (the nesting was missing).
// Callees: the per-type DeleteData stored in each count block (engine/CCountedPointer.h: `__fastcall`, object in ecx), operator delete @ 0x00BFE9BC.
// Shape: engine/CCountedPointer.h's inline destructor: the count is re-read after the decrement, the zero for the two clears is the shared `xor edi,edi`;
//   the member destructors are inline and run last member first. Word +0x00 is not touched by the destructor.
#include "engine/CCountedPointer.h"

class CAFile;

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {
        public:
            long                    LevelID;             // +0x00
            CCountedPointer<CAFile> PTopologyFile;       // +0x04
            CCountedPointer<CAFile> PNavigationFile;     // +0x0c
            CCountedPointer<CAFile> PThingFile;          // +0x14
            ~CLevelFiles();
        };
    };
};

CLevelLoader::CObject::CLevelFiles::~CLevelFiles()
{
}
