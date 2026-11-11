#ifndef FABLE_ENGINE_CLINKEDLIST_H
#define FABLE_ENGINE_CLINKEDLIST_H
#include "rebuild_abi.h"
class CAInputProcess;class CLinkedListPosition;class CLinkedListScanInfo;
template<class T>class CLinkedList;
// Layout: complete original Ego19edc/FW1a8cc,size16.
template<>class CLinkedList<CAInputProcess>{
private:
 CLinkedListPosition * Head;
 CLinkedListPosition * Tail;
 long NoEntries;
 CLinkedListScanInfo * FirstScanInfo;
protected:
 void AddScanInfo(CLinkedListScanInfo * p0) const;
 void RemoveScanInfo(CLinkedListScanInfo * p0) const;
 void CheckCurrentScansForRemoval(CLinkedListPosition * p0);
 void UpdateScansFromRemoval(CLinkedListPosition * p0);
public:
 CLinkedList();
 void InsertAtHead(CAInputProcess * p0, CLinkedListPosition * p1);
 void InsertAtTail(CAInputProcess * p0, CLinkedListPosition * p1);
 void InsertAfter(CAInputProcess * p0, CLinkedListPosition * p1, CLinkedListPosition * p2);
 void InsertBefore(CAInputProcess * p0, CLinkedListPosition * p1, CLinkedListPosition * p2);
 void Remove(CLinkedListPosition * p0);
 void SafeRemove(CLinkedListPosition * p0);
 void Empty();
 CAInputProcess * StartScanAtHead(CLinkedListScanInfo * p0) const;
 CAInputProcess * StartScanAtTail(CLinkedListScanInfo * p0) const;
 CAInputProcess * StartScanAtPosition(CLinkedListPosition * p0, CLinkedListScanInfo * p1) const;
 CAInputProcess * ScanForward(CLinkedListScanInfo * p0) const;
 CAInputProcess * ScanBackward(CLinkedListScanInfo * p0) const;
 void EndScan(CLinkedListScanInfo * p0) const;
 CAInputProcess * GetHead() const;
 CAInputProcess * GetTail() const;
 long GetNoEntries() const;
 void Validate() const;
};
FABLE_STATIC_ASSERT(sizeof(CLinkedList<CAInputProcess>)==16);
#endif
