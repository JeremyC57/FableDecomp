#pragma optimize("s",on)
// Leaf: ?ListNode_InsertBefore_0065fd14@@YGPAPAUInsertNode_0065fd14@@PAPAU1@PAU1@K@Z
struct InsertNode_0065fd14{InsertNode_0065fd14* next;InsertNode_0065fd14* prev;unsigned long value;};
InsertNode_0065fd14* __stdcall AllocateInsertNode_0065fd14(unsigned long value);
InsertNode_0065fd14** __stdcall ListNode_InsertBefore_0065fd14(InsertNode_0065fd14** output,InsertNode_0065fd14* before,unsigned long value){InsertNode_0065fd14* inserted=AllocateInsertNode_0065fd14(value);InsertNode_0065fd14* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}