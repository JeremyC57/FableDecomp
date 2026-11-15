// 00c32fa0 CDefaultAudioFile::Close
// Flags: none
// Leaf: ?Close@CDefaultAudioFile@@UAEXXZ
// Descriptive libpk identity; complete name search in NAMING_EVIDENCE.md.
#include "libpk/CDefaultAudioFile.h"
#include "libpk/PKCDefaultFileManager.h"

void CDefaultAudioFile::Close()
{
    m_pManager->ReleaseFile(m_key);
    m_pManager->ReleaseFileObject(this);
}