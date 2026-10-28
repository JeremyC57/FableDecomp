// NUnprovenLibraryCallbacks::CGroup::VisitOne(...) -- Dispatch the callback while preserving the saved next link.
// Leaf: ?VisitOne@CGroup@NUnprovenLibraryCallbacks@@UAEXPAX@Z
// Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail caches Next before each callback and invokes the current member.
// Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.
namespace NUnprovenLibraryCallbacks{
class CGroup;
class CMember{public:virtual void UnknownSlot0(){}virtual void VisitOne(void*)=0;virtual void VisitTwo(void*,void*)=0;virtual void VisitThree(void*,void*)=0;virtual void VisitFour(void*)=0;unsigned char UnprovenFields04[44];CGroup* Group;CMember* Next;};
class CGroup{public:virtual long* GetIndexSlot(){return &Index;}virtual void VisitOne(void*);virtual void VisitTwo(void*,void*);virtual void VisitThree(void*,void*);virtual void VisitFour(void*);long Index;CMember* First;};
}
void NUnprovenLibraryCallbacks::CGroup::VisitOne(void* first){for(CMember* p=First;p;){CMember* next=p->Next;p->VisitOne(first);p=next;}}