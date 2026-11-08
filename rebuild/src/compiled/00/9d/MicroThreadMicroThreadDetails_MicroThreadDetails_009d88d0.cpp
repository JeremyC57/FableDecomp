// MicroThread::MicroThreadDetails::~MicroThreadDetails() -- Release owned sequence storage and destroy the base.
// Leaf: ??1MicroThreadDetails@MicroThread@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own deleting-dtor009d88b0 and full RTTI129b938 prove MicroThreadDetails; base RTTI129b928 MicroThreadInfo; original Stack+8 vector<unsigned char>.
#include "fable_compiler.h"
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "engine/CArray.h"
// Layout: no active micro-thread header; bool Dead+4 and saved-stack vector+8, confirmed by original stack-size method.
namespace MicroThread {class MicroThreadInfo {public:virtual void MarkAsDead();virtual unsigned int MicroThreadStackSize()const;virtual FABLE_FORCEINLINE ~MicroThreadInfo(){}bool Dead;};class MicroThreadDetails:public MicroThreadInfo {public:std::vector<unsigned char> Stack;}; }

void DestroyCandidate(MicroThread::MicroThreadDetails* p){p->MicroThreadDetails::~MicroThreadDetails();}