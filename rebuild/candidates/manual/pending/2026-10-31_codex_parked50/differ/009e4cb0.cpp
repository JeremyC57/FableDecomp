// 009e4cb0 CDriveManager::GetItemFromPath
// Codex parked-50 best attempt: DIFFER(179v179); residue: 179/179 bytes; explicit-exit path scanner restores callee-saved roles but character/index EAX/ECX roles and dependent instructions differ.
// Flags:
// Leaf: ?GetItemFromPath@CDriveManager@@AAE_NPBGKPAG@Z
#include <stddef.h>
#include <string.h>
#pragma intrinsic(memcpy)
extern "C" unsigned short* __cdecl wcscat(unsigned short*,const unsigned short*);
#pragma function(wcscat)
#define CDriveManager CDriveManagerFields
#include "engine/CDriveManager.h"
#undef CDriveManager
// Layout: original singleton base is four bytes; shared header SystemManager4, size8.
// Original Ego_r TPI a411 proves private member and singleton inheritance.
template<class T>class CTBaseSingletonNoDefaultAlloc {public:virtual ~CTBaseSingletonNoDefaultAlloc();};
class CDriveManager:public CTBaseSingletonNoDefaultAlloc<CDriveManager>{
 CSystemManager*SystemManager;
 bool GetItemFromPath(const unsigned short*,unsigned long,unsigned short*);
};
bool CDriveManager::GetItemFromPath(const unsigned short* path, unsigned long index, unsigned short* item)
{
    unsigned long separators = 0;
    *item = 0;
    unsigned short c = path[0];
    long i=0;
    while(c != 0) {
        if (c == L'\\' && ++separators > index) {
            if (i == 0 || path[i - 1] == L':') {
                wcscat(item, L"\\");
                return true;
            }
            long start;
            for (long j = i - 1;; --j) {
                if (j == -1) {
                    start = 0;
                    break;
                }
                unsigned short d = path[j];
                if (d == L'\\' || d == L':' || j == 0){
                    start = j + 1;
                    if(start!=-1)break;
                }
            }
            memcpy(item, path + start, (i - start) * sizeof(unsigned short));
            item[i - start] = 0;
            return true;
        }
        c = path[++i];
    }
    return false;
}
