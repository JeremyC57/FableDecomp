// PKCAutoDelete<CUnprovenTransferRecord_00C3B7A0>::~PKCAutoDelete() -- Destroy and release the owned record when nonempty.
// Leaf: ??1?$PKCAutoDelete@VCUnprovenTransferRecord_00C3B7A0@@@@QAE@XZ
// Identity: Own unwind funclet from00c2fc70 calls this destructor; sibling Release00c2f2b0 asserts actual PKCAutoDelete.h filename. Same record transfer00C3B7A0 and destructor00c3b6b0, allocation12 bytes. Original record name unknown; shared descriptive record spelling with N200/N201/N203.
// Layout: genuine libpk PKCAutoDelete<T> has one owning pointer0. Unknown record has own allocation size12; destructor is external, no fictitious fields or behavior modeled. No shared headers.
#include <new>
class CUnprovenTransferRecord_00C3B7A0{public:unsigned char Unmodeled[12];~CUnprovenTransferRecord_00C3B7A0();};
template<class T>class PKCAutoDelete{public:T* m_pObject;explicit PKCAutoDelete(T* p):m_pObject(p){}~PKCAutoDelete(){if(m_pObject){delete m_pObject;m_pObject=0;}}};
template PKCAutoDelete<CUnprovenTransferRecord_00C3B7A0>::~PKCAutoDelete();