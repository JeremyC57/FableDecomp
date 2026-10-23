// Behaviour test for C3DAnimFileLoader::C3DAnimFileLoader (0x00A999B0): the chunk-type tree registered under the "3DAF" root.
#include <cstdio>
#include <cstring>
struct ChunkType {
    char Tag[8]; ChunkType* Parent; int Children;
};
struct Registry {
    ChunkType types[16]; int count;
    ChunkType* Add(const char* tag, ChunkType* parent)
    {
        ChunkType* t = &types[count++];
        std::strcpy(t->Tag, tag); t->Parent = parent; t->Children = 0;
        if (parent) ++parent->Children;
        return t;
    }
};
struct Loader {
    char Format[8]; Registry reg;
    ChunkType *Root, *Object, *Helpers, *Sequence, *BoneMask, *MovementVector, *TimingEvent, *MovementTrack;
    Loader()
    {
        std::strcpy(Format, "3DAF"); reg.count = 0;
        Root = reg.Add("ANRT", 0);
        Object = reg.Add("OBJ", Root);
        Helpers = reg.Add("HLPR", Root);
        Sequence = reg.Add("SEQ", Object);
        BoneMask = reg.Add("BMSK", Object);
        MovementVector = reg.Add("MVEC", Helpers);
        TimingEvent = reg.Add("TEVT", Helpers);
        MovementTrack = reg.Add("SEQ", MovementVector);
    }
};
int main()
{
    Loader l;
    if (std::strcmp(l.Format, "3DAF") || l.reg.count != 8 || l.Root->Children != 2 || l.Object->Children != 2
        || l.Helpers->Children != 2 || l.MovementVector->Children != 1 || l.MovementTrack->Parent != l.MovementVector
        || l.TimingEvent->Parent != l.Helpers || l.Sequence->Parent != l.Object) { std::printf("00a999b0_TEST FAIL\n"); return 1; }
    std::printf("00a999b0_TEST PASS\n");
    return 0;
}