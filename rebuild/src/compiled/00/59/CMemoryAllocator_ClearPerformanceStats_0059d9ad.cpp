#include "rebuild_abi.h"
// Leaf: ClearPerformanceStats
// CMemoryAllocator::ClearPerformanceStats @ 0x0059d9ad
class CMemoryAllocator { public: char dummy; void Clear(int n); void ClearPerformanceStats(); };
void CMemoryAllocator::ClearPerformanceStats(){ Clear(0xc); Clear(0xb); }