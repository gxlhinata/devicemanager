/*
 * abi.cc
 *
 *
 */

#include "abi.h"
#include "osutils.h"
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>


#define PROC_SYS "/proc/sys"

bool scan_abi(hwNode & system)
{
    // are we compiled as 32- or 64-bit process ?
    system.setWidth(sysconf(_SC_LONG_BIT));

    pushd(PROC_SYS);

    if(exists("kernel/vsyscall64"))
    {
        system.addCapability("vsyscall64");
        system.setWidth(64);
    }

    if(chdir("abi") == 0)
    {
        int i, n;
        struct dirent **namelist;

        n = scandir(".", &namelist, selectfile, alphasort);
        for(i = 0; i < n; i++)
        {
            system.addCapability(namelist[i]->d_name);
            free(namelist[i]);
        }
        if(namelist)
            free(namelist);
    }

    popd();

    system.describeCapability("vsyscall32", "32-bit processes");
    system.describeCapability("vsyscall64", "64-bit processes");
    return true;
}
