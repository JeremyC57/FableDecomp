#pragma optimize("s",on)
// Leaf: ?ListNode_InsertBefore_0065fda8@@YGPAPAUInsertNode_0065fda8@@PAPAU1@PAU1@K@Z
struct InsertNode_0065fda8{InsertNode_0065fda8* next;InsertNode_0065fda8* prev;unsigned long value;};
InsertNode_0065fda8* __stdcall AllocateInsertNode_0065fda8(unsigned long value);
InsertNode_0065fda8** __stdcall ListNode_InsertBefore_0065fda8(InsertNode_0065fda8** output,InsertNode_0065fda8* before,unsigned long value){InsertNode_0065fda8* inserted=AllocateInsertNode_0065fda8(value);InsertNode_0065fda8* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}